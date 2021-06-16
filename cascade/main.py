#coding:utf-8
import os
import glob
import numpy as np
import cv2
from car_detector import bow_training , svm_trainning
from car_detector import pyramid
from car_detector import sliding_window
from car_detector import non_maximum
import sys

# 将特征描述符转为特征向量
def des2feature(des , num_words , centers):
    '''

    :param des: 单幅图像的特征描述
    :param num_words: /视觉单词数/聚类中心
    :param centers: 聚类中心坐标 num_words * 128
    :return: feature vector  1 * num_words
    '''
    img_feature_vec = np.zeros((1 , num_words) , np.float32)
    for i in range(des.shape[0]):
        feature_k_rows = np.ones((num_words , 128) , np.float32)
        feature = des[i]
        feature_k_rows = feature_k_rows * feature
        feature_k_rows = np.sum((feature_k_rows - centers) ** 2 , 1)
        index = np.argmax(feature_k_rows)

        img_feature_vec[0][index] += 1
    return img_feature_vec

if __name__ == '__main__':
    test_image = "/Users/han/Desktop/67-sku/5028054e-88da-11e8-8865-17405fff1575-0-2.jpg"
    svm_model = "svm.xml"
    voc_model = "myvoc.npy"

    if not os.path.exists(svm_model) or not os.path.exists(voc_model):
        print('缺少模型文件')
        sys.exit(1)

    svm = svm_trainning.get_svm(svm_model)
    bow_extract = bow_training.get_extract_bow(voc_model)
    detect = cv2.xfeatures2d.SIFT_create()

    w , h = 100 , 40
    img = cv2.imread(test_image)

    rectangles = []
    counter = 0
    scale = 1

    for resized in pyramid.pyramid(img , scale=1.25):
        scale = float(img.shape[1]/float(resized.shape[1]))

        for (x , y ,roi) in sliding_window.sliding_window(resized , 20 , (w , h)):
            if roi.shape[1] != w or roi.shape[0] != h:
                continue
            try:
                # 使用视觉词袋集计算一张图像的描述符
                # 返回每个图像的bow码本，即直方图，大小为1*clusterNum
                bf = bow_training.bow_feature(roi , bow_extract , detect)
                cls , score = svm_trainning.predict(svm , roi , bow_extract , detect)
                if cls == 1:
                    if score < -0.5:
                        rx1, ry1, rx2, ry2 = int(x * scale), int(y * scale), int((x + w) * scale), int((y + h) * scale)
                        rectangles.append([rx1, ry1, rx2, ry2, abs(score)])
            except:
                pass

    windows = np.array(rectangles)
    print('windows:' , windows)
    boxes = non_maximum.non_max_suppression_fast(windows , 0.25)
    for (x , y , x2 , y2 , score) in boxes:
        cv2.rectangle(img, (int(x), int(y)), (int(x2), int(y2)), (0, 255, 0), 1)
        cv2.putText(img, "%f" % score, (int(x), int(y)), cv2.FONT_HERSHEY_PLAIN, 1, (0, 0, 255))

    cv2.imshow("img", img)
    cv2.waitKey(0)



