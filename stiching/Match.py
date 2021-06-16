#coding:utf-8

import cv2
import numpy as np


class MatchImages:
    def __init__(self):
        self.surf = cv2.xfeatures2d.SURF_create()
        self.bf = cv2.BFMatcher()
        self.count = 0

    def match(self , img1 , img2 , verify_ratio = 0.6 , saveMatched = False):
        imageSet1 = self.getSurfFeatures(img1)
        imageSet2 = self.getSurfFeatures(img2)

        matches1 = self.bf.knnMatch(imageSet1['des'] ,imageSet2['des'] , 2)
        matches2 = self.bf.knnMatch(imageSet2['des'] , imageSet1['des'] , 2)

        verified_matches1 = []
        verified_matches2 = []
        for m1 , m2 in matches1:
            if m1.distance <  verify_ratio * m2.distance:
                verified_matches1.append(m1)

        for m1 , m2 in matches2:
            if m1.distance <  verify_ratio * m2.distance:
                verified_matches2.append(m1)

        # symmetry test
        verified_matches = []
        for ver1 in verified_matches1:
            for ver2 in verified_matches2:
                if ver1.queryIdx == ver2.trainIdx and  ver1.trainIdx == ver2.queryIdx:
                    verified_matches.append(ver1)
        if len(verified_matches) > 8:
            img1_pts = []
            img2_pts = []
            for match in verified_matches:
                img1_pts.append(imageSet1['kp'][match.queryIdx].pt)
                img2_pts.append(imageSet2['kp'][match.trainIdx].pt)
            img1_pts = np.float32(img1_pts).reshape(-1 , 1 , 2)
            img2_pts = np.float32(img2_pts).reshape(-1 , 1 , 2)

            F , mask = cv2.findFundamentalMat(img1_pts , img2_pts , cv2.RANSAC , 2)
            img1_pts_inlier = img1_pts[mask.ravel() == 1]
            img2_pts_inlier = img2_pts[mask.ravel() == 1]

            if saveMatched:
                index = np.where(mask.ravel() == 1)
                best_matches = [verified_matches[i] for i in index[0]]

                outImg = None
                outImg = cv2.drawMatches(img1, imageSet1['kp'], img2, imageSet2['kp'], best_matches, img2, flags=2)
                cv2.imwrite(str(self.count) + (".jpg") , outImg)
                print("image name:" , str(self.count) + (".jpg"))
                self.count += 1



            H , mask = cv2.findHomography(img1_pts_inlier , img2_pts_inlier , cv2.RANSAC , 2.0)

            return H
        else:
            print('Error , not enough matches')
            exit()

    def getSurfFeatures(self , im):
        kp , des = self.surf.detectAndCompute(im , None)
        return {'kp':kp , 'des':des}
