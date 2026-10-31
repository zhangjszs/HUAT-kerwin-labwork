#!/usr/bin/env node
// 清理 Jupyter Notebook：清空运行输出、导出 markdown 附件（截图）。
// 也可作为 git clean filter 使用，等价于 nbstripout（但不依赖 Python）。
import fs from "node:fs";
import path from "node:path";
import { execFileSync } from "node:child_process";

let cachedRoot;

const MODES = new Set(["--all", "--strip", "--extract-attachments", "--check", "--filter"]);
const argv = process.argv.slice(2);
const mode = argv.find((arg) => arg.startsWith("--")) ?? "--all";
const inputs = argv.filter((arg) => !arg.startsWith("--"));

if (!MODES.has(mode)) {
  console.error(
    `未知参数：${mode}\n` +
      "用法：node scripts/tools/clean_notebooks.mjs [--check|--strip|--extract-attachments|--all] [文件...]"
  );
  process.exit(2);
}

if (mode === "--filter") {
  process.stdout.write(stripText(fs.readFileSync(0, "utf8")));
  process.exit(0);
}

const files = inputs.length ? inputs.map((file) => path.resolve(file)) : listTrackedNotebooks();
if (files.length === 0) {
  console.error("没有找到 .ipynb 文件");
  process.exit(2);
}

let dirty = 0;
let failed = 0;

for (const file of files) {
  try {
    const raw = fs.readFileSync(file, "utf8");
    const notebook = parseNotebook(raw, file);

    if (mode === "--check") {
      const issues = inspect(notebook);
      if (issues.length > 0) {
        dirty += 1;
        console.log(`${display(file)}: ${issues.join("，")}`);
      }
      continue;
    }

    let changed = false;
    if (mode === "--all" || mode === "--extract-attachments") {
      changed = extractAttachments(file, notebook) || changed;
    }
    if (mode === "--all" || mode === "--strip") {
      changed = stripNotebook(notebook) || changed;
    }

    if (!changed) {
      console.log(`已干净：${display(file)}`);
      continue;
    }

    fs.writeFileSync(file, serialize(notebook, raw));
    dirty += 1;
    console.log(`已清理：${display(file)}`);
  } catch (error) {
    failed += 1;
    console.error(`失败：${display(file)} — ${error.message}`);
  }
}

if (mode === "--check") {
  if (dirty > 0) {
    console.error(`\n${dirty} 个 notebook 仍含输出或附件，运行 --all 清理。`);
    process.exit(1);
  }
  console.log("所有 notebook 均已清理。");
}

if (failed > 0) process.exit(1);

function parseNotebook(raw, file) {
  try {
    return JSON.parse(raw);
  } catch (error) {
    throw new Error(`JSON 解析失败（${display(file)}）：${error.message}`);
  }
}

function stripNotebook(notebook) {
  let changed = false;
  for (const cell of notebook.cells ?? []) {
    if (cell.cell_type !== "code") continue;
    if (!Array.isArray(cell.outputs)) {
      cell.outputs = [];
      changed = true;
    } else if (cell.outputs.length > 0) {
      cell.outputs = [];
      changed = true;
    }
    if (cell.execution_count !== null) {
      cell.execution_count = null;
      changed = true;
    }
    if (cell.metadata && "execution" in cell.metadata) {
      delete cell.metadata.execution;
      changed = true;
    }
  }
  return changed;
}

function stripText(raw) {
  const notebook = parseNotebook(raw, "<stdin>");
  if (!stripNotebook(notebook)) return raw;
  return serialize(notebook, raw);
}

function extractAttachments(file, notebook) {
  let changed = false;
  const dir = path.dirname(file);
  const assetsDir = path.join(dir, `${path.basename(file, ".ipynb")}.assets`);
  const written = new Map();

  (notebook.cells ?? []).forEach((cell, index) => {
    if (!cell.attachments || typeof cell.attachments !== "object") return;

    let source = Array.isArray(cell.source) ? cell.source.join("") : String(cell.source ?? "");
    let extracted = 0;

    for (const [name, mimes] of Object.entries(cell.attachments)) {
      const image = Object.entries(mimes ?? {}).find(([mime]) => mime.startsWith("image/"));
      if (!image) {
        console.warn(`  跳过非图片附件（${display(file)}）：${name}`);
        continue;
      }

      const [mime, base64] = image;
      const data = Buffer.from(String(base64), "base64");
      fs.mkdirSync(assetsDir, { recursive: true });

      let fileName = path.basename(name);
      if (!/\.[A-Za-z0-9]+$/.test(fileName)) {
        fileName += `.${mimeExtension(mime)}`;
      }

      let target = path.join(assetsDir, fileName);
      const previous = written.get(target);
      if (previous && !previous.equals(data)) {
        target = path.join(assetsDir, `${index + 1}-${fileName}`);
      }
      fs.writeFileSync(target, data);
      written.set(target, data);
      extracted += 1;

      const url = encodeURI(path.relative(dir, target).split(path.sep).join("/"));
      const token = `attachment:${name}`;
      if (source.includes(token)) {
        source = source.split(token).join(url);
      } else {
        console.warn(`  附件未被引用（已导出）：${display(file)} / ${name}`);
      }
    }

    if (extracted === 0) return;

    cell.source = Array.isArray(cell.source) ? toSourceLines(source) : source;
    delete cell.attachments;
    changed = true;
  });

  return changed;
}

function inspect(notebook) {
  let outputs = 0;
  let executionCounts = 0;
  let attachments = 0;
  let timings = 0;

  for (const cell of notebook.cells ?? []) {
    outputs += cell.outputs?.length ?? 0;
    if (cell.execution_count != null) executionCounts += 1;
    attachments += Object.keys(cell.attachments ?? {}).length;
    if (cell.metadata?.execution) timings += 1;
  }

  const issues = [];
  if (outputs > 0) issues.push(`${outputs} 个输出`);
  if (executionCounts > 0) issues.push(`${executionCounts} 个执行计数`);
  if (attachments > 0) issues.push(`${attachments} 个附件`);
  if (timings > 0) issues.push(`${timings} 个执行时间戳`);
  return issues;
}

function serialize(notebook, original) {
  return `${JSON.stringify(notebook, null, detectIndent(original))}\n`;
}

function detectIndent(raw) {
  const match = raw.match(/^( +)\S/m);
  return match ? Math.min(match[1].length, 8) : 2;
}

function toSourceLines(text) {
  if (!text) return [""];
  const lines = text.split(/(?<=\n)/);
  if (lines[lines.length - 1] === "") lines.pop();
  return lines;
}

function mimeExtension(mime) {
  const subtype = mime.slice("image/".length).split("+")[0].toLowerCase();
  return subtype === "jpeg" ? "jpg" : subtype.replace(/[^a-z0-9]/g, "");
}

function listTrackedNotebooks() {
  const root = repoRoot();
  const output = execFileSync("git", ["ls-files", "-z", "--", "*.ipynb"], {
    cwd: root,
    encoding: "buffer",
    maxBuffer: 64 * 1024 * 1024,
  });
  return output
    .toString("utf8")
    .split("\0")
    .filter(Boolean)
    .map((file) => path.resolve(root, file));
}

function repoRoot() {
  if (cachedRoot) return cachedRoot;
  try {
    cachedRoot = execFileSync("git", ["rev-parse", "--show-toplevel"], { encoding: "utf8" }).trim();
  } catch {
    cachedRoot = process.cwd();
  }
  return cachedRoot;
}

function display(file) {
  return path.relative(repoRoot(), file).split(path.sep).join("/") || file;
}
