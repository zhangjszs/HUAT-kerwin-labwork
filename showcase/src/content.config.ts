import { defineCollection } from 'astro:content';
import { glob } from 'astro/loaders';
import { z } from 'astro/zod';

export const collections = {
	projects: defineCollection({
		loader: glob({ base: './src/content/projects', pattern: '**/*.mdx' }),
		schema: z.object({
			title: z.string(),
			description: z.string(),
			repoPath: z.string(),
			tags: z.array(z.string()),
			keywords: z.array(z.string()),
			techStack: z.array(z.string()),
			screenshotsDir: z.string(),
			codeSnippets: z.array(z.string()).optional(),
			demoUrl: z.string().optional(),
			externalDeployUrl: z.string().optional(),
			featured: z.boolean().default(false),
			date: z.string().optional(),
			difficulty: z.enum(['入门', '进阶', '深度']).optional(),
			status: z.enum(['Completed', 'In Progress', 'Archived']).optional().default('Completed'),
			architecture: z.string().optional(),
			challenges: z.array(z.string()).optional(),
			lessons: z.array(z.string()).optional(),
		}),
	}),
	'deep-dives': defineCollection({
		loader: glob({ base: './src/content/deep-dives', pattern: '**/*.mdx' }),
		schema: z.object({
			title: z.string(),
			description: z.string(),
			date: z.string().optional(),
			tags: z.array(z.string()).optional().default([]),
			relatedProject: z.string().optional(),
		}),
	}),
};
