# UI 美化与功能增强 Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** 对 yolov7_plate_UI_camera 的 PyQt5 界面进行深色科技风美化，并新增置信度显示、截图保存、状态栏提示、识别历史记录四项功能。

**Architecture:** 所有 UI 改动集中在 `yolov7_LPR.py`（布局/样式）和 `detect_ui.py`（逻辑）两个文件。`draw_result` 返回值需扩展以携带置信度信息。不引入新的第三方依赖。

**Tech Stack:** PyQt5, OpenCV, Python 3.8+

---

### Task 1: 深色科技风全局样式

**Files:**
- Modify: `yolov7_LPR.py`

**Step 1: 替换窗口标题和全局背景色**

在 `setupUi` 开头，`MainWindow.resize(942, 589)` 之后插入：

```python
MainWindow.setWindowTitle("YOLOv7 车牌识别系统")
MainWindow.setStyleSheet("""
    QMainWindow, QWidget {
        background-color: #1a1a2e;
        color: #e0e0e0;
        font-family: 'Microsoft YaHei', sans-serif;
    }
    QPushButton {
        background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
            stop:0 #0f3460, stop:1 #16213e);
        color: #00d4ff;
        border: 1px solid #00d4ff;
        border-radius: 6px;
        font-size: 14px;
        padding: 6px 12px;
    }
    QPushButton:hover {
        background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
            stop:0 #00d4ff, stop:1 #0f3460);
        color: #1a1a2e;
    }
    QPushButton:pressed {
        background-color: #00a8cc;
    }
    QTextBrowser {
        background-color: #16213e;
        color: #00d4ff;
        border: 1px solid #0f3460;
        border-radius: 4px;
        font-size: 13px;
    }
    QLabel {
        color: #e0e0e0;
    }
    QStatusBar {
        background-color: #0f3460;
        color: #00d4ff;
        font-size: 12px;
    }
""")
```

**Step 2: 更新 label_2（视频显示区）样式**

将原有 `self.label_2.setStyleSheet(...)` 替换为：

```python
self.label_2.setStyleSheet("""
    QLabel {
        border: 2px solid #00d4ff;
        border-radius: 8px;
        background-color: #0d0d1a;
        color: #555;
        font-size: 16px;
    }
""")
```

**Step 3: 更新 label_6（车牌结果显示）样式**

将原有 `self.label_6.setStyleSheet(...)` 替换为：

```python
self.label_6.setStyleSheet("""
    QLabel {
        border: 2px solid #00d4ff;
        border-radius: 8px;
        background-color: #0f3460;
        color: #00d4ff;
        font-size: 28px;
        font-weight: bold;
        letter-spacing: 4px;
    }
""")
```

**Step 4: 更新所有标题 label 颜色**

在 `retranslateUi` 之前，对 label_1/3/4/5 统一设置样式：

```python
for lbl in [self.label_1, self.label_3, self.label_4, self.label_5]:
    lbl.setStyleSheet("color: #00d4ff; font-weight: bold;")
```

**Step 5: 调整窗口尺寸为 1000x620，给状态栏留空间**

```python
MainWindow.resize(1000, 620)
```

**Step 6: 手动运行验证**

```bash
python detect_ui.py
```
预期：界面呈深色背景，按钮蓝色边框，标题蓝色高亮。

**Step 7: Commit**

```bash
git add yolov7_LPR.py
git commit -m "feat: apply dark tech UI theme to plate recognition system"
```

---

### Task 2: 新增置信度显示

**Files:**
- Modify: `detect_rec_plate.py` — `draw_result` 函数
- Modify: `detect_ui.py` — detect/camera 方法

**Step 1: 确认 draw_result 当前签名**

读取 `detect_rec_plate.py` 中 `draw_result` 函数，确认 `dict_list` 中每个 dict 的字段（应含 `rect`、`plate_no`、`score` 等）。

**Step 2: 在 draw_result 绘制置信度文字**

找到绘制车牌号的代码段，在车牌框上方额外绘制置信度：

```python
# 在已有的 plate_no 绘制之后追加
if 'score' in result:
    conf_text = f"{result['score']:.2f}"
    # 在框右上角绘制小字置信度
    cv2.putText(orgimg, conf_text,
                (int(result['rect'][2]) - 50, int(result['rect'][1]) - 5),
                cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 255), 1)
```

**Step 3: 手动运行验证**

```bash
python detect_ui.py
```
打开一张测试图片并检测，预期：车牌框旁显示置信度数值（如 0.92）。

**Step 4: Commit**

```bash
git add detect_rec_plate.py
git commit -m "feat: display confidence score on detection result"
```

---

### Task 3: 截图保存功能

**Files:**
- Modify: `detect_ui.py`
- Modify: `yolov7_LPR.py`

**Step 1: 在 yolov7_LPR.py 添加截图按钮 PB5**

