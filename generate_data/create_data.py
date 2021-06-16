#coding:utf-8
import cv2
import os
import glob
import sys
import numpy as np
import json
import random
from pascal_voc_io import PascalVocWriter
from pascal_voc_io import XML_EXT
import shutil
from tqdm import tqdm
import math
import multiprocessing
import time
import copy
from pb import *
from PIL import Image

BLENDING_LIST = ['gaussian','poisson', 'none', 'box']
# BLENDING_LIST = ['poisson', 'none']

shape_dir = os.path.join(os.getcwd() , "shapes")

def PIL2array1C(img):
    '''Converts a PIL image to NumPy Array

    Args:
        img(PIL Image): Input PIL image
    Returns:
        NumPy Array: Converted image
    '''
    return np.array(img.getdata(),
                    np.uint8).reshape(img.size[1], img.size[0])

def PIL2array3C(img):
    '''Converts a PIL image to NumPy Array

    Args:
        img(PIL Image): Input PIL image
    Returns:
        NumPy Array: Converted image
    '''
    return np.array(img.getdata(),
                    np.uint8).reshape(img.size[1], img.size[0], 3)

def get_annotation_from_mask(mask):
    '''Given a mask, this returns the bounding box annotations

    Args:
        mask(NumPy Array): Array with the mask
    Returns:
        tuple: Bounding box annotation (xmin, xmax, ymin, ymax)
    '''
    rows = np.any(mask, axis=1)
    cols = np.any(mask, axis=0)
    if len(np.where(rows)[0]) > 0:
        ymin, ymax = np.where(rows)[0][[0, -1]]
        xmin, xmax = np.where(cols)[0][[0, -1]]
        return xmin, xmax, ymin, ymax
    else:
        return -1, -1, -1, -1

def parese_json(fpath):
    with open(fpath, 'rb') as f:
        myjson = json.load(f)
        # imagePath = myjson["imagePath"]
        # print(imagePath)
        # print('shapes:' , myjson["shapes"])
        # print('points:' , myjson["shapes"][0]['points'])
        # print('label:'  , myjson["shapes"][0]['label'])
        return myjson

def update_PolygonPoints(polygon_pts , top_left_pt):
    '''
    根据起始点更新多边形的每个顶点
    author:hanshuo
    :param polygon_pts: [[x1,y1] , [x2 , y2] ,... [xn , yn]]
    :param top_left_pt: [x,y]
    :return:
    '''
    num_poly = len(polygon_pts)
    for i in range(num_poly):
        polygon_pts[i][0] = int(polygon_pts[i][0]) + top_left_pt[0]
        polygon_pts[i][1] = int(polygon_pts[i][1]) + top_left_pt[1]

def isLineCross(pt1_1 , pt1_2 , pt2_1 , pt2_2):
    '''
    判断两条线是否相交
    :param pt1_1:
    :param pt1_2:
    :param pt2_1:
    :param pt2_2:
    :return:True相交
    '''
    ret = min(pt1_1[0] , pt1_2[0]) <= max(pt2_1[0] , pt2_2[0]) and min(pt2_1[0] , pt2_2[0]) <= max(pt1_1[0] , pt1_2[0]) and min(pt1_1[1] , pt1_2[1]) <= max(pt2_1[1] , pt2_2[1]) and min(pt2_1[1] , pt2_2[1]) <= max(pt1_1[1] , pt1_2[1])
    return ret

def isCross(p1 , p2 , p3):
    '''
    跨立实验
    :param p1:
    :param p2:
    :param p3:
    :return:
    '''
    x1 = p2[0] - p1[0]
    y1 = p2[1] - p1[1]
    x2 = p3[0] - p1[0]
    y2 = p3[1] - p1[1]
    return x1 * y2 - x2 * y1

