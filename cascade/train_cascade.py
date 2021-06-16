#coding:utf-8
import os
import glob
import cv2
import numpy as np
import string


if __name__ == '__main__':
    neg_path = "negtive.txt"
    pos_path = "positive.txt"

    pos_infos = []
    with open(pos_path) as fp:
        for eachline in fp:
            tmp = eachline.split()
            # print(tmp)
            pos_infos.append((tmp[0] , string.atoi(tmp[2]) , string.atoi(tmp[3]) , string.atoi(tmp[4]) , string.atoi(tmp[5])))
    print(pos_infos)

    pos_img = cv2.imread(pos_infos[0][0])
    pos_x , pos_y , pos_w , pos_h = pos_infos[0][1] , pos_infos[0][2] , pos_infos[0][3] , pos_infos[0][4]
    cv2.rectangle(pos_img , (pos_x , pos_y) , (pos_x + pos_w , pos_y + pos_h) , (0, 0 , 255))

    pos_roi = pos_img[pos_y:pos_y+pos_h , pos_x:pos_x+pos_w]
    cv2.imshow('pos_img' , pos_img)
    cv2.imshow('pos_roi' , pos_roi)

    neg_infos = []
    with open(neg_path) as fp:
        for eachline in fp:
            tmp = eachline.split()
            # print(tmp)
            neg_infos.append(
                (tmp[0], string.atoi(tmp[2]), string.atoi(tmp[3]), string.atoi(tmp[4]), string.atoi(tmp[5])))
    print(neg_infos)

    neg_img = cv2.imread(neg_infos[0][0])
    neg_x, neg_y, neg_w, neg_h = neg_infos[0][1], neg_infos[0][2], neg_infos[0][3], neg_infos[0][4]
    cv2.rectangle(neg_img, (neg_x, neg_y), (neg_x + neg_w, neg_y + neg_h), (0, 0, 255))

    neg_roi = neg_img[neg_y:neg_y + neg_h, neg_x:neg_x + neg_w]
    cv2.imshow('neg_img', neg_img)
    cv2.imshow('neg_roi', neg_roi)
    cv2.waitKey(0)
