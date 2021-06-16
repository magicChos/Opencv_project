#coding:utf-8
import cv2
import os
import sys
import stereo_calibrate
import numpy as np

if __name__ == '__main__':
    stereo_calib = stereo_calibrate.Stereo_Calibrate("" , "")
    stereo_calib.loadCalibResuls("stereo_calib_result.json")

    cameraMatrixL , distCoeffL , cameraMatrixR , distCoeffR = stereo_calib.getNewIntristicMatrix()

    # left_img_path = "left/left-10.jpg"
    # right_img_path = left_img_path.replace("left", "right")
    # rectify_before , rectify_after = stereo_calib.stereo_rectify(left_img_path , right_img_path)
    # cv2.imshow("before" , rectify_before)
    # cv2.imshow("after" , rectify_after)


    W = stereo_calib.getTranformWMatrix()
    print("W shape:" , np.shape(W))
    print(W)

    # cv2.waitKey(0)




