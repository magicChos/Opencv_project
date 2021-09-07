# coding:utf-8
import cv2
import argparse
import numpy as np


def raw2Color(ir_raw):
    cv2.normalize(ir_raw, ir_raw, 0, 65535, cv2.NORM_MINMAX)
    ir_raw = ir_raw/256.0
    ir_gray = ir_raw.astype(np.uint8)
    ir_color = cv2.applyColorMap(ir_gray, cv2.COLORMAP_HOT)

    return ir_color, ir_gray


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--color', required=True)
    parser.add_argument('--ir', required=True)
    args = parser.parse_args()

    ir_img = cv2.imread(args.ir, -1)
    color_img = cv2.imread(args.color)
    color_img = cv2.resize(color_img, (597, 448))
    ir_color, ir_gray = raw2Color(ir_img)

    cv2.imshow('ir_gray', ir_gray)
    cv2.imshow("ir_color", ir_color)
    cv2.imshow("color_img", color_img)
    cv2.waitKey(0)

    ir_points = [[14, 9], [4, 162], [252, 175], [259, 29]]
    color_points = [[162, 139], [164, 265], [379, 255], [377, 141]]

    ir_points = np.float32(ir_points).reshape(-1, 1, 2)
    color_points = np.float32(color_points).reshape(-1, 1, 2)

    H, mask = cv2.findHomography(ir_points, color_points, cv2.RANSAC, 5.0)
    matchesMask = mask.ravel().tolist()
    print(matchesMask)

    print("H: ", H)

    h, w, d = ir_color.shape
    pts = np.float32([[0, 0], [0, h-1], [w-1, h-1], [w-1, 0]]
                     ).reshape(-1, 1, 2)
    dst = cv2.perspectiveTransform(pts, H)
    img2 = cv2.polylines(color_img, [np.int32(dst)], True, 255, 3, cv2.LINE_AA)

    cv2.imshow("img2", img2)
    cv2.waitKey(0)

    ir_color_dst = cv2.warpPerspective(ir_color , H , (597 , 448))
    

    dst = cv2.add(color_img , ir_color_dst)
    cv2.imshow("ir_color_dst", ir_color_dst)
    cv2.imshow("dst" , dst)
    cv2.waitKey(0)


