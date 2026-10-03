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

## 本地运行

本目录是纯静态站点，无需安装任何依赖，直接从 git 克隆即可运行。

1. 克隆仓库：

   ```bash
   git clone https://github.com/zhangjszs/HUAT-kerwin-labwork.git
   cd HUAT-kerwin-labwork/intelligent-connected-vehicle-course-design/fsd_presentation_website_html
   ```

2. 选择一种方式启动：

   - **方式一（推荐）：Python 内置 HTTP 服务器**

     ```bash
     python -m http.server 8000
     ```

     然后在浏览器访问 `http://localhost:8000`。

   - **方式二：直接打开文件**

     用浏览器直接打开本目录下的 `index.html` 即可预览（注意：部分浏览器对 `file://` 协议加载本地资源有额外限制，遇到问题时请改用方式一）。

3. 部署到 Web 服务器（可选）：将本目录全部文件（含 `images/`）复制到任何静态站点服务器（Apache / Nginx / IIS）的根目录或子目录即可，无需后端与数据库。

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

1. **部分幻灯片图片无法显示（已知遗留问题）**
   - `cover.html`、`background.html`、`perception.html`、`mapping.html`、`simulation_background.html`、`simulation_implementation.html` 中的图片使用的是原开发环境的绝对路径（形如 `/home/ubuntu/fsd_presentation/images/...`），在克隆后的本地环境无法解析。这是站点当前的遗留问题，修复需要修改相应 HTML 文件。
   - 其中 `background.html` 与 `simulation_implementation.html` 引用的 3 张图片（`competition_scene.webp`、`race_car_close_up.webp`、`simulation_implementation.webp`）在仓库中不存在，即使用相对路径也无法显示，属素材缺失。

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

---

> 📁 本项目是 [HUAT-kerwin-labwork](https://github.com/zhangjszs/HUAT-kerwin-labwork) 课程作品集的一部分，项目展示页见 [huat-showcase](https://github.com/zhangjszs/huat-showcase)。
