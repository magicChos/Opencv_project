#coding:utf-8
import cv2
import os
import glob

def detectAndDisplay(img , cascade):
    gray = cv2.cvtColor(img , cv2.COLOR_BGR2GRAY)
    cv2.equalizeHist(gray , gray)

    results = cascade.detectMultiScale(gray , 1.1 , 3)
    for (x , y , w , h) in results:
        cv2.rectangle(img , (x , y) , (x + w , y + h) , (0 , 0 , 255) , 1)
    cv2.imshow("img" , img)

    return img



if __name__ == '__main__':
    cascade_model = "TrainCasecade/cascade.xml"
    img_name = "/Users/han/Desktop/67-sku/5028054e-88da-11e8-8865-17405fff1575-0-266.jpg"
    Mycascade = cv2.CascadeClassifier()


    if not Mycascade.load(cascade_model):
        print('无法加载级联分类器文件!')
    img = cv2.imread(img_name)
    if type(img) == type(None):
        print('打开图片失败！')
    ROI = detectAndDisplay(img , Mycascade)

    cv2.waitKey(0)



