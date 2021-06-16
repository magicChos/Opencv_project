#coding:utf-8

import cv2
import os
import glob

if __name__ == '__main__':
    img_path = "neg-130.png"
    img = cv2.imread(img_path , 0)
    sift_detect = cv2.xfeatures2d.SIFT_create()

    keypoints, descriptor = sift_detect.detectAndCompute(img , None)
    print(type(keypoints))
    print(keypoints)


    img = cv2.drawKeypoints(image=img , outImage=img , keypoints=keypoints , flags=cv2.DRAW_MATCHES_FLAGS_DEFAULT)
    cv2.imshow("img" , img)
    cv2.waitKey(0)