def IsIntersec(p1,p2,p3,p4):
    '''
    判断两条直线是否相交
    :param p1:
    :param p2:
    :param p3:
    :param p4:
    :return:
    '''
    flag = False
    if isLineCross(p1 , p2 , p3 , p4):
        if(isCross(p1,p2,p3) * isCross(p1,p2,p4)<=0 and isCross(p3,p4,p1) * isCross(p3,p4,p2) <= 0):
            flag = True
        else:
            flag = False
    else:
        flag = False
    return flag

def getCrossPoint(p1 , p2 , q1 , q2):
    '''
    计算两条直线的交点
    :param pt1:
    :param pt2:
    :param pt3:
    :param pt4:
    :return:
    '''
    jiaodian = []
    if IsIntersec(p1 , p2 , q1 , q2):
        # 求交点
        left = (q2[0] - q1[0]) * (p1[1] - p2[1]) - (p2[0] - p1[0]) * (q1[1] - q2[1])
        right = (p1[1] - q1[1]) * (p2[0] - p1[0]) * (q2[0] - q1[0]) + q1[0] * (q2[1] - q1[1]) * (p2[0] - p1[0]) - p1[0] * (p2[1] - p1[1]) * (q2[0] - q1[0])

        if left == 0:
            return jiaodian
        x = int(float(right)/float(left))
        left = (p1[0] - p2[0]) * (q2[1] - q1[1]) - (p2[1] - p1[1]) * (q1[0] - q2[0])
        right = p2[1] * (p1[0] - p2[0]) * (q2[1] - q1[1]) + (q2[0] - p2[0]) * (q2[1] - q1[1]) * (p1[1] - p2[1]) - q2[1] * (q1[0] - q2[0]) * (p2[1] - p1[1])
        if left == 0:
            return jiaodian
        y = int(float(right)/float(left))

        jiaodian.append(x)
        jiaodian.append(y)


    return jiaodian

def isInPolygon_2(polygon_pts , pt):
    '''

    :param polygon_pts:
    :param pt:
    :return: 点在多边形内则返回True , 否则返回False
    '''
    num_pts = len(polygon_pts)
    if num_pts < 2:
        return False
    nCross = 0
    for i in range(num_pts):
        pt_A = polygon_pts[i]
        pt_B = polygon_pts[(i + 1) % num_pts]

        if pt_A[1] == pt_B[1]:
            continue
        if pt[1] < min(pt_A[1] , pt_B[1]):
            continue
        if pt[1] >= max(pt_A[1] , pt_B[1]):
            continue
        # 求交点x坐标
        x = float(pt[1] - pt_A[1]) * float(pt_B[0] - pt_A[0])/float(pt_B[1] - pt_A[1]) + pt_A[0]
        # 只统计单边交点
        if x > pt[0]:
            nCross += 1
    # 单边交点为偶数，则点在多边形外
    return nCross %2 == 1

def PointCmp(a , b , center):
    '''
    若点a大于点b，即点a在点b顺时针方向，返回True，否则返回False
    :param a:
    :param b:
    :param center:
    :return:
    '''
    if a[0] >= 0 and b[0] < 0:
        return True
    if a[0] == 0 and b[0] == 0:
        return a[1] > b[1]
    # 向量OA和向量OB的叉积
    det = (a[0] - center[0]) * (b[1] - center[1]) - (b[0] - center[0]) * (a[1] - center[1])
    if det < 0:
        return True
    if det > 0:
        return False

    # 共线，以A、B距离center的距离判断大小
    d1 = (a[0] - center[0]) * (a[0] - center[0]) + (a[1] - center[1]) * (a[1] - center[1])
    d2 = (b[0] - center[0]) * (b[0] - center[0]) + (b[1] - center[1]) * (b[1] - center[1])

    return d1 > d2

def ClockwiseSortpoints(poly_pts):
    '''
    点集排序
    :param poly_pts:
    :return:
    '''
    # 计算重心
    center = []
    x = 0
    y = 0
    num = len(poly_pts)
    for i in range(num):
        x += poly_pts[i][0]
        y += poly_pts[i][1]

    center.append(int(x/num))
    center.append(int(y/num))

    # 排序
    for i in range(num - 1):
        for j in range(num - i - 1):
            if PointCmp(poly_pts[j] , poly_pts[j + 1] , center):
                tmp = poly_pts[j]
                poly_pts[j] = poly_pts[j + 1]
                poly_pts[j + 1] = tmp


