import cv2
import requests
import time
from functools import wraps
import json
from utils.ReadVideo import FrameExtractor

url = "http://0.0.0.0:8888/person/detect/detectPerson"

def timeit_wrapper(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        start = time.perf_counter()  # Alternatively, you can use time.process_time()
        func_return_val = func(*args, **kwargs)
        end = time.perf_counter()
        print('{0:<10}.{1:<8} : {2:<8}'.format(func.__module__, func.__name__, end - start))
        return func_return_val
    return wrapper

def draw_bbox(img , results):
    for res in results:
        x1 , y1 , x2 , y2 = res
        cv2.rectangle(img, (x1, y1), (x2, y2), (0, 0 , 255))

@timeit_wrapper
def test_image(frame):
    if isinstance(frame , str):
        frame = cv2.imread(frame)

    encode_img = cv2.imencode(".jpg", frame)[1].tostring()
    res = requests.post(url , data=encode_img)
    if res.status_code == 200:
        return json.loads(res.content)['data']
    else:
        return []

def test_video(videoName):
    if isinstance(videoName , int):
        cap = cv2.VideoCapture(videoName)
    elif isinstance(videoName , str):
        cap = cv2.VideoCapture(videoName)

    while True:
        r , frame = cap.read()
        if r:
            results = test_image(frame)
            print(results)
            draw_bbox(frame, results)

        cv2.imshow("image" , frame)
        
        k = cv2.waitKey(25)
        if k == 0xFF & ord('q'):
            break

if __name__ == "__main__":
    videoName = "1584104913113595.mp4"
    # test_video(0)

    frame_rgb = FrameExtractor(rtsp = videoName)
    frame_number = frame_rgb.get_frame_number()
    frame_index = 0
    while frame_index < frame_number:
        frame = frame_rgb.get()
        print(frame.shape)
        results = test_image(frame)
        draw_bbox(frame, results)

        cv2.imshow("image" , frame)
        k = cv2.waitKey(25)
        if k == 0xFF & ord('q'):
            break



    

