#!/usr/bin/env node
/**
 * CLI: Generate a new project MDX stub from an existing project directory.
 * Usage: node scripts/new-project.mjs <relative-project-path> [options]
 * Example: node scripts/new-project.mjs javaweb-course-design/ForestBlog-master
 */

import { readFileSync, existsSync, mkdirSync, writeFileSync, readdirSync, statSync } from 'fs';
import { join, dirname, basename, resolve } from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const REPO_ROOT = resolve(__dirname, '..', '..');
const SHOWCASE_ROOT = resolve(__dirname, '..');
const PROJECTS_DIR = join(SHOWCASE_ROOT, 'src', 'content', 'projects');
const SCREENSHOTS_DIR = join(SHOWCASE_ROOT, 'public', 'screenshots');
const CODE_SNIPPETS_DIR = join(SHOWCASE_ROOT, 'public', 'code-snippets');

function slugify(str) {
	return str
		.toLowerCase()
		.replace(/[^a-z0-9\s-]/g, '')
		.trim()
		.replace(/\s+/g, '-');
}

function inferTags(repoPath) {
	const tags = [];
	const lower = repoPath.toLowerCase();

	if (lower.includes('java')) tags.push('Java');
	if (lower.includes('python')) tags.push('Python');
	if (lower.includes('cpp') || lower.includes('c++') || lower.includes('qt')) tags.push('C++');
	if (lower.includes('c-course') || lower.includes('data-structures') || lower.includes('compiler')) tags.push('C');
	if (lower.includes('android')) tags.push('Android');
	if (lower.includes('web') || lower.includes('jsp') || lower.includes('servlet')) tags.push('Web');
	if (lower.includes('network')) tags.push('Network');
	if (lower.includes('ml') || lower.includes('yolo') || lower.includes('titanic')) tags.push('Machine Learning');
	if (lower.includes('data-analysis')) tags.push('Data Analysis');

	if (tags.length === 0) tags.push('Other');
	return tags;
}

function inferTechStack(repoPath, readmeContent) {
	const techs = [];
	const lower = (repoPath + ' ' + (readmeContent || '')).toLowerCase();

	const techMap = {
		'Spring': 'spring',
		'SpringMVC': 'springmvc',
		'Spring Boot': 'spring boot',
		'MyBatis': 'mybatis',
		'MySQL': 'mysql',
		'React': 'react',
		'Vue': 'vue',
		'Qt': 'qt',
		'PyTorch': 'pytorch',
		'TensorFlow': 'tensorflow',
		'YOLO': 'yolo',
		'OpenCV': 'opencv',
		'Pandas': 'pandas',
		'NumPy': 'numpy',
		'Jupyter': 'jupyter',
		'JSP': 'jsp',
		'Servlet': 'servlet',
		'Maven': 'maven',
		'Gradle': 'gradle',
		'CMake': 'cmake',
	};

	for (const [name, keyword] of Object.entries(techMap)) {
		if (lower.includes(keyword)) techs.push(name);
	}

	return techs;
}

function extractDescription(readmeContent) {
	if (!readmeContent) return '';
	const lines = readmeContent.split('\n');
	let description = '';
	for (const line of lines) {
		const trimmed = line.trim();
		if (trimmed && !trimmed.startsWith('#') && !trimmed.startsWith('![') && !trimmed.startsWith('<!--')) {
			description = trimmed;
			break;
		}
	}
	if (description.length > 200) {
		description = description.slice(0, 200) + '...';
	}
	return description;
}

function scanSourceFiles(projectDir) {
	// Scan for interesting source files to suggest as code snippets
	const candidates = [];
	const extensions = ['.java', '.py', '.cpp', '.c', '.h', '.js', '.xml', '.sql'];

	function scan(dir, depth = 0) {
		if (depth > 3) return;
		try {
			const items = readdirSync(dir, { withFileTypes: true });
			for (const item of items) {
				if (item.isDirectory() && !item.name.startsWith('.') && item.name !== 'node_modules' && item.name !== 'target' && item.name !== 'build') {
					scan(join(dir, item.name), depth + 1);
				} else if (item.isFile()) {
					const ext = item.name.slice(item.name.lastIndexOf('.'));
					if (extensions.includes(ext) && item.name.length < 50) {
						const fullPath = join(dir, item.name);
						const size = statSync(fullPath).size;
						if (size > 100 && size < 50000) { // Between 100B and 50KB
							candidates.push(fullPath);
						}
					}
				}
			}
		} catch {}
	}

	scan(projectDir);

	// Pick up to 2 files
	const picked = candidates.slice(0, 2);
	return picked.map(p => basename(p));
}

function generateFileTree(projectDir) {
	let tree = '';
	const baseName = basename(projectDir);
	tree += `${baseName}/\n`;

	try {
		const items = readdirSync(projectDir, { withFileTypes: true });
		const dirs = items.filter(i => i.isDirectory() && !i.name.startsWith('.')).map(i => i.name).sort();
		const files = items.filter(i => i.isFile()).map(i => i.name).sort();

		const all = [...dirs.map(d => ({ name: d, isDir: true })), ...files.map(f => ({ name: f, isDir: false }))];

		for (let i = 0; i < all.length; i++) {
			const isLast = i === all.length - 1;
			const prefix = isLast ? '└── ' : '├── ';
			tree += `${prefix}${all[i].name}${all[i].isDir ? '/' : ''}\n`;
		}
	} catch {
		tree += '└── (empty)\n';
	}

	return tree;
}