def getPolygonCrossPoint(poly_pts1 , poly_pts2):
    '''
    计算两个多边形的交点
    :param poly_pts1:
    :param poly_pts2:
    :return:
    '''
    jiaodian = set()
    num1 = len(poly_pts1)
    num2 = len(poly_pts2)
    if num1 <3 or num2 < 3:
        return list(jiaodian)
    for i in range(num1):
        poly_next_idx = (i + 1) % num1
        for j in range(num2):
            poly2_next_idx = (j + 1) % num2
            pt = getCrossPoint(poly_pts1[i] , poly_pts1[poly_next_idx] , poly_pts2[j] , poly_pts2[poly2_next_idx])

            if pt:
                jiaodian.add(tuple(pt))
    jiaodian = list(jiaodian)

    # 如果没有交点
    if len(jiaodian) == 0:
        return jiaodian
    else:
        for j in jiaodian:
            j = list(j)
    return jiaodian

def updatePolygon(polygon_pts1 , polygon_pts2 , cross_pts):
    '''
    根据两个多边形相交，重新计算其重叠后的多边形
    :param polygon_pts1:
    :param polygon_pts2:
    :return:
    '''
    # random_seed = random.randint(0 , 1)
    # # 如果random_seed等于0 则poly1覆盖poly2，如果random_seed等于1，则poly2覆盖poly1

    new_poly1 = []
    new_poly2 = []
    new_poly2 = polygon_pts2
    for i in cross_pts:
        new_poly1.append(i)
    for j in polygon_pts1:
        if isInPolygon_2(polygon_pts2 , j):
            continue
        new_poly1.append(j)
    ClockwiseSortpoints(new_poly1)

    return new_poly1 , new_poly2


def calcIOU(x1, y1, w1, h1, x2, y2, w2, h2):
    '''

    :param x1:center_x
    :param y1:center_y
    :param w1:
    :param h1:
    :param x2:
    :param y2:
    :param w2:
    :param h2:
    :return:
    '''
    IOU = 0
    if ((abs(x1 - x2) < ((w1 + w2) / 2.0)) and (abs(y1 - y2) < ((h1 + h2) / 2.0))):
        left = max((x1 - (w1 / 2.0)), (x2 - (w2 / 2.0)))
        upper = max((y1 - (h1 / 2.0)), (y2 - (h2 / 2.0)))

        right = min((x1 + (w1 / 2.0)), (x2 + (w2 / 2.0)))
        bottom = min((y1 + (h1 / 2.0)), (y2 + (h2 / 2.0)))
        inter_w = abs(left - right)
        inter_h = abs(upper - bottom)
        inter_square = inter_w * inter_h
        union_square = (w1 * h1) + (w2 * h2) - inter_square
        IOU = inter_square / union_square * 1.0

    return IOU


def maxIOU_2(x1 , y1 , w1 , h1 , x2 , y2 , w2 , h2):
    W = min(x1 + w1 , x2 + w2) - max(x1 , x2)
    H = min(y1 + h1 , y2 + h2) - max(y1 , y2)

    if W <= 0 or H <= 0:
        return 0
    SA = w1 * h1
    SB = w2 * h2
    cross_area = H * W

    return max(float(cross_area)/float(SA) , float(cross_area)/float(SB))

