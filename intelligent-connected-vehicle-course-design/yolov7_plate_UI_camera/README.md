# 基于 YOLOv7 的智能车牌识别系统

## 项目简介

本项目是智能网联汽车课程设计作品，实现了一套完整的车牌检测与识别系统。系统采用 YOLOv7-lite 进行车牌区域检测及四个关键点定位，结合 LPRNet 完成车牌字符识别，并通过 PyQt5 构建了深色科技风格的图形化操作界面，支持图片、视频及摄像头实时检测。

## 功能特性

- 车牌检测：基于 YOLOv7-lite 检测车牌区域，同时回归 4 个关键点用于透视矫正
- 车牌识别：基于 LPRNet 对矫正后的车牌图像进行字符识别
- 多类型车牌支持：蓝牌、绿牌（新能源）、黄牌、港澳牌、警用牌等单层及双层车牌
- 多输入源：支持图片文件、视频文件、摄像头实时画面
- 图形界面：PyQt5 深色科技风主题，操作直观
- 置信度显示：实时展示检测置信度
- 截图保存：一键保存当前检测画面
- 识别历史：带时间戳的识别记录，方便回溯
- 状态栏提示：实时反馈系统运行状态

## 环境要求

- Python 3.8+
- PyTorch（CPU 或 CUDA 版本均可）
- 其他依赖详见 `requirements.txt`

## 安装步骤

1. 克隆或下载本项目到本地：

```bash
git clone <仓库地址>
cd yolov7_plate_UI_camera
```

2. 创建虚拟环境（推荐）：

```bash
python -m venv venv
# Windows
venv\Scripts\activate
# Linux / macOS
source venv/bin/activate
```

3. 安装依赖：

```bash
pip install -r requirements.txt
```

4. 确认 `weights/` 目录下包含以下预训练权重文件：
   - `yolov7-lite-s.pt` — 车牌检测模型
   - `plate_rec.pth` — 车牌识别模型

## 使用说明

### 图形界面模式

```bash
python detect_ui.py
```

启动后可在界面中选择图片、视频或摄像头作为输入源，检测结果将实时显示在主窗口中。

### 命令行模式

```bash
# 检测 imgs/ 目录下的所有图片
python detect_rec_plate.py --source imgs/

# 指定视频文件
python detect_rec_plate.py --source video.mp4

# 使用摄像头（索引 0）
python detect_rec_plate.py --source 0

# 使用 GPU 推理
python detect_rec_plate.py --source imgs/ --device 0
```

### 一键启动

Windows 环境下可直接双击 `run.bat` 启动图形界面。

## 命令行参数

| 参数 | 说明 | 默认值 |
|------|------|--------|
| `--detect_model` | 检测模型路径 | `weights/yolov7-lite-s.pt` |
| `--rec_model` | 识别模型路径 | `weights/plate_rec.pth` |
| `--source` | 输入源（图片/视频路径或摄像头索引） | `imgs/` |
| `--img_size` | 推理图像尺寸 | `640` |
| `--device` | 推理设备（`cpu` 或 `0`） | `cpu` |

## 项目结构

```
yolov7_plate_UI_camera/
├── detect_ui.py           # UI 主程序入口
├── detect_rec_plate.py    # 检测 + 识别核心逻辑
├── detect.py              # 命令行检测脚本
├── yolov7_LPR.py          # UI 布局定义
├── yolov7_LPR.ui          # Qt Designer 界面文件
├── train.py               # 模型训练脚本
├── test.py                # 模型测试脚本
├── requirements.txt       # Python 依赖清单
├── run.bat                # Windows 一键启动脚本
├── models/                # YOLOv7 网络结构定义
├── utils/                 # 工具函数（NMS、数据增强等）
├── plate_recognition/     # 车牌识别模块（LPRNet / CRNN）
├── weights/               # 预训练权重文件
├── fonts/                 # 车牌渲染字体
├── imgs/                  # 测试图片
├── result/                # 检测结果输出目录
└── data/                  # 数据集配置文件
```

## 致谢

- [YOLOv7](https://github.com/WongKinYiu/yolov7) — 目标检测框架
- [LPRNet](https://github.com/sirius-ai/LPRNet_Pytorch) — 车牌识别网络
- PyQt5 — 图形界面框架
