#coding:utf-8

import cv2
import os
import glob
import numpy as np
import shutil
import sys
import uuid



def get_annotation_from_mask(mask):
    '''Given a mask, this returns the bounding box annotations

    Args:
        mask(NumPy Array): Array with the mask
    Returns:
        tuple: Bounding box annotation (xmin, xmax, ymin, ymax)
    '''
    rows = np.any(mask, axis=1)
    cols = np.any(mask, axis=0)
    if len(np.where(rows)[0]) > 0:
        ymin, ymax = np.where(rows)[0][[0, -1]]
        xmin, xmax = np.where(cols)[0][[0, -1]]
        return xmin, xmax, ymin, ymax
    else:
        return -1, -1, -1, -1
    
if __name__ == '__main__':
	data_dir = "/home/han/Desktop/ppp"
	all_lst = os.listdir(data_dir)
	dir_lst = []
	
	for i in all_lst:
		if os.path.isdir(os.path.join(data_dir , i)) and "json" in i:
			dir_lst.append(os.path.join(data_dir , i))
		else:
			continue
	
	if len(dir_lst) == 0:
		print
		'没有掩模文件夹'
		sys.exit(1)
	
	shape_dir = "shapes"
	if os.path.exists(shape_dir):
		shutil.rmtree(shape_dir)
	else:
		os.mkdir(shape_dir)
	if os.path.isdir(shape_dir):
		pass
	else:
		os.mkdir(shape_dir)
	key = uuid.uuid1()
	for index , d in enumerate(dir_lst):
		img_path = os.path.join(d , "img.png")
		label_path = os.path.join(d , "label.png")
		
		src_img = cv2.imread(img_path)
		label_img = cv2.imread(label_path , 0)
		_ , label_img = cv2.threshold(label_img , 0 , 255 , cv2.THRESH_BINARY)
		xmin_1, xmax_1, ymin_1, ymax_1 = get_annotation_from_mask(label_img)
		
		foreground_img_1 = src_img[ymin_1:ymax_1, xmin_1:xmax_1]
		label_img = label_img[ymin_1:ymax_1, xmin_1:xmax_1]
		img_foreground_name = os.path.join(shape_dir , str(key) + "-foreground-" + str(index) + ".png")
		img_mask_name = os.path.join(shape_dir, str(key) + "-mask-" + str(index) + ".png")
		cv2.imwrite(img_foreground_name , foreground_img_1)
		cv2.imwrite(img_mask_name , label_img)
	
	
	
