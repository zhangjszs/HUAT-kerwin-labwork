# AGENTS.md

Guidance for AI coding agents working in this repository.

## What this repo is

HUAT-kerwin-labwork is a **polyglot coursework archive**: 25+ independent course
directories (C/C++, Java, Python, Android, Qt, Jupyter, SPSS…) containing lab
code, course designs, and study notes from HUAT Computer Science. There is **no
root build system, no root test suite, and no CI** (`.github/` holds only issue/PR
templates). `pyproject.toml` only configures black/isort — tooling hygiene, not a buildable root project.

Always work **inside one course directory at a time**; repo-wide builds don't exist.

## Layout

- `<course>/` — one self-contained directory per course (e.g. `data-structures/`,
  `computer-network/`, `android-mobile-development/`, `java-course-design/`).
  Each should have its own `README.md` describing structure and how to run.
- `tests/` — scratch/practice programs (cpp/java/python), **not** a test suite.
- `scripts/` — PowerShell maintenance and notes tooling.
- `docs/` — repo-facing docs: [MAINTENANCE.md](docs/MAINTENANCE.md) (temp-file
  management, README standards, naming rules), personal notes and plans, and
  [agents/](docs/agents/) (agent skill docs, linked below).
- Root [README.md](README.md) — course index with the 中文→English directory
  mapping table; [CONTRIBUTING.md](CONTRIBUTING.md) — commit/PR rules and the
  README template for new course directories.

## Conventions that differ from defaults

- **Human-facing content is in Chinese**: READMEs, commit descriptions, issues.
  Write user-visible text in Chinese; keep code identifiers in English.
- **Directory names are English kebab-case** (mapping table in the root README).
  A new course directory needs **both** its own `README.md` **and** an entry in
  the root README index — the PR template checklist enforces this.
- **Course vs. course-design directories**: when one course topic has both a
  course directory (class materials/labs, e.g. `microcomputer-principles/`) and a
  course-design directory (design output, e.g. `microcomputer-course-design/`),
  the root README index entry for each must state which it is — use the
  「（课堂实验）」/「（课程资料）」/「（课程设计）」 annotation so readers can tell
  them apart.
- **Commits**: `<type>: <description>` with `feat|fix|docs|chore|refactor`.
- **Git LFS is mandatory for binaries**: `*.doc(x)`, `*.pdf`, `*.ppt(x)`,
  `*.xls(x)`, `*.jpg`, `*.apk` are LFS-tracked in
  [.gitattributes](.gitattributes); anything >1MB must go through LFS.
  ⚠️ `*.zip`/`*.rar`/`*.7z` are LFS-tracked **and** gitignored — never
  `git add -f` archives; check with the maintainer instead.
- **Never commit temp/AI-state files**: `*~`, `*.bak`, `*.swp`, `__pycache__/`,
  `tmp/`, `.DS_Store`, `Thumbs.db`, and local AI-tool state (`.omc/`, `.claude/`,
  `.trae/`) are all gitignored — don't fight the ignore rules. Details in
  [docs/MAINTENANCE.md](docs/MAINTENANCE.md).
- **C++**: target **C++11**; the root [.clang-format](.clang-format)
  (Google-based, 4-space indent, 100 cols) applies to C/C++ code.
- **Python**: format with black + isort per [pyproject.toml](pyproject.toml)
  (line length 88).

## Build & test

No repo-wide commands exist. Read the target course directory's `README.md`
first. Typical patterns:

- C/C++ dirs (e.g. `c-course-design/`, parts of `compiler-principles/`): CMake ≥ 3.10.
- Android (`android-mobile-development/`): Gradle projects built via
  Android Studio or `gradlew`; `final_course_project/Company/` is a full app.
- Python/Jupyter dirs: create a venv **per directory**; there is no shared
  root requirements file.
- Java dirs: per-project builds (Maven/Gradle/IDE) — check each `README.md`.

## Environment notes

- **Windows-first**: development and scripts assume Windows/PowerShell.
  Line endings are enforced by [.gitattributes](.gitattributes): `*.ps1`/`*.bat`
  are CRLF, all other text is LF — keep that split when creating files.
- `docs/agents/domain.md` treats `CONTEXT.md`/`docs/adr/` as **lazy** artifacts —
  they don't exist yet; proceed silently, don't scaffold them proactively.

## Agent skills

### Issue tracker

Issues and specs live as GitHub issues. Uses the `gh` CLI. See `docs/agents/issue-tracker.md`.

### Triage labels

Five canonical triage roles with default label strings. See `docs/agents/triage-labels.md`.

### Domain docs

Single-context: one `CONTEXT.md` + `docs/adr/` at the repo root. See `docs/agents/domain.md`.
