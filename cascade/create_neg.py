#coding:utf-8
import os
import glob
import cv2
import numpy as np
import uuid
import shutil
import datetime
datetime.date

if __name__ == '__main__':
    neg_dir = "/Users/han/generate_data/sku/background"
    img_lsts = glob.glob(os.path.join(neg_dir + "/*.png"))
    save_path = "/Users/han/Opencv_Project/cascade/neg"
    roi_h , roi_w = 96 , 128

    if os.path.exists(save_path):
        shutil.rmtree(save_path)
    else:
        os.mkdir(save_path)

    if os.path.isdir(save_path):
        pass
    else:
        os.mkdir(save_path)

    key = uuid.uuid1()
    index = 0
    for i in img_lsts:
        img = cv2.imread(i)
        if type(img) == type(None):
            continue
        else:
            img_h , img_w = img.shape[:2]
            for i_h in xrange(roi_h , img_h , 64):
                for i_w in xrange(roi_w , img_w , 64):
                    if (i_h + roi_h) < img_h and (i_w + roi_w) < img_w:
                        roi = img[i_h:i_h + roi_h , i_w:i_w + roi_w]
                        roi_name = '%s-%s.png' % (str(key) , str(index))
                        roi_name = os.path.join(save_path , roi_name)
                        cv2.imwrite(roi_name , roi)

                        index += 1

    neg_file = "negtive.txt"
    with open(neg_file , 'w') as fp:
        neg_lsts = glob.glob(os.path.join(save_path , "*.png"))
        for i in neg_lsts:
            fp.write(i)
            fp.write('\n')


    print('finished!')