在 PB3（退出）定义之后添加：

```python
self.PB5 = QtWidgets.QPushButton(self.centralwidget)
self.PB5.setGeometry(QtCore.QRect(60, 545, 161, 40))
font = QtGui.QFont()
font.setPointSize(13)
self.PB5.setFont(font)
self.PB5.setObjectName("PB5")
```

在 `retranslateUi` 中添加：

```python
self.PB5.setText(_translate("MainWindow", "截图保存"))
```

**Step 2: 在 detect_ui.py 绑定截图逻辑**

在 `__init__` 的按钮绑定区域添加：

```python
self.PB5.clicked.connect(self.save_screenshot)
```

添加方法：

```python
def save_screenshot(self):
    pixmap = self.label_2.pixmap()
    if pixmap is None or pixmap.isNull():
        self.statusBar().showMessage("当前无图像可保存", 3000)
        return
    os.makedirs("result/screenshots", exist_ok=True)
    timestamp = time.strftime("%Y%m%d_%H%M%S")
    save_path = f"result/screenshots/screenshot_{timestamp}.png"
    pixmap.save(save_path)
    self.statusBar().showMessage(f"截图已保存: {save_path}", 3000)
```

**Step 3: 手动运行验证**

```bash
python detect_ui.py
```
检测一张图片后点击"截图保存"，预期：`result/screenshots/` 下生成 PNG 文件，状态栏显示保存路径。

**Step 4: Commit**

```bash
git add yolov7_LPR.py detect_ui.py
git commit -m "feat: add screenshot save button"
```

---

### Task 4: 状态栏提示

**Files:**
- Modify: `detect_ui.py`

**Step 1: 在关键操作节点插入 statusBar 提示**

在 `__init__` 末尾（模型加载完成后）：

```python
self.statusBar().showMessage("模型加载完成，就绪")
```

在 `OpenFile` 成功后：

```python
self.statusBar().showMessage(f"已加载: {os.path.basename(self.FileName)}")
```

在 `detect` 图片检测开始时：

```python
self.statusBar().showMessage("检测中...")
QApplication.processEvents()
```

在 `detect` 图片检测完成后：

```python
self.statusBar().showMessage(f"检测完成，耗时 {time_end - time_start:.2f}s，共识别 {len(dict_list)} 个车牌")
```

在 `camera` 打开摄像头时：

```python
self.statusBar().showMessage("摄像头已开启，实时检测中...")
```

在 `camera` 关闭摄像头时：

```python
self.statusBar().showMessage("摄像头已关闭")
```

**Step 2: 手动运行验证**

```bash
python detect_ui.py
```
依次执行打开文件、检测、摄像头操作，预期底部状态栏实时更新提示文字。

**Step 3: Commit**

```bash
git add detect_ui.py
git commit -m "feat: add status bar messages for all key operations"
```

---

### Task 5: 识别历史记录（带时间戳 + 清空按钮）

**Files:**
- Modify: `yolov7_LPR.py`
- Modify: `detect_ui.py`

**Step 1: 在 yolov7_LPR.py 添加清空历史按钮 PB6**

在 PB5 定义之后添加：

```python
self.PB6 = QtWidgets.QPushButton(self.centralwidget)
self.PB6.setGeometry(QtCore.QRect(250, 545, 161, 40))
font = QtGui.QFont()
font.setPointSize(13)
self.PB6.setFont(font)
self.PB6.setObjectName("PB6")
```

在 `retranslateUi` 中添加：

```python
self.PB6.setText(_translate("MainWindow", "清空记录"))
```

**Step 2: 在 detect_ui.py 绑定清空逻辑，并在写入历史时加时间戳**

在 `__init__` 绑定区域添加：

```python
self.PB6.clicked.connect(self.clear_history)
```

添加方法：

```python
def clear_history(self):
    self.textBrowser.clear()
    self.statusBar().showMessage("识别记录已清空", 2000)
```

将所有 `self.textBrowser.append(f"检测结果为：{result_str}")` 替换为：

```python
timestamp = time.strftime("%H:%M:%S")
self.textBrowser.append(f"[{timestamp}] {result_str}")
```

**Step 3: 手动运行验证**

```bash
python detect_ui.py
```
检测多张图片，预期：textBrowser 中每条记录前有时间戳；点击"清空记录"后列表清空，状态栏提示。

**Step 4: Commit**

```bash
git add yolov7_LPR.py detect_ui.py
git commit -m "feat: add timestamped history and clear button"
```

---

## 完成后验证

```bash
python detect_ui.py
```

逐项检查：
- [ ] 界面呈深色科技风，按钮蓝色渐变
- [ ] 检测结果图上显示置信度数值
- [ ] 截图保存按钮可用，文件生成在 result/screenshots/
- [ ] 底部状态栏在各操作节点有提示
- [ ] 识别历史带时间戳，清空按钮有效
