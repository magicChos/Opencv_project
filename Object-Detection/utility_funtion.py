#coding:utf-8
'''
一些实用功能实现
'''

import cv2
import os
import glob
import numpy as np


def resize_image(image):
    '''
    按照该图像的尺寸，将该图像缩放为方形
    :param image: opencv narray
    :return: 新图像
    '''

    img_h , img_w ,c = image.shape
    if img_h > img_w:
        temp = np.ones(shape=(img_h , img_h , c) , dtype=np.uint8) * 0
        extra_rows = img_h - img_w
        temp[: , extra_rows/2 : extra_rows/2 + img_w] = image
        return temp
    if img_w > img_h:
        temp = np.ones(shape=(img_w , img_w , c) , dtype=np.uint8) * 0
        extra_cols = img_w - img_h
        temp[extra_cols/2 : extra_cols/2 + img_h , :] = image
        return temp

def resize_image(image , new_shape):
    '''

    :param image:
    :param new_shape:
    :return:
    '''
    src_h , src_w , c = image.shape
    new_h = new_shape[0]
    new_w = new_shape[1]

    scale_h = float(new_h)/float(src_h)
    scale_w = float(new_w)/float(src_w)

    if scale_w < scale_h:
        tmp = np.zeros(shape=(new_h , new_w , c) , dtype=np.uint8) * 0
        resize_h = int(scale_w * src_h)
        resize_w = new_w

        resize_img = cv2.resize(image , (resize_w , resize_h))
        extra_rows = new_h - resize_h
        tmp[extra_rows/2 : extra_rows/2 + resize_h , :] = resize_img
        return tmp
    else:
        tmp = np.zeros(shape=(new_h, new_w, c), dtype=np.uint8) * 0
        resize_h = new_h
        resize_w = int(src_w * scale_h)

        resize_img = cv2.resize(image, (resize_w, resize_h))
        extra_cols = new_w - resize_w
        tmp[: ,  extra_cols/2 : extra_cols/2 + resize_w, :] = resize_img
        return tmp





if __name__ == '__main__':
    img_path = "/Users/han/Opencv_Project/Object-Detection/4_1539311172_642933.jpg"
    img = cv2.imread(img_path)

    # img_resize = resize_image(img)
    dst_resize = cv2.resize(img.copy() , (512 , 512))
    cv2.imshow("img" , dst_resize)
    img_resize = resize_image(img , (512 , 512))
    cv2.imshow("resize image" , img_resize)
    cv2.imwrite("resize_image.jpg" , img_resize)
    cv2.waitKey(0)
