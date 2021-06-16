#coding:utf-8
import cv2
import os
import numpy as np
import sys

def is_inside(o , i):
    ox , oy , ow , oh = o
    ix , iy , iw , ih = i

    return ox > ix and oy > iy and ox + ow < ix + iw and oy + oh < iy + ih

def draw_person(image , person):
    x , y , w , h = person
    cv2.rectangle(image , (x , y) , (x + w , y + h) , (0 , 255 , 255) , 2)

if __name__ == '__main__':
    img = cv2.imread("aobama.jpg")
    hog = cv2.HOGDescriptor()
    hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())

    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print('open camera failture!')
        sys.exit(1)

    while True:
        ret , frame = cap.read()
        if ret:
            found , w = hog.detectMultiScale(frame)
            found_filtered = []
            print(found)

            for ri , r in enumerate(found):
                for qi , q in enumerate(found):
                    if ri != qi and is_inside(r , q):
                        break
                    else:
                        found_filtered.append(r)
            for person in found_filtered:
                draw_person(frame , person)

            cv2.imshow("people detection" , frame)
            if cv2.waitKey(25) & 0xFF == ord('q'):
                break
        else:
            break
    cv2.destroyAllWindows()