def rotate_image(image , angle):
    '''
    图像旋转
    :param image:
    :param angle:
    :return: 返回旋转后的图像以及旋转矩阵
    '''
    (h , w) = image.shape[:2]
    (cx , cy) = (int(0.5 * w) , int(0.5 * h))
    M = cv2.getRotationMatrix2D((cx , cy) , -angle , 1.0)
    
    cos = np.abs(M[0 , 0])
    sin = np.abs(M[0 , 1])

    # 计算新图像的bounding
    nW = int((h * sin) + (w * cos))
    nH = int((h * cos) + (w * sin))

    M[0 , 2] += int(0.5 * nW) - cx
    M[1 , 2] += int(0.5 * nH) - cy
    return cv2.warpAffine(image , M , (nW , nH)) , M

def warfPoints(poly_pts , matrix_):
    '''
    对多边形点进行仿射变换
    :param poly_pts:
    :param matrix_:
    :return:
    '''
    rotate_pts = []
    for i in range(len(poly_pts)):
        tmp = np.append(poly_pts[i] , [1]).reshape((3 , 1))
        x_y = np.matmul(matrix_ , tmp)
        rotate_pts.append([x_y[0][0] , x_y[1][0]])
    return rotate_pts

def imageZoom(img ,poly_pts ,  flag):
    '''
    缩放操作
    :param img:
    :param poly_pts:
    :param flag:
    :return:
    '''
    img_h , img_w = img.shape[:2]
    new_h = 0
    new_w = 0

    if flag == 1:
        # new_h = int(1.1 * img_h)
        # new_w = int(1.1 * img_w)
        
        random_val = random.randint(1 , 3)
        new_h = int(float(random_val * 0.1 + 1) * img_h)
        new_w = int(float(random_val * 0.1 + 1) * img_w)
    else:
        new_h = int(img_h/1.1)
        new_w = int(img_w/1.1)

    img2 = cv2.resize(img , (new_w , new_h))
    new_poly_pts = []
    for i in poly_pts:
        new_x = int(new_w * i[0] / float(img_w))
        new_y = int(new_h * i[1] / float(img_h))
        new_poly_pts.append([new_x, new_y])
    return  img2 , new_poly_pts

def addPepperNoise(src):
    '''
    添加椒盐噪声
    :param src:
    :return:
    '''
    img_h , img_w = src.shape[:2]
    new_img = src.copy()
    NoiseNum = int(0.001 * img_h * img_w)
    for i in range(NoiseNum):
        randX = np.random.randint(0 , img_w - 1)
        randY = np.random.randint(0 , img_h - 1)
        if np.random.randint(0 , 2) == 0:
            new_img[randY , randX] = [0 , 0 , 0]
        else:
            new_img[randY, randX] = [255, 255, 255]

    return new_img

def getLineLength(p1 , p2):
    l = math.pow((p1[0] - p2[0]) , 2) + math.pow((p1[1] - p2[1]) , 2)
    l = math.sqrt(l)

    return l

def getAreaOfTraingle(p1 , p2 , p3):
    '''
    海伦公式，计算三角形面积
    :param p1:
    :param p2:
    :param p3:
    :return:
    '''
    area = 0
    p1p2 = getLineLength(p1 , p2)
    p2p3 = getLineLength(p2 , p3)
    p3p1 = getLineLength(p3 , p1)

    s = (p1p2 + p2p3 + p3p1) * 0.5

    # 海伦公式
    area = s * (s - p1p2) * (s - p2p3) * (s - p3p1)
    area = math.sqrt(math.fabs(area))
    return area



def getAreaPoly(points):
    area = 0
    num = len(points)
    if num < 3:
        raise Exception('error')

    p1 = points[0]
    for i in range(1 , num - 1):
        p2 = points[i]
        p3 = points[i + 1]

        vec_p1p2 = (p2[0] - p1[0] , p2[1] - p1[1])
        vec_p2p3 = (p3[0] - p2[0] , p3[1] - p2[1])

        sign = 0

        # 判断正负
        vecMult = vec_p1p2[0] * vec_p2p3[1] - vec_p1p2[1] * vec_p2p3[0]
        if vecMult > 0:
            sign = 1
        elif vecMult < 0:
            sign = -1
        triArea = getAreaOfTraingle(p1 , p2 , p3) * sign

        area += triArea
    return abs(area)

            
