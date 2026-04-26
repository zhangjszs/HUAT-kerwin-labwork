#!/usr/bin/env node
/**
 * CLI: Check project assets integrity (screenshots, code snippets).
 * Usage: node scripts/check-assets.mjs
 */

import { readFileSync, existsSync, statSync, readdirSync } from 'fs';
import { join, dirname, resolve } from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const SHOWCASE_ROOT = resolve(__dirname, '..');
const PROJECTS_DIR = join(SHOWCASE_ROOT, 'src', 'content', 'projects');
const SCREENSHOTS_DIR = join(SHOWCASE_ROOT, 'public', 'screenshots');
const CODE_SNIPPETS_DIR = join(SHOWCASE_ROOT, 'public', 'code-snippets');

const SIZE_WARN = 200 * 1024; // 200KB
const SIZE_ERROR = 500 * 1024; // 500KB

function getAllProjects() {
	if (!existsSync(PROJECTS_DIR)) return [];
	const files = readdirSync(PROJECTS_DIR);
	return files.filter((f) => f.endsWith('.mdx') || f.endsWith('.md'));
}

function parseFrontmatter(content) {
	const match = content.match(/^---\s*\n([\s\S]*?)\n---/);
	if (!match) return null;

	const lines = match[1].split('\n');
	const data = {};
	let currentKey = null;
	let currentArray = [];

	for (const line of lines) {
		const arrayMatch = line.match(/^  - (.+)$/);
		if (arrayMatch && currentKey) {
			currentArray.push(arrayMatch[1].trim().replace(/^["']|["']$/g, ''));
			continue;
		}

		const keyMatch = line.match(/^([a-zA-Z0-9_]+):\s*(.+)?$/);
		if (keyMatch) {
			if (currentKey && currentArray.length > 0) {
				data[currentKey] = currentArray;
				currentArray = [];
			}
			currentKey = keyMatch[1];
			const value = keyMatch[2] ? keyMatch[2].trim() : '';
			if (value.startsWith('[') && value.endsWith(']')) {
				// Inline array: ["a", "b"]
				try {
					data[currentKey] = JSON.parse(value.replace(/'/g, '"'));
				} catch {
					data[currentKey] = [];
				}
			} else if (value === '') {
				// Multi-line array starting
				currentArray = [];
			} else {
				data[currentKey] = value.replace(/^["']|["']$/g, '');
			}
		}
	}

	if (currentKey && currentArray.length > 0) {
		data[currentKey] = currentArray;
	}

	return data;
}

function formatSize(bytes) {
	if (bytes < 1024) return `${bytes}B`;
	if (bytes < 1024 * 1024) return `${(bytes / 1024).toFixed(1)}KB`;
	return `${(bytes / (1024 * 1024)).toFixed(1)}MB`;
}

function checkAssets() {
	const projects = getAllProjects();
	let totalProjects = 0;
	let okCount = 0;
	let warnCount = 0;
	let errorCount = 0;
	const results = [];

	for (const file of projects) {
		totalProjects++;
		const filePath = join(PROJECTS_DIR, file);
		const content = readFileSync(filePath, 'utf-8');
		const frontmatter = parseFrontmatter(content);

		if (!frontmatter) {
			results.push({ status: 'error', file, message: 'No frontmatter found' });
			errorCount++;
			continue;
		}

		const slug = file.replace(/\.(mdx|md)$/, '');
		const screenshotsDir = frontmatter.screenshotsDir || slug;
		const codeSnippets = frontmatter.codeSnippets || [];

		let projectErrors = [];
		let projectWarns = [];

		// Check screenshots directory
		const screenshotPath = join(SCREENSHOTS_DIR, screenshotsDir);
		if (!existsSync(screenshotPath)) {
			projectErrors.push(`Missing screenshot dir: screenshots/${screenshotsDir}/`);
		} else {
			const files = readdirSync(screenshotPath).filter((f) => /\.(png|jpe?g|webp|gif)$/i.test(f));
			if (files.length === 0) {
				projectWarns.push(`Screenshot dir exists but is empty`);
			}
			for (const img of files) {
				const imgPath = join(screenshotPath, img);
				const size = statSync(imgPath).size;
				if (size > SIZE_ERROR) {
					projectErrors.push(`Image too large: ${img} (${formatSize(size)})`);
				} else if (size > SIZE_WARN) {
					projectWarns.push(`Image large: ${img} (${formatSize(size)})`);
				}
			}
		}

		// Check code snippets
		for (const snippet of codeSnippets) {
			const snippetPath = join(CODE_SNIPPETS_DIR, snippet);
			if (!existsSync(snippetPath)) {
				projectErrors.push(`Missing code snippet: code-snippets/${snippet}`);
			}
		}

		if (projectErrors.length > 0) {
			results.push({ status: 'error', file, messages: projectErrors });
			errorCount++;
		} else if (projectWarns.length > 0) {
			results.push({ status: 'warn', file, messages: projectWarns });
			warnCount++;
		} else {
			results.push({ status: 'ok', file, message: 'All assets OK' });
			okCount++;
		}
	}

	// Print results
	console.log(`\n📊 Asset Check Report`);
	console.log(`========================`);
	console.log(`Total projects: ${totalProjects}`);
	console.log(`✅ OK: ${okCount}`);
	console.log(`⚠️  Warnings: ${warnCount}`);
	console.log(`❌ Errors: ${errorCount}`);
	console.log('');

	for (const r of results) {
		if (r.status === 'ok') {
			console.log(`✅ ${r.file}: ${r.message}`);
		} else if (r.status === 'warn') {
			console.log(`⚠️  ${r.file}:`);
			for (const m of r.messages) console.log(`   - ${m}`);
		} else {
			console.log(`❌ ${r.file}:`);
			for (const m of r.messages) console.log(`   - ${m}`);
		}
	}

	console.log('');
	if (errorCount > 0) {
		console.log(`❌ ${errorCount} project(s) have errors. Fix them before deploying.`);
		process.exit(1);
	} else if (warnCount > 0) {
		console.log(`⚠️  ${warnCount} project(s) have warnings. Review recommended.`);
		process.exit(0);
	} else {
		console.log('✅ All assets look good!');
		process.exit(0);
	}
}

checkAssets();
