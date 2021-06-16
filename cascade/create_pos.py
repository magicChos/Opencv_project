#coding:utf-8
'''
生成 opencv_traincascade格式的数据集
格式：
    文件路径 num_obj x_1 y_1 w_1 h_1 ... x_num y_num w_num h_num
'''

import os
import glob
import cv2
import numpy as np
import xml.etree.cElementTree as ET
import shutil



CLASSES = ['67']
WIDTH = 64
HEIGHT = 64

def resize_image(image , new_shape):
    '''

    :param image:
    :param new_shape:
    :return:
    '''
    src_h , src_w , c = image.shape
    new_h = new_shape[0]
    new_w = new_shape[1]

    scale_h = float(new_h)/float(src_h)
    scale_w = float(new_w)/float(src_w)

    if scale_w < scale_h:
        tmp = np.zeros(shape=(new_h , new_w , c) , dtype=np.uint8) * 0
        resize_h = int(scale_w * src_h)
        resize_w = new_w

        resize_img = cv2.resize(image , (resize_w , resize_h))
        extra_rows = new_h - resize_h
        tmp[extra_rows/2 : extra_rows/2 + resize_h , :] = resize_img
        return tmp
    else:
        tmp = np.zeros(shape=(new_h, new_w, c), dtype=np.uint8) * 0
        resize_h = new_h
        resize_w = int(src_w * scale_h)

        resize_img = cv2.resize(image, (resize_w, resize_h))
        extra_cols = new_w - resize_w
        tmp[: ,  extra_cols/2 : extra_cols/2 + resize_w, :] = resize_img
        return tmp

def readAnnotations(xml_path):
    et = ET.parse(xml_path)
    element = et.getroot()
    element_objs = element.findall('object')
    # element_filename = element.find('filename').text
    element_width = int(element.find('size').find('width').text)
    element_height = int(element.find('size').find('height').text)

    results = []
    if element_width == 0 or element_height == 0:
        os.remove(xml_path)
        print('the xml size is 0 ,  delete the xml:', xml_path)
        return results
    for element_obj in element_objs:
        result = []
        class_name = element_obj.find('name').text
        if class_name == None:
            print('the xml class is None , delete the xml :', os.path.basename(xml_path))
        else:
            obj_bbox = element_obj.find('bndbox')
            x1 = int(round(float(obj_bbox.find('xmin').text)))
            y1 = int(round(float(obj_bbox.find('ymin').text)))
            x2 = int(round(float(obj_bbox.find('xmax').text)))
            y2 = int(round(float(obj_bbox.find('ymax').text)))

            result.append(class_name)
            result.append(x1)
            result.append(y1)
            result.append(x2)
            result.append(y2)
            results.append(result)
    return results

if __name__ == '__main__':
    positive_file = "positive.txt"
    img_path = "/Users/han/Desktop/67-sku"
    xml_path = "/Users/han/Desktop/67-sku"

    img_lsts = glob.glob(os.path.join(img_path , "*.jpg"))
    xml_lsts = glob.glob(os.path.join(xml_path , "*.xml"))
    pos_dir = "pos"

    if os.path.exists(pos_dir):
        shutil.rmtree(pos_dir)
    else:
        os.mkdir(pos_dir)
    if os.path.isdir(pos_dir):
        pass
    else:
        os.mkdir(pos_dir)

    assert len(img_lsts) == len(xml_lsts)

    f_p = open(positive_file , 'w')
    pos_count = 1
    for i, xml in enumerate(xml_lsts):
        img_name = xml.replace("xml" , "jpg")
        if not os.path.exists(img_name):
            img_name = xml.replace("xml" , "png")

        img = cv2.imread(img_name)
        if type(img) == type(None):
            print('读取图片失败！')
            continue
        else:
            results = readAnnotations(xml)

            result_pos = []
            result_neg = []
            for result in results:
                if result[0] in CLASSES:
                    result_pos.append(result)
                else:
                    result_neg.append(result)

            num_pos = len(result_pos)
            num_neg = len(result_neg)

            if num_pos == 1:
                content = '%s   %s   %s   %s   %s  %s\n' % (
                    img_name, 1, result_pos[0][1], result_pos[0][2], result_pos[0][3] - result_pos[0][1],
                    result_pos[0][4] - result_pos[0][2])
                f_p.write(content)

                x1 = result_pos[0][1]
                x2 = result_pos[0][3]
                y1 = result_pos[0][2]
                y2 = result_pos[0][4]

                roi = img[y1:y2 , x1:x2]
                roi_name = os.path.join(os.getcwd() , pos_dir , os.path.basename(img_name))


                roi = resize_image(roi , (WIDTH , HEIGHT))
                cv2.imwrite(roi_name , roi)

                # f_p.write(roi_name + "\n")
                pos_count += 1

    f_p.close()