# 10-15
def process_sub_4(background_lst, dir_lsts):
    global imgFolderName
    rotate_angles = np.arange(0 , 180)
    np.random.seed(0)

    global img_infos
    global foreground_lsts
    num_foreground = len(foreground_lsts)

    for i in tqdm(range(len(background_lst))):
        img = cv2.imread(background_lst[i])
        img = cv2.cvtColor(img , cv2.COLOR_BGR2RGB)
        img_h , img_w = img.shape[:2]

        dir_lsts_tmp = copy.deepcopy(dir_lsts)
        np.random.shuffle(dir_lsts)
        while len(dir_lsts_tmp) > 0:
            result = img.copy()
            mask_objects = []
            try:
                for j in range(2):
                    mask_objects.append(os.path.join(dir_lsts_tmp.pop() , "label.png"))
            except:
                break

            mask_img_1 = cv2.imread(mask_objects[0] , 0)
            if type(mask_img_1) == type(None):
                print '读取roi图片失败'
                print(mask_objects[0])
                break

            foreground_img_1 = cv2.imread(mask_objects[0][:-9] + "img.png")
            foreground_img_1 = cv2.cvtColor(foreground_img_1 , cv2.COLOR_BGR2RGB)
            _ , mask_img_1 = cv2.threshold(mask_img_1 , 0 ,255 , cv2.THRESH_BINARY)
            xmin_1, xmax_1, ymin_1, ymax_1 = get_annotation_from_mask(mask_img_1)

            foreground_img_1 = foreground_img_1[ymin_1:ymax_1 , xmin_1:xmax_1]
            mask_img_1 = mask_img_1[ymin_1:ymax_1 , xmin_1:xmax_1]
            mask_h_1 , mask_w_1 = mask_img_1.shape[:2]

            # 缩放
            if np.random.randint(0 , 2):
                o_h_1 = mask_h_1
                o_w_1 = mask_w_1

                while True:
                    scale = random.uniform(0.7 , 0.9)
                    # 缩放后的尺寸
                    o_h_1 , o_w_1 = int(scale * mask_h_1) , int(scale * mask_w_1)
                    if img_h - o_h_1 > 0 and img_w - o_w_1 > 0 and o_h_1 > 0 and o_w_1 > 0:
                        break
                foreground_img_1 = cv2.resize(foreground_img_1 , dsize=(o_w_1 , o_h_1) , interpolation=cv2.INTER_CUBIC)
                mask_img_1 = cv2.resize(mask_img_1 , dsize=(o_w_1 , o_h_1) , interpolation=cv2.INTER_CUBIC)

            # 旋转
            while True:
                rotate_angles_1 = random.randint(-180, 180)
                foreground_img_1_tmp = Image.fromarray(np.uint8(foreground_img_1)).rotate(rotate_angles_1 , expand=True)
                mask_img_1_tmp = Image.fromarray(np.uint8(mask_img_1)).rotate(rotate_angles_1 , expand=True)
                o_w_1 , o_h_1 = foreground_img_1_tmp.size
                if img_h - o_h_1 > 0 and img_w - o_w_1 > 0:
                    break

            xmin_1, xmax_1, ymin_1, ymax_1 = get_annotation_from_mask(mask_img_1_tmp)
            mask_img_1 = PIL2array1C(mask_img_1_tmp)
            foreground_img_1 = PIL2array3C(foreground_img_1_tmp)

            label_1 = mask_objects[0].split('/')[-2].split('-')[0]

            # ---------------------------------------------

            mask_img_2 = cv2.imread(mask_objects[1] , 0)
            if type(mask_img_2) == type(None):
                print '读取roi图片失败'
                break

            foreground_img_2 = cv2.imread(mask_objects[1][:-9] + "img.png")
            foreground_img_2 = cv2.cvtColor(foreground_img_2 , cv2.COLOR_BGR2RGB)
            _, mask_img_2 = cv2.threshold(mask_img_2, 0, 255, cv2.THRESH_BINARY)
            xmin_2, xmax_2, ymin_2, ymax_2 = get_annotation_from_mask(mask_img_2)

            foreground_img_2 = foreground_img_2[ymin_2:ymax_2, xmin_2:xmax_2]
            mask_img_2 = mask_img_2[ymin_2:ymax_2, xmin_2:xmax_2]
            mask_h_2, mask_w_2 = mask_img_2.shape[:2]

            # 缩放
            if np.random.randint(0, 2):
                o_h_2, o_w_2 = mask_h_2, mask_w_2
                while True:
                    scale = random.uniform(0.7, 0.9)
                    # 缩放后的尺寸
                    o_h_2, o_w_2 = int(scale * mask_h_2), int(scale * mask_w_2)
                    if img_h - o_h_2 > 0 and img_w - o_w_2 > 0 and o_h_2 > 0 and o_w_2 > 0:
                        break

                foreground_img_2 = cv2.resize(foreground_img_2, dsize=(o_w_2, o_h_2) , interpolation=cv2.INTER_CUBIC)
                mask_img_2 = cv2.resize(mask_img_2, dsize=(o_w_2, o_h_2) , interpolation=cv2.INTER_CUBIC)

            # 旋转
            while True:
                rotate_angles_2 = random.randint(-180, 180)
                foreground_img_2_tmp = Image.fromarray(np.uint8(foreground_img_2)).rotate(rotate_angles_2, expand=True)
                mask_img_2_tmp = Image.fromarray(np.uint8(mask_img_2)).rotate(rotate_angles_2, expand=True)
                o_w_2, o_h_2 = foreground_img_2_tmp.size
                if img_h - o_h_2 > 0 and img_w - o_w_2 > 0:
                    break

            xmin_2, xmax_2, ymin_2, ymax_2 = get_annotation_from_mask(mask_img_2_tmp)

            mask_img_2 = PIL2array1C(mask_img_2_tmp)
            foreground_img_2 = PIL2array3C(foreground_img_2_tmp)
            label_2 = mask_objects[1].split('/')[-2].split('-')[0]

            x1 = random.randint(int(-0.25 * o_w_1) , int(img_w - o_w_1 + 0.25 * o_w_1))
            y1 = random.randint(int(-0.25 * o_h_1) , int(img_h - o_h_1 + 0.25 * o_h_1))

            # x2 = random.randint(int(-0.5 * o_w_2), int(img_w - o_w_2 + 0.5 * o_w_2))
            # y2 = random.randint(int(-0.5 * o_h_2), int(img_h - o_h_2 + 0.5 * o_h_2))
            x2 = random.randint(int(x1 - o_w_2) , int(x1 + o_w_1))
            y2 = random.randint(int(y1 - o_h_2) , int(y1 + o_h_1))

            # print('x2:' , x2)
            # print('y2:' , y2)


            xmin_1 += x1
            xmax_1 += x1
            ymin_1 += y1
            ymax_1 += y1

            xmin_2 += x2
            xmax_2 += x2
            ymin_2 += y2
            ymax_2 += y2

            xmin_1 = max(0 , xmin_1)
            xmax_1 = min(xmax_1 , img_w)
            ymin_1 = max(0 , ymin_1)
            ymax_1 = min(ymax_1 , img_h)

            xmin_2 = max(0, xmin_2)
            xmax_2 = min(xmax_2, img_w)
            ymin_2 = max(0, ymin_2)
            ymax_2 = min(ymax_2, img_h)

            if xmin_1 - xmax_1 >= 0 or ymin_1 - ymax_1 >= 0 or xmin_2 - xmax_2 > 0 or ymin_2 - ymax_2 > 0:
                continue

            if (xmax_2 - xmin_2) < int(0.5 * o_w_2) or (ymax_2 - ymin_2) < int(0.5 * o_h_2):
                continue

            # iou_val = maxIou(xmin_1 , ymin_1 , xmax_1 - xmin_1 , ymax_1 - ymin_1 , xmin_2 , ymin_2 , xmax_2 - xmin_2 , ymax_2 - ymin_2)
            iou_val = maxIOU_2(xmin_1, ymin_1, xmax_1 - xmin_1, ymax_1 - ymin_1, xmin_2, ymin_2, xmax_2 - xmin_2,
                             ymax_2 - ymin_2)
            if iou_val > 0.75 or iou_val == 0:
                continue



            for blend in BLENDING_LIST:
                result_tmp = result.copy()
                rotate_shape_angle = random.randint(-30 , 30)
                result_tmp = Image.fromarray(np.uint8(result_tmp))

                random_index = np.random.choice(np.arange(num_foreground) , 1)
                mask_shape_img = img_infos[foreground_lsts[random_index[0]]][1]
                foreground_shape_img = img_infos[foreground_lsts[random_index[0]]][0]

                mask_shape_img = Image.fromarray(mask_shape_img).rotate(rotate_shape_angle , expand= True)
                foreground_shape_img = Image.fromarray(foreground_shape_img).rotate(rotate_shape_angle , expand= True)


                result_tmp.paste(foreground_shape_img , (img_w/2 , img_h/2) , mask_shape_img)

                if blend == "gaussian":
                    # result_tmp.paste(Image.fromarray(cv2.GaussianBlur(foreground_img_1 , (5 , 5) , 2)) , (x1 , y1) , Image.fromarray(np.uint8(mask_img_1)))
                    # result_tmp.paste(Image.fromarray(cv2.GaussianBlur(foreground_img_2 , (5 , 5) , 2)) , (x2 , y2) , Image.fromarray(np.uint8(mask_img_2)))

                    result_tmp.paste(Image.fromarray(foreground_img_1), (x1, y1),
                                     Image.fromarray(np.uint8(cv2.GaussianBlur(mask_img_1 , (5 , 5) , 2))))
                    result_tmp.paste(Image.fromarray(foreground_img_2), (x2, y2),
                                     Image.fromarray(np.uint8(cv2.GaussianBlur(mask_img_2 , (5 , 5) , 2))))


                elif blend == "poisson":
                    img_mask_1 , img_src_1 , offset_adj_1 = create_mask(mask_img_1.astype(np.float64) , result.copy() , foreground_img_1.astype(np.float64) , (y1 , x1))
                    result_tmp = poisson_blend(img_mask_1, img_src_1, result.copy(), method='normal', offset_adj=offset_adj_1)

                    img_mask_2 , img_src_2 , offset_adj_2 = create_mask(mask_img_2.astype(np.float64) , result_tmp , foreground_img_2.astype(np.float64) , (y2 , x2))
                    result_tmp = poisson_blend(img_mask_2, img_src_2, result_tmp, method='normal', offset_adj=offset_adj_2)

                    result_tmp = Image.fromarray(result_tmp , 'RGB')

                elif blend == "box":
                    # result_tmp.paste(Image.fromarray(cv2.blur(foreground_img_1 , (3 , 3))), (x1, y1),
                    #                  Image.fromarray(np.uint8(mask_img_1)))
                    # result_tmp.paste(Image.fromarray(cv2.blur(foreground_img_2 , (3 , 3))), (x2, y2),
                    #                  Image.fromarray(np.uint8(mask_img_2)))

                    result_tmp.paste(Image.fromarray(foreground_img_1), (x1, y1),
                                     Image.fromarray(np.uint8(cv2.blur(mask_img_1, (3, 3)))))
                    result_tmp.paste(Image.fromarray(foreground_img_2), (x2, y2),
                                     Image.fromarray(np.uint8(cv2.blur(mask_img_2, (3, 3)))))
                else:

                    result_tmp.paste(Image.fromarray(np.uint8(foreground_img_1)) , (x1 , y1) , Image.fromarray(np.uint8(mask_img_1)))
                    result_tmp.paste(Image.fromarray(np.uint8(foreground_img_2)) , (x2 , y2) , Image.fromarray(np.uint8(mask_img_2)))

                # imgFileName = os.path.basename(background_lst[i])[:-4] + "-" + label_1 + "-" + str(rotate_angles_1) + "-" + label_2 + "-" + str(rotate_angles_2) + "-" +  blend + ".jpg"
                imgFileName = '%s-%s-%s-%s-%s-%s.jpg' % (os.path.basename(background_lst[i])[:-4] , label_1 , str(rotate_angles_1) , label_2 , str(rotate_angles_2) , blend)
                # print(imgFileName)
                imagePath = os.path.join(imgFolderName, imgFileName)
                writer = PascalVocWriter(imgFolderName, imgFileName, (img_h, img_w, 3), localImgPath=imagePath,
                                         usrname="auto")
                writer.verified = True
                writer.addBndBox(xmin_1 , ymin_1 , xmax_1 , ymax_1 , label_1 , 0)
                writer.addBndBox(xmin_2, ymin_2, xmax_2, ymax_2, label_2, 0)
                writer.save(targetFile=imagePath[:-4] + XML_EXT)

                # result_tmp.show()
                # print(result_tmp.mode)
                result_tmp.save(imagePath)
                # cv2.imwrite(imagePath, PIL2array3C(result_tmp))

