#coding:utf-8
import cv2
import numpy as np
from tqdm import tqdm
import os
import glob


SAMPLES = 400
datapath = "/Users/han/Opencv_Project/cascade"
pos, neg = 'pos', 'neg'

def path(cls , i):
    return "%s/%s%d.png" % (datapath , cls , i + 1)

def extract_sift(fn , detect , extract):
    im = cv2.imread(fn , 0)

    # print(extract.compute(im , detect.detect(im))[1].shape)
    return extract.compute(im , detect.detect(im))[1]


def train(num_cluster , path_save):
    # 创建bow训练器
    bow_kmeans_trainer = cv2.BOWKMeansTrainer(num_cluster)

    detect , extract = cv2.xfeatures2d.SIFT_create() , cv2.xfeatures2d.SIFT_create()
    # for i in tqdm(range(SAMPLES)):
    #     try:
    #         bow_kmeans_trainer.add(extract_sift(path(pos , i) , detect , extract))
    #         bow_kmeans_trainer.add(extract_sift(path(neg , i) , detect , extract))
    #     except:
    #         print('pos path:' , path(pos , i))
    #         print('neg path:' , path(neg , i))
    #         continue


    pos_path = os.path.join(datapath , pos)
    neg_path = os.path.join(datapath , neg)
    pos_img_lsts = glob.glob(pos_path + "/*.png")
    neg_img_lsts = glob.glob(neg_path + "/*.png")

    for i in pos_img_lsts:
        # try:
        #     descritor = extract_sift(i , detect , extract)
        #     bow_kmeans_trainer.add(descritor)
        # except:
        #     os.remove(i)
        #     continue
        try:
            descritor = extract_sift(i, detect, extract)
        except:
            os.remove(i)
            continue
        # print('.')
        bow_kmeans_trainer.add(descritor)

    for i in neg_img_lsts:
        # try:
        #     descritor = extract_sift(i, detect, extract)
        #     bow_kmeans_trainer.add(descritor)
        # except:
        #     os.remove(i)
        #     continue
        try:
            descritor = extract_sift(i, detect, extract)
        except:
            os.remove(i)
            continue
        print(i)
        bow_kmeans_trainer.add(descritor)
    print('-------------here----------------')
    # 执行k-means分类并返回词汇
    voc = bow_kmeans_trainer.cluster()
    print('-------------save----------------')
    np.save(path_save , voc)

# 获取bow提取器
def get_extract_bow(voc_path):
    extract = cv2.xfeatures2d.SIFT_create()
    flann_params = dict(algorithm=1, trees=5)
    matcher = cv2.FlannBasedMatcher(flann_params, {})

    extract_bow = cv2.BOWImgDescriptorExtractor(extract , matcher)
    voc = np.load(voc_path)
    print('type of voc:' , type(voc))
    extract_bow.setVocabulary(voc)

    print('running get_extract_bow')
    return extract_bow

# 基于bow描述符提取器计算得到描述符
def bow_feature(img , extract_bow , detect):
    gray = cv2.cvtColor(img , cv2.COLOR_BGR2GRAY)
    return extract_bow.compute(gray , detect.detect(gray))

if __name__ == '__main__':
    print('--- starting trainning ---')
    path_save = "myvoc"
    train(1000 , path_save)
    print('--- ending trainning ---')









