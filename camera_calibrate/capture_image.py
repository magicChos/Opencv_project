#coding:utf-8


import os
import glob
import cv2
import shutil
import copy

camera_ids = [2 , 5]
caps = []
chess_size = (5 , 6)
CAPTURE_NUM = 20

left_path_bak = "left_path.bak"
if os.path.exists(left_path_bak):
	shutil.rmtree(left_path_bak)
else:
	os.mkdir(left_path_bak)

if os.path.isdir(left_path_bak):
	pass
else:
	os.mkdir(left_path_bak)

right_path_bak = "right_path.bak"
if os.path.exists(right_path_bak):
	shutil.rmtree(right_path_bak)
else:
	os.mkdir(right_path_bak)

if os.path.isdir(right_path_bak):
	pass
else:
	os.mkdir(right_path_bak)
	
def capture_img(cam_ids):
	for i in cam_ids:
		cap = cv2.VideoCapture(i)
		if (cap.isOpened()):
			caps.append(cap)
		else:
			print('camera % can not open!'.format(i))
	
	assert len(caps) == 2
	left_path = "left"
	right_path = "right"
	if os.path.exists(left_path):
		# os.system("mv " + left_path + "/*.jpg " + left_path_bak)
		shutil.rmtree(left_path)
	else:
		os.mkdir(left_path)
	if os.path.isdir(left_path):
		pass
	else:
		os.mkdir(left_path)
		
	if os.path.exists(right_path):
		# os.system("mv " + right_path + "/*.jpg " + right_path_bak)
		shutil.rmtree(right_path)
	else:
		os.mkdir(right_path)
		
	if os.path.isdir(right_path):
		pass
	else:
		os.mkdir(right_path)
		
	count = 0
	per_count = 0
	while count < CAPTURE_NUM:
		ret1, frame1 = caps[0].read()
		ret2, frame2 = caps[1].read()
		if not ret1 or not ret2:
			break
		
		# frame1 = cv2.flip(frame1, 1)
		# frame2 = cv2.flip(frame2, 1)
		
		frame1_src = copy.deepcopy(frame1)
		frame2_src = copy.deepcopy(frame2)
		
		gray1 = cv2.cvtColor(frame1 , cv2.COLOR_BGR2GRAY)
		gray2 = cv2.cvtColor(frame2 , cv2.COLOR_RGB2GRAY)
		
		ret1, corners1 = cv2.findChessboardCorners(gray1, chess_size, None)
		ret2, corners2 = cv2.findChessboardCorners(gray2, chess_size, None)
		
		if ret1 and ret2:
			criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)
			corners1_1 = cv2.cornerSubPix(gray1, corners1, (11, 11), (-1, -1), criteria)
			corners2_1 = cv2.cornerSubPix(gray2, corners2, (11, 11), (-1, -1), criteria)
			
			frame1 = cv2.drawChessboardCorners(frame1, chess_size, corners1_1, ret1)
			frame2 = cv2.drawChessboardCorners(frame2, chess_size, corners2_1, ret2)
			
			
			if per_count == 20:
				left_name = left_path + "/left-" + str(count) + ".jpg"
				right_name = right_path + "/right-" + str(count) + ".jpg"
				print(left_name)
				print(right_name)
				
				frame1 = cv2.cvtColor(frame1, cv2.COLOR_BGR2YCrCb)
				frame2 = cv2.cvtColor(frame2, cv2.COLOR_BGR2YCrCb)
	
				cv2.imwrite(left_name, frame2_src)
				cv2.imwrite(right_name, frame1_src)
				
				cv2.imshow("left-" + str(count), frame2)
				cv2.imshow("right-" + str(count), frame1)
				cv2.waitKey(2000)
				
				per_count = 0
				count += 1
			else:
				per_count += 1


		
		cv2.imshow("left-" + str(count), frame2)
		cv2.imshow("right-" + str(count), frame1)
		cv2.moveWindow("left-" + str(count), 300, 100)
		cv2.moveWindow("right-" + str(count), 300 + frame2.shape[1], 100)
		
		c = cv2.waitKey(40)
		if c == ord('q'):
			break
	
	for cap in caps:
		cap.release()
	

def getCameraNumber():
	count = 0
	for i in range(0 , 10):
		cap = cv2.VideoCapture(i)
		if not cap.isOpened():
			break
		cap.release()
		count = i + 1
	
	return count
	
	
	

if __name__ == '__main__':
	
	print('---------------------------------------------')
	print('starting capture...')
	capture_img(camera_ids)

	cv2.destroyAllWindows()
	print('---------------------------------------------')
	print('ending capture...')