'''
该代码为yolov7车牌检测的检测代码
'''
import argparse
import time
import os
import copy
import cv2
import torch
import numpy as np
import torch.backends.cudnn as cudnn
from models.experimental import attempt_load
from utils.general import  non_max_suppression, scale_coords
from plate_recognition.plate_rec import get_plate_result,allFilePath,init_model,cv_imread
from plate_recognition.double_plate_split_merge import get_split_merge
from utils.datasets import letterbox
from utils.cv_puttext import cv2ImgAddText
def cv_imread(path):
    img=cv2.imdecode(np.fromfile(path,dtype=np.uint8),-1)
    return img

clors = [(255,0,0),(0,255,0),(0,0,255),(255,255,0),(0,255,255)]

def order_points(pts):  #关键点按照（左上，右上，右下，左下）排列
    rect = np.zeros((4, 2), dtype = "float32")
    s = pts.sum(axis = 1)
    rect[0] = pts[np.argmin(s)]
    rect[2] = pts[np.argmax(s)]
    diff = np.diff(pts, axis = 1)
    rect[1] = pts[np.argmin(diff)]
    rect[3] = pts[np.argmax(diff)]
    return rect


def four_point_transform(image, pts):  #透视变换
    rect = order_points(pts)
    (tl, tr, br, bl) = rect
    widthA = np.sqrt(((br[0] - bl[0]) ** 2) + ((br[1] - bl[1]) ** 2))
    widthB = np.sqrt(((tr[0] - tl[0]) ** 2) + ((tr[1] - tl[1]) ** 2))
    maxWidth = max(int(widthA), int(widthB))
    heightA = np.sqrt(((tr[0] - br[0]) ** 2) + ((tr[1] - br[1]) ** 2))
    heightB = np.sqrt(((tl[0] - bl[0]) ** 2) + ((tl[1] - bl[1]) ** 2))
    maxHeight = max(int(heightA), int(heightB))
    dst = np.array([
        [0, 0],
        [maxWidth - 1, 0],
        [maxWidth - 1, maxHeight - 1],
        [0, maxHeight - 1]], dtype = "float32")
    M = cv2.getPerspectiveTransform(rect, dst)
    warped = cv2.warpPerspective(image, M, (maxWidth, maxHeight))
    return warped

def get_plate_rec_landmark(img, xyxy, conf, landmarks, class_num,device,plate_rec_model):
    h,w,c = img.shape
    result_dict={}
    tl = 1 or round(0.002 * (h + w) / 2) + 1  # line/font thickness

    x1 = int(xyxy[0])
    y1 = int(xyxy[1])
    x2 = int(xyxy[2])
    y2 = int(xyxy[3])
    height=y2-y1
    landmarks_np=np.zeros((4,2))
    rect=[x1,y1,x2,y2]
    for i in range(4):
        point_x = int(landmarks[2 * i])
        point_y = int(landmarks[2 * i + 1])
        landmarks_np[i]=np.array([point_x,point_y])

    class_label= int(class_num)  #车牌的的类型0代表单牌，1代表双层车牌
    roi_img = four_point_transform(img,landmarks_np)   #透视变换得到车牌小图
    # cv2.imwrite("roi.jpg",roi_img)
    # roi_img_h = roi_img.shape[0]
    # roi_img_w = roi_img.shape[1]
    # if roi_img_w/roi_img_h<3:
    #     class_label=
    # h_w_r = roi_img_w/roi_img_h
    if class_label :        #判断是否是双层车牌，是双牌的话进行分割后然后拼接
        roi_img=get_split_merge(roi_img)
    plate_number = get_plate_result(roi_img,device,plate_rec_model) #对车牌小图进行识别
