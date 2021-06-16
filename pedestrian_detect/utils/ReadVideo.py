import cv2
import threading
import time

class FrameExtractor(object):
    """
    抽帧类
    """

    def __init__(self, rtsp):
        self._src = rtsp
        self._vc = cv2.VideoCapture(rtsp)
        self._frame_index = 0
        self._frame_number = self._vc.get(cv2.CAP_PROP_FRAME_COUNT)
        self._fps = self._vc.get(cv2.CAP_PROP_FPS)

    def is_opened(self):
        """

        :return:
        """
        return self._vc.isOpened()

    def get_frame_number(self):
        return self._frame_number

    def get_current_frame_index(self):
        return self._frame_index

    def _restart(self , frame_index):
        self._vc = cv2.VideoCapture(self._src)
        self._vc.set(cv2.CAP_PROP_POS_FRAMES , frame_index)

    def get(self):
        ok , frame = self._vc.read()
        if ok:
            self._frame_index += 1
            return frame
        else:
                self._restart(self._frame_index)
                self.get()

