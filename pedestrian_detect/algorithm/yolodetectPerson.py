#!/usr/bin/env python
# -*- encoding: utf-8 -*-
'''
@filename    :yolodetectPerson.py
@brief       :利用yolov3-tiny做行人检测
@time        :2020/03/14 11:39:47
@author      :hscoder
@versions    :1.0
@email       :hscoder@163.com
'''



import cv2
from pydarknet import Detector , Image
import os 
import numpy as np

abs_path = os.path.dirname(os.path.abspath(__file__))
model_dir_path = os.path.join(abs_path , ".." , "model")
print(model_dir_path)

print(os.path.join(model_dir_path, "yolov3-tiny.cfg"))

try:
    net = Detector(bytes(os.path.join(model_dir_path, "yolov3-tiny.cfg"), encoding = "utf-8"),
                   bytes(os.path.join(model_dir_path, "yolov3-tiny.weights"), encoding = "utf-8"), 0,
                   bytes(os.path.join(model_dir_path, "yolov3-tiny.data"), encoding = "utf-8"))
except:
    print("Please check cfg and model files")


def yolodetectPerson(img , thresh_val):
    image = Image(img)
    results = net.detect(image)

    res = []
    for cat , conf , bd in results:
        if cat != b'person':
            continue

        x , y , w , h = bd
        leftSrc = int(x - w / 2) 
        topSrc = int(y - h / 2) 
        rightSrc = int(x + w / 2) 
        bottomSrc = int(y + h / 2)

        if conf > thresh_val:
            conf *= conf
            # res.append([str(cat.decode("utf-8")), conf, [leftSrc, topSrc, rightSrc, bottomSrc]])
            res.append([leftSrc, topSrc, rightSrc, bottomSrc])

    return res

if __name__ == "__main__":
    testImg = cv2.imread("aobama.jpeg")
    results = yolodetectPerson(testImg , 0.6)

    for (x1 , y1 , x2 , y2) in results:
        cv2.rectangle(testImg , (x1 , y1 ), (x2 , y2) , (0 , 0 , 255))

    cv2.imshow("image" , testImg)
    cv2.waitKey(0)

    






