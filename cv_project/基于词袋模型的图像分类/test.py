#!/usr/bin/env python
# -*- encoding: utf-8 -*-
'''
@filename    :test.py
@brief       :计算图像在bow下的特征进行类别预测
@time        :2020/03/14 20:34:16
@author      :hscoder
@versions    :1.0
@email       :hscoder@163.com
'''



import cv2
import random
from utils import create_csv , map_label
from BowClass import BOW
import numpy as np

if __name__ == "__main__":
    visual_vocabulary = "bow.npy"
    svm_model = "classify.xml"
    bow = BOW()
    bow.load(visual_vocabulary, svm_model)

    test_samples = 25000
    test_results = np.zeros(test_samples, dtype=np.bool)

    test_images , test_labels = create_csv('data/猫狗训练集' , 'test.csv' , map_label , False)
    index = random.sample(range(len(test_images)) , test_samples)
    test_data = []
    for i in index:
        test_data.append((test_images[i], test_labels[i]))

    for i , (imageName , label) in enumerate(test_data):
        predict_res = bow.predict(imageName)
        label = True if label == 1 else False
        test_results[i] = predict_res == label

    accuracy = np.mean(test_results.astype(np.float32))
    print("测试准确率为：" , accuracy)

