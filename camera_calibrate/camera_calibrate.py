#coding:utf-8


import cv2
import numpy as np
import shutil
import os
import json
import glob

# 标定的黑白格子的大小单位nm
squareSize = 25

class CameraCalibrate(object):
    def __init__(self , chessboard_size , img_dir):
        self.chessboard_size = chessboard_size
        img_lsts = os.listdir(img_dir)
        self.img_lsts = [os.path.join(img_dir , i) for i in img_lsts if i.endswith("jpg") or i.endswith("png")]

        self.criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

        self.objpoints = []
        self.imgpoints = []

        # 相机内参
        self.mtx = None
        # 畸变系数
        self.dist = None

        self.rvecs = None
        self.tvecs = None
        
    def calibrate(self , showflag , save_params_file):
        (chess_w , chess_h) = self.chessboard_size
        objP = np.zeros((chess_w * chess_h , 3) , np.float32)
        objP[:,:2] = np.mgrid[0:chess_h , 0:chess_w].T.reshape(-1 , 2)

        # objpoints = []
        # imgpoints = []

        for fname in self.img_lsts:
            img = cv2.imread(fname)
            print(fname)
            gray = cv2.cvtColor(img , cv2.COLOR_BGR2GRAY)

            ret , corners = cv2.findChessboardCorners(gray , (chess_h , chess_w) , cv2.CALIB_CB_ADAPTIVE_THRESH)
            if ret == True:
                self.objpoints.append(objP * squareSize)
                corners2 = cv2.cornerSubPix(gray , corners , (11 , 11) , (-1 , -1) , self.criteria)
                self.imgpoints.append(corners2)

                if showflag:
                    img = cv2.drawChessboardCorners(img , (chess_h , chess_w) , corners2 , ret)
                    cv2.imshow("img" , img)
                    # cv2.waitKey(0)
            else:
                print('can not find chessboardCorners:' , fname)
        if(len(self.imgpoints) < 9):
            print('do not have enough images!')
            return

        # mtx:相机内参，dist：畸变系数
        # ret , self.mtx , self.dist , self.rvecs , self.tvecs = cv2.calibrateCamera(self.objpoints , self.imgpoints , gray.shape[::-1] , None , None , flags=cv2.CALIB_FIX_K3 + cv2.CALIB_ZERO_TANGENT_DIST)
        ret, self.mtx, self.dist, self.rvecs, self.tvecs = cv2.calibrateCamera(self.objpoints, self.imgpoints,
                                                                               gray.shape[::-1], None, None,
                                                                               )
        params_info = {}
        params_info['mtx'] = self.mtx.tolist()
        params_info['dist'] = self.dist.tolist()

        with open(save_params_file , "w") as f:
            json.dump(params_info , f)

    def getProjectError(self):
        total_error = 0
        for i in xrange(len(self.objpoints)):
            imagepoints2 , _ = cv2.projectPoints(self.objpoints[i] , self.rvecs[i] , self.tvecs[i] , self.mtx , self.dist)
            error = cv2.norm(self.imgpoints[i] , imagepoints2 , cv2.NORM_L2)/len(imagepoints2)
            total_error += error
        return total_error/len(self.objpoints)



def undistImage(img , json_params):
    '''
    矫正图片
    :param img: numpy ndarray
    :param json_params: 保存相机的畸变参数
    :return:
    '''
    load_params = {}
    with open(json_params , 'r') as f:
        load_params = json.load(f)

    mtx = np.array(load_params['mtx'])
    dist = np.array(load_params['dist'])

    h , w = img.shape[:2]
    newcamerax, roi = cv2.getOptimalNewCameraMatrix(mtx, dist, (w, h), 1, (w, h))

    dst = cv2.undistort(img , mtx , dist , None , newcamerax)
    x , y , w , h = roi
    dst = dst[y:y + h, x: x + w]
    return dst

if __name__ == '__main__':
    chess_shape = (9 , 6)
    
    left_path = "left"
    right_path = "right"
    
    # left_path = "/home/han/Desktop/left"
    # right_path = "/home/han/Desktop/right"

    # # 标定
    # calibrate_obj = CameraCalibrate(chess_shape , left_path)
    # calibrate_obj.calibrate(True , os.path.join(left_path , "cameraparam_left.json"))
    # project_error = calibrate_obj.getProjectError()
    # print('project_error:' , project_error)
    #
    # calibrate_obj = CameraCalibrate(chess_shape, right_path)
    # calibrate_obj.calibrate(True, os.path.join(right_path, "cameraparam_right.json"))
    #
    # project_error = calibrate_obj.getProjectError()
    # print('project_error:' , project_error)

    # 矫正
    img_lst = glob.glob("left" + "/*.jpg")
    cv2.namedWindow("dst")
    cv2.namedWindow("img")
    for i_img in img_lst:
        img = cv2.imread(i_img)
        dst = undistImage(img, os.path.join("left", "cameraparam_left.json"))
        print(np.shape(dst))
        cv2.moveWindow("dst" , 100 , 100)
        cv2.moveWindow("img" , 100 + img.shape[1] , 100)
        cv2.imshow("dst", dst)
        cv2.imshow("img", img)

        cv2.waitKey(0)


    img_lst = glob.glob("right" + "/*.jpg")
    for i_img in img_lst:
        img = cv2.imread(i_img)
        dst = undistImage(img, os.path.join("right", "cameraparam_right.json"))
        print(np.shape(dst))
        cv2.moveWindow("dst" , 100 , 100)
        cv2.moveWindow("img" , 100 + img.shape[1] , 100)
        cv2.imshow("dst", dst)
        cv2.imshow("img", img)

        cv2.waitKey(0)








