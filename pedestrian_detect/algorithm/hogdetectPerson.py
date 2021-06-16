#!/usr/bin/env python
# -*- encoding: utf-8 -*-
'''
@filename    :hogdetectPerson.py
@brief       :利用HOG做行人检测
@time        :2020/03/14 11:17:55
@author      :hscoder
@versions    :1.0
@email       :hscoder@163.com
'''

from imutils.object_detection import non_max_suppression
from imutils.paths import list_images
import numpy as np
import cv2
import time
import imutils

hog = cv2.HOGDescriptor()
hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())
# hog.setSVMDetector(cv2.HOGDescriptor_getDaimlerPeopleDetector())

def hogdetectPerson(img):
    image = imutils.resize(img, width=min(400 , img.shape[1]))
    scale = float(float(img.shape[0] / image.shape[0]))

    (rects , weights) = hog.detectMultiScale(image , winStride=(4 , 4) , padding=(8 , 8) , scale=1.05)
    # apply nms
    rects = np.array([[x , y , x + w , y + h] for (x , y , w , h) in rects])
    pick = non_max_suppression(rects , probs = None , overlapThresh=0.65)

    pick = scale * pick

    return pick



if __name__ == "__main__":
    testImg = cv2.imread("pedtrian.jpeg")
    results = hogdetectPerson(testImg)

    for (x1 , y1 , x2 , y2) in results:
        cv2.rectangle(testImg , (int(x1) , int(y1) ), (int(x2) , int(y2)) , (0 , 0 , 255))

    cv2.imshow("image" , testImg)
    cv2.waitKey(0)

