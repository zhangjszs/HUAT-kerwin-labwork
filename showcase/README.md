# HUAT Labwork 课程档案

课程档案的独立站点部署在 [Cloudflare Pages](https://huat-kerwin-labwork.pages.dev/)。源码和课程资料在本仓库的各课程目录，项目说明与技术文章由这个目录的 Astro 应用生成。

## 本地开发

使用 Node.js 22.12 或更高版本。从仓库根目录执行：

```sh
cd showcase
npm ci
npm run dev
```

使用 `npm run check` 检查类型，然后运行 `npm run build` 生成 `showcase/dist`。`npm run preview` 可预览构建产物。

## Cloudflare Pages

现有项目名为 `huat-kerwin-labwork`。如果使用 Git 集成，在 Pages 项目的构建设置中将根目录设为 `showcase`、构建命令设为 `npm run build`、输出目录设为 `dist`。构建环境需要 Node.js 22.12 或更高版本。

对于已授权的 Wrangler 会话，可在 `showcase` 目录构建后上传静态产物：

```sh
npx wrangler pages deploy dist --project-name huat-kerwin-labwork
```
