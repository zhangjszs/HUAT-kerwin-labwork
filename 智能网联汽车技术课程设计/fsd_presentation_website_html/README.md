# Formula Student Driverless 演示文稿网站 (纯HTML版)

## 项目简介

这是一个纯HTML、CSS和JavaScript的Web应用，用于展示"Formula Student Driverless - 自动驾驶算法系统"的演示文稿。该网站包含14张幻灯片，涵盖了从项目背景到技术实现的完整内容。

## 功能特性

- 📱 响应式设计，支持桌面和移动设备
- 🎯 交互式幻灯片导航
- ⌨️ 键盘快捷键支持（空格键/右箭头：下一页，左箭头：上一页，ESC：退出）
- 🎨 专业的视觉设计，采用"北理红"主题色
- 📊 包含动态图表和数据可视化 (通过CDN引入Chart.js和D3.js)

## 幻灯片内容

1. **封面页** - 项目标题和团队信息
2. **目录** - 演示文稿内容概览
3. **项目背景与挑战** - FSD赛事介绍和技术挑战
4. **系统总体架构** - 软硬件系统架构图
5. **环境感知** - LiDAR和视觉感知算法
6. **状态估计与建图** - 定位和地图构建技术
7. **规划与控制** - 路径规划和车辆控制
8. **仿真平台背景** - 仿真技术栈介绍
9. **方案探索** - 仿真系统设计方案对比
10. **核心实现** - 仿真环境技术实现
11. **仿真成果** - 测试平台和应用效果
12. **成果展示** - 性能指标和比赛成绩
13. **总结与展望** - 项目总结和未来规划
14. **感谢聆听 & Q&A** - 致谢和联系方式

## 如何部署

由于这是一个纯HTML网站，部署非常简单，您不需要任何后端服务器或数据库。只需将项目文件放置在任何Web服务器（如Apache, Nginx, IIS）的根目录或子目录中即可。

### 步骤：

1. **下载并解压**：下载提供的 `fsd_presentation_website_html.zip` 压缩包，并将其解压到您希望部署的目录。

2. **放置到Web服务器**：
   - **如果您有Web服务器**：将解压后的 `fsd_presentation_website_html` 文件夹中的所有内容（包括 `index.html`、所有幻灯片HTML文件和 `images` 文件夹）复制到您的Web服务器的网站根目录（例如：Apache的 `htdocs` 目录，Nginx的 `html` 目录）。
   - **如果您没有Web服务器（本地预览）**：直接用浏览器打开 `index.html` 文件即可预览。

3. **访问网站**：
   - 如果您部署到Web服务器，通过服务器的域名或IP地址访问，例如 `http://yourdomain.com/` 或 `http://localhost/`。
   - 如果是本地预览，直接在浏览器中打开 `index.html` 文件。

### 示例 (使用Python的简单HTTP服务器进行本地测试):

如果您只是想快速在本地测试，而没有安装专业的Web服务器，可以使用Python内置的HTTP服务器：

1. 打开命令行工具（CMD 或 PowerShell）。
2. 导航到 `fsd_presentation_website_html` 文件夹：
   ```bash
   cd path\to\your\fsd_presentation_website_html
   ```
3. 运行Python的HTTP服务器：
   ```bash
   python -m http.server 8000
   ```
4. 在浏览器中访问 `http://localhost:8000`。

## 项目结构

```
fsd_presentation_website_html/
├── index.html                 # 网站主页和导航
├── cover.html                 # 封面页
├── contents.html              # 目录页
├── background.html            # 项目背景与挑战
├── architecture.html          # 系统总体架构
├── perception.html            # 核心模块I - 环境感知
├── mapping.html               # 核心模块II - 状态估计与建图
├── planning.html              # 核心模块III - 规划与控制
├── simulation_background.html # 仿真平台：背景与技术栈
├── simulation_exploration.html# 方案探索
├── simulation_implementation.html # 核心实现
├── simulation_results.html    # 仿真成果
├── achievements.html          # 成果展示
├── conclusion.html            # 总结与展望
├── qa.html                    # 感谢聆听 & Q&A
└── images/                    # 包含所有图片资源的文件夹
    ├── *.webp
    ├── *.png
    └── *.jpg
```

## 使用说明

### 主页导航

- 点击"开始演示"按钮从第一张幻灯片开始
- 点击任意幻灯片卡片直接跳转到对应页面
- 使用顶部导航快速访问封面和目录

### 幻灯片控制

- **鼠标控制**：点击右上角的"上一页"/"下一页"按钮
- **键盘控制**：
  - 空格键或右箭头：下一页
  - 左箭头：上一页
  - ESC键：退出幻灯片模式
- **页面计数**：右下角显示当前页码和总页数

### 移动设备支持

网站采用响应式设计，在手机和平板设备上也能良好显示。移动设备上可以通过触摸滑动进行导航。

## 技术栈

- **前端**：HTML5, CSS3, JavaScript
- **样式框架**：Tailwind CSS (通过CDN引入)
- **图标库**：Font Awesome (通过CDN引入)
- **图表库**：Chart.js, D3.js (通过CDN引入)
- **字体**：思源黑体 (Noto Sans SC, 通过Google Fonts CDN引入)

## 自定义配置

### 修改主题色

在各个HTML文件的`<style>`标签中，主要颜色变量为：
- 主色调：`#C8102E` (北理红)
- 辅助色：`#003366` (深蓝)
- 背景色：`#F5F5F5` (浅灰)

### 添加新幻灯片

1. 在 `fsd_presentation_website_html/` 目录下创建新的HTML文件。
2. 在 `index.html` 文件中的JavaScript `slides` 数组中添加新幻灯片的ID（文件名，不带`.html`）。
3. 在 `index.html` 的幻灯片网格 (`.slide-grid`) 中添加对应的卡片。

### 修改团队信息

在 `index.html` 和各个幻灯片HTML文件的 `<footer>` 部分修改团队名称和联系方式。

## 故障排除

### 常见问题

1. **图片无法显示**
   - 检查图片路径是否正确。所有图片都应位于 `images/` 文件夹中，并在HTML中引用时使用相对路径 `images/your_image.webp`。
   - 确保图片文件存在于 `fsd_presentation_website_html/images/` 目录中。

2. **样式异常**
   - 检查CDN链接（如Tailwind CSS, Font Awesome, Google Fonts）是否可访问，确保您的网络连接正常。
   - 确保HTML文件中的CSS引用路径正确。

3. **JavaScript功能异常**
   - 检查浏览器控制台（按F12打开开发者工具）是否有错误信息。
   - 确保所有JavaScript库（Chart.js, D3.js）通过CDN正确加载。

## 联系方式

- 团队：东风HUAT无人驾驶车队
- 邮箱：bitfsd@bit.edu.cn
- 网站：bitfsd.bit.edu.cn
- 代码仓库：github.com/bitfsd/fsd_algorithm

## 许可证

本项目仅供学术交流和教育使用。

