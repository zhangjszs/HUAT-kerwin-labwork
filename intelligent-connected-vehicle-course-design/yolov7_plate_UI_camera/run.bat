@echo off
chcp 65001 >nul
title YOLOv7 车牌识别系统

echo ================================
echo   YOLOv7 车牌识别系统
echo ================================
echo.

REM 检查 Python 是否可用
python --version >nul 2>&1
if errorlevel 1 (
    echo [错误] 未检测到 Python，请先安装 Python 3.8+
    pause
    exit /b 1
)

REM 检查权重文件
if not exist "weights\yolov7-lite-s.pt" (
    echo [错误] 未找到检测模型 weights\yolov7-lite-s.pt
    pause
    exit /b 1
)
if not exist "weights\plate_rec.pth" (
    echo [错误] 未找到识别模型 weights\plate_rec.pth
    pause
    exit /b 1
)

REM 创建输出目录
if not exist "result" mkdir result
if not exist "result\screenshots" mkdir result\screenshots

echo 正在启动车牌识别系统...
echo.
python detect_ui.py

if errorlevel 1 (
    echo.
    echo [错误] 程序异常退出，请检查依赖是否安装完整
    echo 运行 pip install -r requirements.txt 安装依赖
    pause
)
