
import requests
import numpy as np
import cv2


def get_image_content(url):
    try:
        r = requests.get(url)
        assert r.status_code == 200
        return r.content
    except BaseException as e:
        return None


def download_img(url):
    try:
        r = requests.get(url)
        npimg = np.fromstring(r.content, np.uint8)
        img = cv2.imdecode(npimg, cv2.IMREAD_UNCHANGED)
    except BaseException as e:
        return None
    return img


def decode_image(content):
    try:
        npimg = np.fromstring(content, np.uint8)
        img = cv2.imdecode(npimg, cv2.IMREAD_UNCHANGED)
    except BaseException as e:
        return None
    return img

def batch_decode_image(contents):
    try:
        imgs = []
        for content in contents:
            npimg = np.fromstring(content , np.uint8)
            img = cv2.imdecode(npimg , cv2.IMREAD_UNCHANGED)
            imgs.append(img)
        return imgs
    except BaseException as e:
        return None
            


def download_img_stream(url):
    try:
        r = requests.get(url)
        npimg = np.fromstring(r.content, np.uint8)
        img = cv2.imdecode(npimg, cv2.IMREAD_UNCHANGED)
    except BaseException as e:
        return None, None
    return img, r.content


def encode_png_image(img):
    try:
        s, img_encode = cv2.imencode('.jpg', img)
        if s:
            data_encode = np.array(img_encode)
            str_encode = data_encode.tostring()
        else:
            str_encode = None
    except BaseException as e:
        return None
    return str_encode