if __name__ == '__main__':

    start_time = time.time()

    background_imgs = glob.glob(os.path.join(os.getcwd() , "sku" , "background") + "/*.png")
    background_num = len(background_imgs)
    if background_num == 0:
        print '没有背景图片'
        sys.exit(1)


    all_lst = os.listdir(os.path.join(os.getcwd() , "sku"))
    dir_lst = []
    for i in all_lst:
        if os.path.isdir(os.path.join(os.getcwd() , "sku" , i)) and "json" in i:
            dir_lst.append(os.path.join(os.getcwd() , "sku" , i))
        else:
            continue

    if len(dir_lst) == 0:
        print '没有掩模文件夹'
        sys.exit(1)

    img_infos = {}

    mask_foreground_lsts = glob.glob(os.path.join(shape_dir , "*.png"))
    print(len(mask_foreground_lsts))
    assert len(mask_foreground_lsts) % 2 == 0
    foreground_lsts = []

    for i in mask_foreground_lsts:
        if "foreground" in i:
            foreground_img = cv2.imread(i)
            foreground_img = cv2.cvtColor(foreground_img , cv2.COLOR_BGR2RGB)
            mask_img = cv2.imread(i.replace("foreground" , "mask") , 0)
            foreground_lsts.append(i)
            img_infos[i] = []
            img_infos[i].append(foreground_img)
            img_infos[i].append(mask_img)

    generate_dir = os.path.join(os.getcwd() , "sku" , "generate")
    if os.path.exists(generate_dir):
        shutil.rmtree(generate_dir)
    else:
        os.mkdir(generate_dir)

    if os.path.isdir(generate_dir):
        pass
    else:
        os.mkdir(generate_dir)

    imgFolderName = generate_dir


    process_lst = []
    num_cpu = multiprocessing.cpu_count()

    if len(background_imgs) % num_cpu == 0:
        per_count = len(background_imgs) / num_cpu
    else:
        per_count = int(round(float(len(background_imgs))/float(num_cpu) + 0.5))

    for i in xrange(num_cpu):
        if i < num_cpu - 1:
            ps = multiprocessing.Process(target=process_sub_4 , args=(background_imgs[i * per_count:(i + 1) * per_count] , dir_lst))
        else:
            ps = multiprocessing.Process(target=process_sub_4 , args=(background_imgs[i * per_count:] , dir_lst))
        ps.daemon = True
        process_lst.append(ps)
    for i in range( num_cpu):
        process_lst[i].start()

    for  i in range(num_cpu):
        process_lst[i].join()


    cost_time = time.time() - start_time
    print('cost time:' , cost_time)



    
    
    
    








