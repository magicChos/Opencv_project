#!/usr/bin/env python
# -*- encoding: utf-8 -*-
'''
@filename    :BowClass.py
@brief       :视觉词汇树封装类，可用于生成词汇字典，完成svm的训练
@time        :2020/03/14 20:19:12
@author      :hscoder
@versions    :1.0
@email       :hscoder@163.com
'''
import cv2
import numpy as np
from tqdm import tqdm


class BOW(object):
 
    def __init__(self, ):
        # 创建一个SIFT对象  用于关键点提取
        self.feature_detector = cv2.xfeatures2d.SIFT_create()
        # 创建一个SIFT对象  用于关键点描述符提取
        self.descriptor_extractor = cv2.xfeatures2d.SIFT_create()

        # 创建一个SVM对象
        self.svm = cv2.ml.SVM_create()

        # FLANN匹配  参数algorithm用来指定匹配所使用的算法，可以选择的有LinearIndex、KTreeIndex、KMeansIndex、CompositeIndex和AutotuneIndex，这里选择的是KTreeIndex(使用kd树实现最近邻搜索)
        self.flann_params = dict(algorithm=1, tree=5)
        self.matcher = cv2.FlannBasedMatcher(self.flann_params, {})
       
        # 初始化bow提取器(设置词汇字典),用于提取每一张图像的BOW特征描述
        self.bow_img_descriptor_extractor = cv2.BOWImgDescriptorExtractor(self.descriptor_extractor, self.matcher)
 
    def fit(self, train_data, k , visual_vocabulary , svm_model):
        '''
        开始训练，包括创建视觉词汇字典和svm分类模型
        args：
            train_path：训练集图片路径  我们使用的数据格式为 ('/Users/han/Opencv_Project/opencv_opengl/基于词袋模型的图像分类/data/猫狗训练集/train/dog.12217.jpg', 0)
            k：k-means参数k
        '''
 
        # 创建BOW训练器，指定k-means参数k   把处理好的特征数据全部合并，利用聚类把特征词分为若干类，此若干类的数目由自己设定，每一类相当于一个视觉词汇
        bow_kmeans_trainer = cv2.BOWKMeansTrainer(k)

        # 通过聚类创建视觉词汇
        train_data = tqdm(train_data)
        for imagePath , _ in train_data:
            bow_kmeans_trainer.add(self.sift_descriptor_extractor(imagePath))

        # 进行k-means聚类，返回词汇字典 也就是聚类中心
        voc = bow_kmeans_trainer.cluster()
        self.bow_img_descriptor_extractor.setVocabulary(voc)

        # 输出词汇字典  <class 'numpy.ndarray'> (40, 128)
        print(type(voc), voc.shape)
        np.save(visual_vocabulary , voc)
        print("-----------create visual vocabulary finished-----------")
 
        # 创建两个数组，分别对应训练数据和标签，并用BOWImgDescriptorExtractor产生的描述符填充
        traindata, trainlabels = [], []

        for imagePath , label in train_data:
            traindata.extend(self.bow_descriptor_extractor(imagePath))
            trainlabels.append(label)
        print("-----------开始训练svm------------")
        # 使用训练数据和标签进行训练
        self.svm.train(np.array(traindata), cv2.ml.ROW_SAMPLE, np.array(trainlabels))

        self.svm.save(svm_model)
        
    def load(self , visual_vocabulary , svm_model):
        '''
        加载视觉词汇字典和svm模型
        '''

        self.svm = cv2.ml.SVM_load(svm_model)
        voc = np.load(visual_vocabulary)
        self.bow_img_descriptor_extractor.setVocabulary(voc)


    def predict(self, img_path):
        '''
        进行预测样本
        '''
        # 提取图片的BOW特征描述
        data = self.bow_descriptor_extractor(img_path)
        res = self.svm.predict(data)
 
        # 如果是狗 返回True
        if res[1][0][0] == 1.0:
            return True
        # 如果是猫，返回False
        else:
            return False
 
    def sift_descriptor_extractor(self, img_path):
        '''
        特征提取：提取数据集中每幅图像的特征点，然后提取特征描述符，形成特征数据(如：SIFT或者SURF方法)；
        '''
        im = cv2.imread(img_path, 0)
        return self.descriptor_extractor.compute(im, self.feature_detector.detect(im))[1]
 
    def bow_descriptor_extractor(self, img_path):
        '''
        提取图像的BOW特征描述(即利用视觉词袋量化图像特征)
        '''

        im = cv2.imread(img_path , 0)
        return self.bow_img_descriptor_extractor.compute(im, self.feature_detector.detect(im))
 