function generateMDX(projectDir, options = {}) {
	const projectName = basename(projectDir);
	const slug = slugify(projectName);
	const repoPath = projectDir.replace(/\\/g, '/');
	const absoluteProjectDir = join(REPO_ROOT, projectDir);

	if (!existsSync(absoluteProjectDir)) {
		console.error(`❌ Project directory not found: ${absoluteProjectDir}`);
		process.exit(1);
	}

	// Read README if exists
	const readmePaths = ['README.md', 'readme.md', 'Readme.md', 'README.MD'];
	let readmeContent = '';
	for (const r of readmePaths) {
		const readmePath = join(absoluteProjectDir, r);
		if (existsSync(readmePath)) {
			readmeContent = readFileSync(readmePath, 'utf-8');
			break;
		}
	}

	const title = projectName.replace(/[-_]/g, ' ').replace(/\b\w/g, (c) => c.toUpperCase());
	const description = options.description || extractDescription(readmeContent);
	const tags = options.tags || inferTags(repoPath);
	const keywords = options.keywords || [];
	const techStack = options.techStack || inferTechStack(repoPath, readmeContent);
	const screenshotsDir = slug;
	const featured = options.featured || false;

	// Auto-scan for code snippet candidates
	const sourceFiles = scanSourceFiles(absoluteProjectDir);
	const snippetPaths = sourceFiles.length > 0
		? sourceFiles.map(f => `"${screenshotsDir}/${f}"`).join(', ')
		: '';

	// Generate file tree
	const fileTree = generateFileTree(absoluteProjectDir);

	const mdxContent = `---
title: "${title}"
description: "${description}"
repoPath: "${repoPath}"
tags: [${tags.map((t) => `"${t}"`).join(', ')}]
keywords: [${keywords.map((k) => `"${k}"`).join(', ')}]
techStack: [${techStack.map((t) => `"${t}"`).join(', ')}]
screenshotsDir: "${screenshotsDir}"
${snippetPaths ? `codeSnippets: [${snippetPaths}]` : ''}
featured: ${featured}
date: ""
difficulty: "进阶"
status: "Completed"
architecture: ""
challenges: []
lessons: []
---

## 项目背景

${description || 'TODO: 添加项目背景与目标'}

## 系统架构

{/* 添加架构图：
<MermaidDiagram chart={\`
graph TD
    A[Client] --> B[Frontend]
    B --> C[Backend API]
    C --> D[(Database)]
\`} caption="System Architecture" />
*/}

## 核心功能

- TODO: 列出核心功能点

## 项目结构

<FileTree tree={\`
${fileTree}\`} caption="Project Directory Structure" />

## 核心代码

{/* 添加代码片段：
<CodePreview src="/code-snippets/${screenshotsDir}/example.java" lang="java" caption="Main Controller" />
*/}

## 效果展示

<ScreenshotGallery dir="${screenshotsDir}" />

## 实现难点

TODO: 描述实现过程中遇到的技术难点与解决方案

## 总结反思

TODO: 技术收获与可改进点
`;

	// Write MDX file
	const mdxPath = join(PROJECTS_DIR, `${slug}.mdx`);
	if (!existsSync(PROJECTS_DIR)) {
		mkdirSync(PROJECTS_DIR, { recursive: true });
	}
	writeFileSync(mdxPath, mdxContent);
	console.log(`✅ Created: ${mdxPath}`);

	// Create screenshot directory placeholder
	const screenshotPath = join(SCREENSHOTS_DIR, screenshotsDir);
	if (!existsSync(screenshotPath)) {
		mkdirSync(screenshotPath, { recursive: true });
		console.log(`📁 Created screenshot dir: ${screenshotPath}`);
	} else {
		console.log(`📁 Screenshot dir already exists: ${screenshotPath}`);
	}

	// Create code snippets directory placeholder
	const codePath = join(CODE_SNIPPETS_DIR, screenshotsDir);
	if (!existsSync(codePath)) {
		mkdirSync(codePath, { recursive: true });
		console.log(`📁 Created code snippets dir: ${codePath}`);
	} else {
		console.log(`📁 Code snippets dir already exists: ${codePath}`);
	}

	// Copy detected source files as snippet stubs
	for (const file of sourceFiles) {
		const srcPath = join(absoluteProjectDir, file);
		const destPath = join(codePath, file);
		if (existsSync(srcPath) && !existsSync(destPath)) {
			try {
				const content = readFileSync(srcPath, 'utf-8');
				// Only copy if under 10KB
				if (content.length < 10000) {
					writeFileSync(destPath, content);
					console.log(`📄 Copied snippet: ${file}`);
				}
			} catch {}
		}
	}

	console.log(`\n⚠️  Next steps:`);
	console.log(`   1. Add screenshots to: public/screenshots/${screenshotsDir}/`);
	console.log(`   2. Review copied snippets in: public/code-snippets/${screenshotsDir}/`);
	console.log(`   3. Edit architecture, challenges, lessons in: ${mdxPath}`);
	console.log(`   4. Run: npm run check-assets`);

	return mdxPath;
}

// CLI entry
const projectDir = process.argv[2];
if (!projectDir) {
	console.log('Usage: node scripts/new-project.mjs <relative-project-path>');
	console.log('Example: node scripts/new-project.mjs javaweb-course-design/ForestBlog-master');
	process.exit(1);
}

generateMDX(projectDir);
