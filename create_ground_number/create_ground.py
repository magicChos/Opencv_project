#coding:utf-8

import os
import cv2
import numpy as np
from PIL import Image
import random
from pascal_voc_io import PascalVocWriter
from pascal_voc_io import XML_EXT
import imutils
from imutils import contours
import shutil


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

if __name__ == '__main__':
    imgName = "/Users/han/Desktop/0125596_1.jpg"
    ground_img = cv2.imread(imgName)
    cv2.imshow("ground" , ground_img)

    srcTri = np.float32([[0 , 0] , [0 , 69] , [349 , 0]])

    saveDir = "output"
    if os.path.exists(saveDir):
        shutil.rmtree(saveDir)
    else:
        os.mkdir(saveDir)
    if os.path.isdir(saveDir):
        pass
    else:
        os.mkdir(saveDir)

    count = 1
    while count < 10000:
        dstTri = np.float32([[random.randint(0 , 20) , random.randint(0 , 20)] ,
                             [random.randint(0 , 20) , 69 - random.randint(0 , 20)] ,
                             [349 - random.randint(0 , 10) , random.randint(0, 20)]])
        warp_mat = cv2.getAffineTransform(srcTri , dstTri)
        print(warp_mat)
        img = np.zeros((70 , 360 , 3) , dtype=np.uint8)
        img[:,:] = [30 + random.randint(-5 , 5) , 190 + random.randint(-20 , 20) , 245 - random.randint(0 , 5)]

        plate_text = []
        for i in range(7):
            num = np.random.randint(0, 10)
            plate_text.append(str(num))
            cv2.putText(img, str(num), (i * 50 + 10 , 55) , cv2.FONT_HERSHEY_SIMPLEX, 2.0, (0, 0 , 0) , 5)
        name = ''.join(plate_text)
        imgName = name + ".png"
        imagePath = os.path.join(saveDir , imgName)


        gray = cv2.cvtColor(img , cv2.COLOR_BGR2GRAY)
        binary = cv2.threshold(gray , 0 , 255 , cv2.THRESH_BINARY_INV)[1]
        cnts , hier = cv2.findContours(binary , cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        cnts = contours.sort_contours(cnts , method='left-to-right')[0]
        dst = cv2.warpAffine(img, warp_mat , dsize=(img.shape[1] , img.shape[0]))
        writer = PascalVocWriter(saveDir , imgName , (dst.shape[0] , dst.shape[1] , 3) , localImgPath = imagePath , usrname = "auto")
        writer.verified = True
        for i , cnt in enumerate(cnts):
            rect = cv2.minAreaRect(cnt)
            box = cv2.boxPoints(rect).tolist()
            box = np.expand_dims(box, axis = 1)
            box = box.reshape(4 , 2).tolist()

            rot_pts = warfPoints(box , warp_mat)
            rot_pts = np.array(rot_pts)
            xmin , ymin  = np.min(rot_pts , axis=0)
            xmax , ymax  = np.max(rot_pts , axis=0)

            writer.addBndBox(int(xmin) , int(ymin) , int(xmax) , int(ymax) , plate_text[i] , 0)
        writer.save(targetFile=imagePath[:-4] + XML_EXT)

        if random.randint(0 , 3) == 2:
            dst = cv2.GaussianBlur(dst, (5, 5) , 0)
        cv2.imwrite(imagePath , dst)

        count += 1




