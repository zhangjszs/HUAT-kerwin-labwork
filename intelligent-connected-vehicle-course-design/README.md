# 智能网联汽车技术课程设计

![Web](https://img.shields.io/badge/Web-HTML5%20%7C%20CSS3%20%7C%20JavaScript-orange.svg)
![Python](https://img.shields.io/badge/Python-PyTorch%20%7C%20YOLOv7-blue.svg)

《智能网联汽车技术》课程设计成果，包含两个作品：

- **[FSD 演示网站](./fsd_presentation_website_html/)** —— 「Formula Student Driverless」自动驾驶算法方案演示站点（主页 + 14 张幻灯片，约 160 KB HTML + 9 张图片）
- **[YOLOv7 车牌识别系统](./yolov7_plate_UI_camera/)** —— 「检测 → 矫正 → 识别」两阶段车牌识别桌面应用，支持摄像头实时检测

## 📁 目录结构

```text
intelligent-connected-vehicle-course-design/
├── fsd_presentation_website_html/    # 作品一：FSD 演示网站（HTML/CSS/JS）
│   ├── index.html                    # 主页（幻灯片导航外壳）
│   ├── *.html                        # 14 张幻灯片页面
│   └── images/                       # 图片资源
└── yolov7_plate_UI_camera/           # 作品二：YOLOv7 车牌识别系统（Python）
```

## 🌐 作品一：FSD 演示网站

静态站点形式的演示文稿，围绕「Formula Student Driverless - 自动驾驶算法系统」展开：从项目背景与系统架构，到感知 / 建图 / 规划三大核心模块，再到仿真平台的方案探索、实现与成果，完整呈现一套自动驾驶算法方案。

**15 个页面一览**：

| 分组 | 页面 |
| ---- | ---- |
| 导航与开篇 | `index.html` 主页与幻灯片总览 · `cover.html` 封面 · `contents.html` 目录 |
| 背景与架构 | `background.html` 项目背景与挑战 · `architecture.html` 系统总体架构 |
| 核心模块 | `perception.html` 环境感知（看懂赛道） · `mapping.html` 状态估计与建图 · `planning.html` 规划与控制 |
| 仿真平台 | `simulation_background.html` 背景与技术栈 · `simulation_exploration.html` 方案探索 · `simulation_implementation.html` 核心实现 · `simulation_results.html` 仿真成果 |
| 成果与收尾 | `achievements.html` 成果展示 · `conclusion.html` 总结与展望 · `qa.html` 感谢聆听 & Q&A |

- **技术栈**：HTML5 / CSS3 / JavaScript；Tailwind CSS、Font Awesome、Chart.js、D3.js、思源黑体（均通过 CDN 引入）；响应式设计，支持键盘（空格 / 方向键 / ESC）与触摸导航
- **本地运行**：克隆仓库后直接双击 `fsd_presentation_website_html/index.html`，或在 `fsd_presentation_website_html/` 目录下执行 `python -m http.server 8000` 并访问 `http://localhost:8000`
- **完整说明**（幻灯片清单、自定义主题、故障排除）：见 [fsd_presentation_website_html/README.md](./fsd_presentation_website_html/README.md)

## 🚗 作品二：基于 YOLOv7 的车牌识别系统

采用「YOLOv7-lite 检测 + 角点回归 → 透视矫正 → LPRNet 识别」流水线，PyQt5 图形界面，支持图片、视频与摄像头实时检测，覆盖蓝牌、绿牌、黄牌、港澳牌与警用牌等单 / 双层车牌。

- **技术栈**：Python、PyTorch、YOLOv7-lite、LPRNet、PyQt5
- **运行方式**：`python detect_ui.py`（或双击 `run.bat`；命令行用法见内层 README）
- **环境配置与使用说明**：见 [yolov7_plate_UI_camera/README.md](./yolov7_plate_UI_camera/README.md)

## ⚠️ 注意事项

- 站点页面与图片、车牌识别源码与权重均为普通 Git 对象，克隆后即可使用
- 打包版 `fsd_presentation_website_html.zip`（与本目录站点内容不完全一致，非运行所需）由 Git LFS 管理；如需获取该压缩包，请先执行 `git lfs pull`
- FSD 站点部分幻灯片的历史图片引用路径与实际不符，本地预览时个别图片无法显示，详见其 README 的「故障排除」节

---

> 📁 本项目是 [HUAT-kerwin-labwork](https://github.com/zhangjszs/HUAT-kerwin-labwork) 课程作品集的一部分，项目展示页见 [huat-showcase](https://github.com/zhangjszs/huat-showcase)。
