
#!/usr/bin/env python
# -*- encoding: utf-8 -*-
'''
@filename    :train.py
词袋模型BOW+SVM 目标识别
以狗和猫数据集二分类为例
如果是狗 返回True
如果是猫 返回False
@time        :2020/03/14 16:05:13
@author      :hscoder
@versions    :1.0
@email       :hscoder@163.com
'''

import numpy as np
import cv2
from utils import create_csv , map_label
import random
from BowClass import BOW
 
if __name__ == '__main__':
    images , labels = create_csv('data/猫狗训练集' , 'train.csv' , map_label , True)

    # 随机选取200
    # 220张图像作为训练数据
    index = random.sample(range(len(images)), 5000)
    train_data = []
    for i in index:
        train_data.append((images[i] , labels[i]))

    visual_vocabulary = "bow"
    svm_model = "classify.xml"

    bow = BOW()
    bow.fit(train_data , 40 , visual_vocabulary , svm_model)
    


    

    