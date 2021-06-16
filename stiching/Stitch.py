#coding:utf-8

import numpy as np
import cv2
from Match import MatchImages


class StitchImage:
    def __init__(self , img_lsts , pydown = True , eqHist = True):
        if pydown:
            self.images = [cv2.pyrDown(cv2.imread(each)) for each in img_lsts]
        else:
            self.images = [cv2.imread(each) for each in img_lsts]
        self.eq_images = []
        if eqHist:
            self.eq_images = [self.equalize_histogram_color(each) for each in self.images]
        else:
            self.eq_images = self.images

        self.count = len(img_lsts)
        self.match_obj = MatchImages()

    def process(self):
        head_img = self.images[0]
        eq_head_img = self.eq_images[0]

        # for tail_img in self.images[1:]:
        for eq_tail_img , tail_img in zip(self.eq_images[1:] , self.images[1:]):
            H = self.match_obj.match(eq_head_img , eq_tail_img , saveMatched=True)
            print('H:' , H)

            h1 , w1 = eq_head_img.shape[:2]
            h2 , w2 = eq_tail_img.shape[:2]

            # Get the canvas dimesions
            img1_dims = np.float32([[0, 0], [0, h1], [w1, h1], [w1, 0]]).reshape(-1, 1, 2)
            img2_dims_temp = np.float32([[0, 0], [0, h2], [w2, h2], [w2, 0]]).reshape(-1, 1, 2)

            # Get relative perspective of second image
            img2_dims = cv2.perspectiveTransform(img2_dims_temp, H)
            result_dims = np.concatenate((img1_dims , img2_dims) , axis=0)

            # Getting images together
            # Calculate dimensions of match points
            [x_min, y_min] = np.int32(result_dims.min(axis=0).ravel() - 0.5)
            [x_max, y_max] = np.int32(result_dims.max(axis=0).ravel() + 0.5)

            # Create output array after affine transformation
            transform_dist = [-x_min, -y_min]

            transform_array = np.array([[1, 0, transform_dist[0]],
                                        [0, 1, transform_dist[1]],
                                        [0, 0, 1]])

            eq_result_img = cv2.warpPerspective(eq_head_img , transform_array.dot(H) , (x_max - x_min , y_max - y_min))
            eq_result_img[transform_dist[1]:h2 + transform_dist[1], transform_dist[0]:w2 + transform_dist[0]] = eq_tail_img

            result_img = cv2.warpPerspective(head_img, transform_array.dot(H), (x_max - x_min, y_max - y_min))
            result_img[transform_dist[1]:h2 + transform_dist[1],transform_dist[0]:w2 + transform_dist[0]] = tail_img

            eq_head_img = eq_result_img
            head_img = result_img

            # cv2.imshow("head_img" , head_img)
            # cv2.waitKey(0)


        return head_img

    # Equalize Histogram of Color Images
    def equalize_histogram_color(self,img):
        img_yuv = cv2.cvtColor(img, cv2.COLOR_BGR2YUV)
        img_yuv[:, :, 0] = cv2.equalizeHist(img_yuv[:, :, 0])
        img = cv2.cvtColor(img_yuv, cv2.COLOR_YUV2BGR)
        return img



