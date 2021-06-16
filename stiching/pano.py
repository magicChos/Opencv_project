#coding:utf-8
import cv2
from image_sort import sortImages
import sys
from Stitch import StitchImage


if __name__ == '__main__':
    try:
        args = sys.argv[1]
    except:
        args = "/Users/han/Opencv_Project/stiching/stitchImg"
    finally:
        print(args)

    image_lsts = sortImages(args)
    print(image_lsts)

    stitch_obj = StitchImage(image_lsts , True , False)
    pano_image = stitch_obj.process()

    cv2.imwrite("pano_image.jpg" , pano_image)


