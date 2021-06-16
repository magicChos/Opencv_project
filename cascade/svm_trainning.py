#coding:utf-8
import cv2
import numpy as np
from tqdm import tqdm
from bow_training import bow_feature
from bow_training import get_extract_bow
import glob
import os


datapath = "/Users/han/Opencv_Project/cascade"
pos, neg = 'pos', 'neg'

def path(cls , i):
    return "%s/%s%d.png" % (datapath , cls , i + 1)

def extract_sift(fn , detect , extract):
    im = cv2.imread(fn , 0)
    return extract.compute(im , detect.detect(im))[1]

def train(voc_path , svm_model):
    bow_extract = get_extract_bow(voc_path)
    detect = cv2.xfeatures2d.SIFT_create()
    traindata , trainlabels = [] , []

    pos_path = os.path.join(datapath, pos)
    neg_path = os.path.join(datapath, neg)
    pos_img_lsts = glob.glob(pos_path + "/*.png")
    neg_img_lsts = glob.glob(neg_path + "/*.png")

    for i in pos_img_lsts:
        bow_descriptor = bow_feature(cv2.imread(i), bow_extract, detect)
        traindata.extend(bow_descriptor)
        trainlabels.append(1)

    for i in neg_img_lsts:
        try:
            bow_descriptor = bow_feature(cv2.imread(i), bow_extract, detect)
            traindata.extend(bow_descriptor)
            trainlabels.append(-1)
        except:
            print('neg path:' , i)

    # 创建svm实例
    svm = cv2.ml.SVM_create()
    svm.setType(cv2.ml.SVM_C_SVC)
    svm.setGamma(1)
    # 此参数可以决定分类器的训练误差和预测误差，其值越大，误判的可能习惯越小，但训练精度会降低。另一方面，值太低可能会导致过拟合，从而使预测精度降低
    svm.setC(35)
    svm.setKernel(cv2.ml.SVM_RBF)
    svm.train(np.array(traindata), cv2.ml.ROW_SAMPLE, np.array(trainlabels))

    svm.save(svm_model)

# 加载svm模型
def get_svm(svm_model):
    svm = cv2.ml.SVM_load(svm_model)
    return svm

def predict(svm , fn , bow_extract , detect):
    bf = bow_feature(fn , bow_extract , detect)
    result = svm.predict(bf)[1]
    res = svm.predict(bf , flags = cv2.ml.STAT_MODEL_RAW_OUTPUT)[1]

    # 返回classes , 得分
    return result[0][0] , res[0][0]
if __name__ == '__main__':
    print('--- beiginint training ---')
    save_model = "svm.xml"
    voc_path = "myvoc.npy"
    train(voc_path , save_model)
    print('--- ending trainning ---')