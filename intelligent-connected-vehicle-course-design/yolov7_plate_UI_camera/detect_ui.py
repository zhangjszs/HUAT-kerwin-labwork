import cv2
from PyQt5.QtWidgets import *
from PyQt5 import QtGui
from PyQt5.QtCore import *
from yolov7_LPR import Ui_MainWindow
import sys,os,time
from detect_rec_plate import *
from PyQt5.QtGui import QImage


class myMainWindow(Ui_MainWindow,QMainWindow):
    def __init__(self):
        super(myMainWindow, self).__init__()
        self.setupUi(self)
        self.timer_camera = QTimer()
        self.PB1.clicked.connect(self.OpenFile)  #绑定按键1：打开文件
        self.PB2.clicked.connect(self.detect)      #绑定按键2：车牌检测
        self.PB3.clicked.connect(self.close)      #绑定按键3;退出键
        self.PB4.clicked.connect(self.camera)     #打开/关闭摄像头
        self.PB5.clicked.connect(self.save_screenshot)
        self.camera_open = True

        parser = argparse.ArgumentParser()
        parser.add_argument('--detect_model', nargs='+', type=str, default='weights/yolov7-lite-s.pt',
                            help='model.pt path(s)')
        parser.add_argument('--rec_model', type=str, default='weights/plate_rec.pth', help='model.pt path(s)')
        parser.add_argument('--source', type=str, default='imgs', help='source')  # file/folder, 0 for webcam
        parser.add_argument('--img_size', type=int, default=640, help='inference size (pixels)')
        parser.add_argument('--output', type=str, default='result', help='source')
        parser.add_argument('--kpt-label', type=int, default=4, help='number of keypoints')
        self.device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        self.opt = parser.parse_args()
        print(self.opt)
        self.model = attempt_load(self.opt.detect_model, map_location=self.device)  # 加载模型
        self.plate_rec_model = init_model(self.device, self.opt.rec_model)  # 初始化模型
        self.statusBar().showMessage("模型加载完成，就绪")

    def OpenFile(self): #打开文件
        self.FileName, self.FileType = QFileDialog.getOpenFileName(self, "打开文件", "./imgs/",
                                                                   "All Files(*);;*.jpg;;*.png;;*.mp4;;*.avi")
        file = os.path.splitext(self.FileName)  # 将文件路径分离文件名与扩展名
        file_name, file_type = file  # 获取文件名称和文件类型
        if file_type in ".jpg .png":
            jpg = QtGui.QPixmap(self.FileName).scaled(self.label_2.width(), self.label_2.height())
            self.label_2.setPixmap(jpg)
        if file_type in ".mp4 .avi":
            self.cap = cv2.VideoCapture(self.FileName)
            ret, image = self.cap.read()
            if ret:
                image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
                showImage = QImage(image.data, image.shape[1], image.shape[0],
                                   QImage.Format_RGB888)
                video_result = QtGui.QPixmap(showImage).scaled(self.label_2.width(), self.label_2.height())
                self.label_2.setPixmap(video_result)
            else:
                self.cap.release()
        self.statusBar().showMessage(f"已加载: {os.path.basename(self.FileName)}")

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

    def closeEvent(self, event): #重写关闭事件函数
        result = QMessageBox.question(self, "提示", "是否退出?",
                                                QMessageBox.Yes | QMessageBox.No,
                                      QMessageBox.No)
        if result == QMessageBox.Yes:
            event.accept()
            sys.exit(0)
        else:
            event.ignore()

    def detect(self): #车牌检测函数
        try:
            self.FileName #如果在界面中没有点击打开文件，直接点击车牌检测，会造成 self.FileName 未被定义，故在这里设置异常检查
        except AttributeError: #如果出现异常，会弹出警告对话框，并不触发该函数
            QMessageBox.about(self, "警告", "未选择文件！")
        else:
            '''后面为检测核心代码'''
            file = os.path.splitext(self.FileName) #将文件路径分离文件名与扩展名
            file_name, file_type = file  # 获取文件名称和文件类型
            '''这里对图片和视频类文件分开进行处理'''
            if file_type in ".jpg .png": # 如果获取的文件为图片类型
                self.statusBar().showMessage("检测中...")
                QApplication.processEvents()
                time_start = time.time() # 记录检测开始时间
                img = cv_imread(self.FileName) # cv2读取图片
                if img.shape[-1] == 4:
                    img = cv2.cvtColor(img, cv2.COLOR_BGRA2BGR) # 将图片格式由BGRA格式转换为BGR
                dict_list = detect_Recognition_plate(self.model, img, self.device, self.plate_rec_model, self.opt.img_size) #调用车牌检测函数
                ori_img, result_str = draw_result(img, dict_list) #将检测结果在原图上显示，画框

                time_end = time.time() # 记录结束时间
                self.label_6.setText('%.2f' % (time_end - time_start) + 'S')  # 将检测时间显示到GUI界面中，对结果保留两位小数

                '''保存检测结果'''
                img_name = os.path.basename(self.FileName)
                save_img_path = os.path.join(self.opt.output, img_name)
                cv2.imwrite(save_img_path, ori_img)
                '''检测结果在GUI界面显示'''
                jpg_result = QtGui.QPixmap(save_img_path).scaled(self.label_2.width(), self.label_2.height()) #将结果显示在窗口中
                self.label_2.setPixmap(jpg_result)
                self.statusBar().showMessage(f"检测完成，耗时 {time_end - time_start:.2f}s，共识别 {len(dict_list)} 个车牌")
                if len(result_str) != 0:
                    self.textBrowser.append(f"检测结果为：{result_str}")

            if file_type in ".mp4 .avi": # 对文件夹中的.mp4和.avi格式的视频进行检测
                self.video = cv2.VideoCapture(self.FileName)  # 读取视频文件
                width = int(self.video.get(cv2.CAP_PROP_FRAME_WIDTH))  # 获取视频的宽度
                height = int(self.video.get(cv2.CAP_PROP_FRAME_HEIGHT))  # 获取视频的高度
                fps = int(self.video.get(cv2.CAP_PROP_FPS)) # 获取视频的帧率

                img_name = os.path.basename(self.FileName)
                save_img_path = os.path.join(self.opt.output, img_name)
                videoWriter = cv2.VideoWriter(save_img_path, cv2.VideoWriter_fourcc('m', 'p', '4', 'v'), fps, (width,height))

                while self.video.isOpened():
                    time_start = time.time()  # 记录开始时间
                    ret, frame = self.video.read() # 频按帧读取
                    if frame is None: # 对异常帧进行处理
                        break
                    if ret == True:
                        dict_list = detect_Recognition_plate(self.model, frame, self.device, self.plate_rec_model, self.opt.img_size)
                        video_result, result_str = draw_result(frame, dict_list)
                        # cv2.imshow('video_result', video_result)
                        time_end = time.time() # 记录结束时间

                        videoWriter.write(video_result)  # cv2保存视频检测结果
                        video_result = cv2.cvtColor(video_result, cv2.COLOR_BGR2RGB) #转换图片格式
                        showImage = QImage(video_result.data, video_result.shape[1],video_result.shape[0], QImage.Format_RGB888)
                        video_result = QtGui.QPixmap(showImage).scaled(self.label_2.width(), self.label_2.height())
                        self.label_2.setPixmap(video_result)
                        if len(result_str) !=0: #用字符串长度判断检测结果是否为空
                            self.textBrowser.append(f"检测结果为：{result_str}") #将检测结果写在界面上
                        self.label_6.setText('%.2f' % (time_end - time_start) + 'S')  # 将检测时间显示到GUI界面中，对结果保留两位小数
                        key = cv2.waitKey(1000 // fps)
                        if key == ord('q'):
                            break
                self.video.release()
                # cv2.destroyAllWindows()

    def camera(self): #打开/关闭摄像头检测
        if self.camera_open:
            self.camera_open = False
            self.video_1 = cv2.VideoCapture(0)  # 打开摄像头
            self.statusBar().showMessage("摄像头已开启，实时检测中...")
            width = int(self.video_1.get(cv2.CAP_PROP_FRAME_WIDTH))  # 获取视频的宽度
            height = int(self.video_1.get(cv2.CAP_PROP_FRAME_HEIGHT))  # 获取视频的高度
            fps = int(self.video_1.get(cv2.CAP_PROP_FPS))  # 获取视频的帧率
            save_img_path = os.path.join(self.opt.output, 'camera.mp4')
            videoWriter = cv2.VideoWriter(save_img_path, cv2.VideoWriter_fourcc('m', 'p', '4', 'v'), fps, (width, height))

            while self.video_1.isOpened():
                time_start = time.time()  # 记录开始时间
                ret, frame = self.video_1.read()  # 频按帧读取
                if frame is None:  # 对异常帧进行处理
                    break
                if ret == True:
                    dict_list = detect_Recognition_plate(self.model, frame, self.device, self.plate_rec_model, self.opt.img_size)
                    video_result, result_str = draw_result(frame, dict_list)
                    # cv2.imshow('video_result', video_result)
                    time_end = time.time()  # 记录结束时间

                    videoWriter.write(video_result)  # cv2保存视频检测结果
                    video_result = cv2.cvtColor(video_result, cv2.COLOR_BGR2RGB)  # 转换图片格式
                    showImage = QImage(video_result.data, video_result.shape[1], video_result.shape[0],
                                       QImage.Format_RGB888)
                    video_result = QtGui.QPixmap(showImage).scaled(self.label_2.width(), self.label_2.height())
                    self.label_2.setPixmap(video_result)
                    if len(result_str) != 0:
                        self.textBrowser.append(f"检测结果为：{result_str}")  # 将检测结果写在界面上
                    self.label_6.setText('%.2f' % (time_end - time_start) + 'S')  # 将检测时间显示到GUI界面中，对结果保留两位小数
                    cv2.waitKey(1000 // fps)

        else:
            self.camera_open = True
            if self.video_1.isOpened():
                self.video_1.release()
            if self.timer_camera.isActive():
                self.timer_camera.stop()
            self.label_2.clear()
            self.statusBar().showMessage("摄像头已关闭")
        # cv2.destroyAllWindows()



if __name__ == '__main__':
    app = QApplication(sys.argv)
    vieo_gui = myMainWindow()
    vieo_gui.show()
    sys.exit(app.exec_())