# 利用相机的左右两张图计算单应矩阵
# 一般步骤包括如下几步
# 1 提取特征点
# 2 计算特征描述子
# 3 特征点匹配
# 4 通过RANSAC算法剔除错误匹配
# 5 计算单应矩阵

import cv2
import numpy as np

click_pt = None
click_flag = False
def draw_circle(event , x , y , flags , param):
    global img1 , img2 , H , click_pt , click_flag
    if event == cv2.EVENT_LBUTTONDOWN:
        click_pt = [x , y]
        cv2.circle(img1 , (x , y) , 10 , (255,0,0) , -1)
        click_flag = True

    if click_pt != None and click_flag:
        pts = np.float32([click_pt]).reshape(-1,1,2)
        dst = cv2.perspectiveTransform(pts , H)[0][0]
        print('dst: ' , dst)
        cv2.circle(img2 , tuple(dst) , 10 , (0 , 0 , 255) , 1)

        click_pt = None
        click_flag = False

if __name__ == "__main__":
    imageName1 = "left.jpeg"
    imageName2 = "right.jpeg"

    img1 = cv2.imread(imageName1)
    img2 = cv2.imread(imageName2)

    surf = cv2.xfeatures2d.SURF_create()
    kp1 , des1 = surf.detectAndCompute(img1 , None)
    kp2 , des2 = surf.detectAndCompute(img2 , None)

    # 匹配特征点描述子
    bf = cv2.BFMatcher()
    matches = bf.knnMatch(des1, des2 , k =2)
    
    # 提取较好的特征点
    good = []
    for m , n in matches:
        if m.distance < 0.7 * n.distance:
            good.append(m)

    # 通过特征点计算你单应性矩阵
    src_pts = np.float32([kp1[m.queryIdx].pt for m in good]).reshape(-1 , 1 , 2)
    dst_pts = np.float32([kp2[m.trainIdx].pt for m in good]).reshape(-1 , 1 , 2)

    H , mask = cv2.findHomography(src_pts , dst_pts , cv2.RANSAC , 5.0)
    matchesMask = mask.ravel().tolist()
    print(matchesMask)

    # 使用单应性矩阵计算变换结果并绘图
    h , w, d = img1.shape
    pts = np.float32([[0,0], [0,h-1], [w-1,h-1], [w-1,0]]).reshape(-1,1,2)
    dst = cv2.perspectiveTransform(pts , H)

    # img2 = cv2.polylines(img2 , [np.int32(dst)] , True , 255 , 3 , cv2.LINE_AA)

    # draw_params = dict(matchColor = (0,255,0), # draw matches in green color
    #                singlePointColor = None,
    #                matchesMask = matchesMask, # draw only inliers
    #                flags = 2)

    # img3 = cv2.drawMatches(img1 , kp1 , img2 , kp2 , good , None , **draw_params)
    # cv2.imwrite('save.jpg' , img3)

    cv2.namedWindow("image")
    cv2.setMouseCallback("image" , draw_circle)

    cv2.namedWindow("image2")
    while True:
        cv2.imshow("image" , img1)
        cv2.imshow("image2" , img2)
        if cv2.waitKey(20) & 0xFF == 27:
            break
    cv2.destroyAllWindows()

