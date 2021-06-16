#coding:utf-8

import cv2
import glob
import os
import numpy as np
import math
import json
import random



boardWidth = 6
boardHeight = 5
boardCorner = boardWidth * boardHeight
squareSize = 55
boardSize = (boardWidth , boardHeight)

objpL = np.zeros((boardHeight * boardWidth , 3) , dtype=np.float32)


class Stereo_Calibrate:
	def __init__(self , left_path , right_path , imageW = 640, imageH = 480 , boardW = 6 , boardH = 5 , squareS = 55, frameN = 20):
		self.left_path = left_path
		self.right_path = right_path
		self.imageWidth = imageW
		self.imageHeight = imageH
		self.boardWidth = boardW
		self.boardHeight = boardH
		self.squareSize = squareS
		self.frameNumber = frameN
		self.left_maps = None
		self.right_maps = None
		# 第一个相机和第二个相机之间的旋转矩阵
		self.R = None
		# 平移矩阵
		self.T = None
		# 两个相机的投影矩阵
		self.Pl = None
		self.Pr = None
		# 基础矩阵和本质矩阵
		self.F = None
		self.E = None

		self.R1 = None
		self.R2 = None

		self.cameraMatrixL = None
		self.cameraMatrixR = None
		self.cameraDistCoeffL = None
		self.cameraDistCoeffR = None

		self.imagePointL = []
		self.imagePointR = []
		self.objRealPoint = []

		self.calib_info = {}

	def readCameraParam(self , calib_params):
		load_params = {}
		with open(calib_params, 'r') as f:
			load_params = json.load(f)

		mtx = load_params["mtx"]
		dist = load_params["dist"]

		return np.squeeze(np.array([mtx])), np.squeeze(np.array([dist]))

	# 计算标定板上真实的物理坐标
	def calRealPoint(self):
		imgpoint = np.zeros((self.boardWidth * self.boardHeight, 1, 3), dtype=np.float32)
		for rowIndex in range(self.boardHeight):
			for colIndex in range(self.boardWidth):
				imgpoint[rowIndex * self.boardWidth + colIndex, 0, 0] = rowIndex * self.squareSize
				imgpoint[rowIndex * self.boardWidth + colIndex, 0, 1] = colIndex * self.squareSize
				imgpoint[rowIndex * self.boardWidth + colIndex, 0, 2] = 0

		objs = []
		for imgIndex in range(self.frameNumber):
			objs.append(imgpoint)
		return objs

	def createMap(self):

		print('cameraMatrixL:' , self.cameraMatrixL)
		print('cameraDistCoeffL:' , self.cameraDistCoeffL)
		print('R1:' , self.R1)
		print('Pl:' , self.Pl)

		self.left_maps = cv2.initUndistortRectifyMap(self.cameraMatrixL , self.cameraDistCoeffL , self.R1 , self.Pl ,
													 (self.imageWidth , self.imageHeight) , cv2.CV_16SC2)

		self.right_maps = cv2.initUndistortRectifyMap(self.cameraMatrixR, self.cameraDistCoeffR, self.R2, self.Pr,
													 (self.imageWidth, self.imageHeight), cv2.CV_16SC2)

	def stereo_rectify(self , left_img_path , right_img_path):
		left_img = cv2.imread(left_img_path)
		right_img = cv2.imread(right_img_path)

		if not self.left_maps or not self.right_maps:
			self.createMap()

		rectifyImageL = cv2.remap(left_img, self.left_maps[0], self.left_maps[1], cv2.INTER_LINEAR)
		rectifyImageR = cv2.remap(right_img, self.right_maps[0], self.right_maps[1], cv2.INTER_LINEAR)

		(height, width) = np.shape(left_img)[:2]

		rectifyBefore = np.zeros((height, 2 * width, 3), dtype=np.uint8)
		rectifyBefore[:, 0:width, :] = left_img
		rectifyBefore[:, width:, :] = right_img

		np.random.seed(0)
		for i in range(0, self.imageHeight, 32):
			cv2.line(rectifyBefore, (0, i), (2 * width, i),
			         (random.randint(0, 256), random.randint(0, 256), random.randint(0, 256)), 1, 8)

		# cv2.imshow("rectifyBefore", rectifyBefore)

		sf = 600.0 / max(width, height)
		w = int(math.floor(width * sf + 0.5))
		h = int(math.floor(height * sf + 0.5))

		canvas = np.zeros((h, w * 2, 3), dtype=np.uint8)
		# 左图画到画布上
		canvasPart = canvas[:, 0:w, :]
		canvasPart = cv2.resize(rectifyImageL, (w, h), 0, 0, cv2.INTER_AREA)

		canvas[:, 0:w, :] = canvasPart

		# 右图画到画布上
		canvasPart = canvas[:, w:, :]
		canvasPart = cv2.resize(rectifyImageR, (w, h), 0, 0, cv2.INTER_AREA)
		canvas[:, w:, :] = canvasPart

		# 画上对应的线条
		for i in range(0, canvas.shape[0], 32):
			cv2.line(canvas, (0, i), (2 * w, i),
			         (random.randint(0, 256), random.randint(0, 256), random.randint(0, 256)), 1, 8)
		# cv2.imshow("rectofied", canvas)
		# cv2.waitKey(0)

		return rectifyBefore , canvas


	def calibrate(self):
		'''
		:return: 重投影误差
		'''
		if not os.path.exists(left_path) or not os.path.exists(right_path):
			print('存储图片路径不存在，请检查！')
			return None
		if not os.path.exists(os.path.join(left_path , "cameraparam_left.json")) or not os.path.exists(os.path.join(right_path , "cameraparam_right.json")):
			print('未进行相机标定，请对单个相机进行标定！')
			return None

		cameraMatrixL, distCoeffL = self.readCameraParam(os.path.join(self.left_path, "cameraparam_left.json"))
		cameraMatrixR, distCoeffR = self.readCameraParam(os.path.join(self.right_path, "cameraparam_right.json"))

		goodFrameCount = 0
		while goodFrameCount < self.frameNumber:
			leftName = os.path.join(self.left_path, "left-%d.jpg" % (goodFrameCount))
			print(leftName)

			rgbImageL = cv2.imread(leftName)

			grayImageL = cv2.cvtColor(rgbImageL, cv2.COLOR_BGR2GRAY)

			rightName = leftName.replace("left", "right")
			rgbImageR = cv2.imread(rightName)
			grayImageR = cv2.cvtColor(rgbImageR, cv2.COLOR_BGR2GRAY)

			find_chessboard_flags = cv2.CALIB_CB_ADAPTIVE_THRESH | cv2.CALIB_CB_NORMALIZE_IMAGE | cv2.CALIB_CB_FAST_CHECK
			isFindL, cornersL = cv2.findChessboardCorners(rgbImageL, boardSize, flags=find_chessboard_flags)
			isFindR, cornersR = cv2.findChessboardCorners(rgbImageR, boardSize, flags=find_chessboard_flags)

			if isFindL and isFindR:
				criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.1)
				cv2.cornerSubPix(grayImageL, cornersL, (5, 5), (-1, -1), criteria)
				cv2.drawChessboardCorners(rgbImageL, boardSize, cornersL, isFindL)
				cv2.imshow("chessboardL", rgbImageL)
				self.imagePointL.append(cornersL)

				cv2.cornerSubPix(grayImageR, cornersR, (5, 5), (-1, -1), criteria)
				cv2.drawChessboardCorners(rgbImageR, boardSize, cornersR, isFindR)
				cv2.imshow("chessboardR", rgbImageR)
				self.imagePointR.append(cornersR)

				goodFrameCount += 1
				print("The image ", goodFrameCount, " is good")
			else:
				print('The image is bad ,pleaes try again')

			if cv2.waitKey(40) == ord('q'):
				break
		self.objRealPoint = self.calRealPoint()

		R = None
		T = None
		E = None
		F = None

		# stereocalib_retval, cameraMatrixL, distCoeffL, cameraMatrixR, distCoeffR, self.R, self.T, self.E, self.F = cv2.stereoCalibrate(
		# 	objRealPoint, imagePointL, imagePointR, cameraMatrixL, distCoeffL, cameraMatrixR, distCoeffR,
		# 	(self.imageWidth, self.imageHeight), flags=cv2.CALIB_USE_INTRINSIC_GUESS,
		# 	criteria=(cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 100, 1e-5))

		stereocalib_retval, cameraMatrixL, distCoeffL, cameraMatrixR, distCoeffR, self.R, self.T, self.E, self.F = cv2.stereoCalibrate(
			self.objRealPoint, self.imagePointL, self.imagePointR, cameraMatrixL, distCoeffL, cameraMatrixR, distCoeffR,
			(self.imageWidth, self.imageHeight), flags=cv2.CALIB_FIX_INTRINSIC,
			criteria=(cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 100, 1e-5))

		# 0 = full crop , 1 = no crop
		rectify_scale = 0
		# Pl 相机1矫正后的投影矩阵 3 * 4
		self.R1, self.R2, self.Pl, self.Pr, Q, roi1, roi2 = cv2.stereoRectify(cameraMatrixL, distCoeffL, cameraMatrixR, distCoeffR,
		                                                  (self.imageWidth, self.imageHeight), self.R, self.T, alpha=rectify_scale)

		# self.left_maps = cv2.initUndistortRectifyMap(cameraMatrixL, distCoeffL, self.R1, self.Pl, (self.imageWidth, self.imageHeight),
		#                                         cv2.CV_16SC2)
		# self.right_maps = cv2.initUndistortRectifyMap(cameraMatrixR, distCoeffR, self.R2, self.Pr, (self.imageWidth, self.imageHeight),
		#                                          cv2.CV_16SC2)

		self.cameraMatrixL = cameraMatrixL
		self.cameraMatrixR = cameraMatrixR
		self.cameraDistCoeffL = distCoeffL
		self.cameraDistCoeffR = distCoeffR

		self.createMap()



		return stereocalib_retval

	def getCalibError(self):

		err = 0
		npoints = 0
		for i in xrange(len(self.objRealPoint)):
			npt = len(self.objRealPoint[i])

			self.imagePointL[i] = cv2.undistortPoints(self.imagePointL[i] , self.cameraMatrixL , self.cameraDistCoeffL )
			self.imagePointR[i] = cv2.undistortPoints(self.imagePointR[i] , self.cameraMatrixR , self.cameraDistCoeffR )

			line_L = cv2.computeCorrespondEpilines(self.imagePointL[i] , 2 , self.F)
			line_R = cv2.computeCorrespondEpilines(self.imagePointR[i] , 1 , self.F)

			line_L = line_L.reshape(-1 , 3)
			line_R = line_R.reshape(-1 , 3)


			# # 计算误差
			for j in xrange(npt):
				err_ij = math.fabs(self.imagePointL[i][j][0][0] * line_R[j][0] + self.imagePointL[i][j][0][1] * line_R[j][1] + line_R[j][2]) + math.fabs(self.imagePointR[i][j][0][0] * line_L[j][0] + self.imagePointR[i][j][0][1] * line_L[j][1] + line_L[j][2])
				err += err_ij

			npoints += err
		# average reprojection err
		return err/npoints


	# 校正后的投影矩阵
	def getProjectRectifyMatrix(self):
		return self.Pl , self.Pr


	def getNewIntristicMatrix(self):
		return self.cameraMatrixL , self.cameraDistCoeffL , self.cameraMatrixR , self.cameraDistCoeffR

	def getRotaionTranslation(self):
		return self.R , self.T

	def getFoundationMatrix(self):
		return self.F

	def getEssentialMatrix(self):
		return self.E

	def getTranformWMatrix(self):
		'''
		计算相机1到相机2的变换矩阵
		Ar :右相机内参 4 * 4
		M  :立体标定的旋转和平移矩阵 4 * 4 [R|T]
		Al_ :左相机的内参的逆 4 * 4
		W = Ar * M * Al_
		:return: 4 x 4
		'''

		# W_matrix = np.identity(4 , dtype=np.float32)
		Ar = np.identity(4 , dtype=np.float32)
		Ar[:3 , :3] = self.cameraMatrixL
		Al = np.identity(4 , dtype=np.float32)
		Al[:3 , :3] = self.cameraMatrixL

		M = np.identity(4 , dtype=np.float32)
		M[:3,:3] = self.R
		M[:3,3:] = self.T

		print('M shape:' ,np.shape(M))

		W = np.dot(Ar , M)

		return np.dot(W , np.mat(Al).I.getA())

	def saveCalibResult(self , savefile):
		self.calib_info["cameraMatrixL"] = self.cameraMatrixL.tolist()
		self.calib_info["cameraMatrixR"] = self.cameraMatrixR.tolist()
		self.calib_info["rotation"] = self.R.tolist()
		self.calib_info["translation"] = self.T.tolist()
		self.calib_info["W"] = self.getTranformWMatrix().tolist()
		self.calib_info["F"] = self.F.tolist()
		self.calib_info["E"] = self.E.tolist()
		self.calib_info["distCoeffL"] = self.cameraDistCoeffL.tolist()
		self.calib_info["distCoeffR"] = self.cameraDistCoeffR.tolist()
		self.calib_info["R1"] = self.R1.tolist()
		self.calib_info["R2"] = self.R2.tolist()
		self.calib_info["Pl"] = self.Pl.tolist()
		self.calib_info["Pr"] = self.Pr.tolist()


		with open(savefile , "w") as f:
			json.dump(self.calib_info , f , indent=4)

	def loadCalibResuls(self , calibResult):
		camera_info = {}
		with open(calibResult , "rb") as f:
			camera_info = json.load(f)
		self.cameraMatrixL = np.array(camera_info["cameraMatrixL"])
		self.cameraMatrixR = np.array(camera_info["cameraMatrixR"])
		self.R = np.array(camera_info["rotation"])
		self.T = np.array(camera_info["translation"])
		self.cameraDistCoeffL = np.array(camera_info["distCoeffL"])
		self.cameraDistCoeffR =  np.array(camera_info["distCoeffR"])
		self.R1 = np.array(camera_info["R1"])
		self.R2 = np.array(camera_info["R2"])
		self.Pl = np.array(camera_info["Pl"])
		self.Pr = np.array(camera_info["Pr"])






if __name__ == '__main__':
	print(cv2.__version__)
	left_path = "left"
	right_path = "right"

	stereo_calib = Stereo_Calibrate(left_path , right_path , frameN=10)
	err = stereo_calib.calibrate()
	print('err:' , err)
	stereo_calib.saveCalibResult("stereo_calib_result.json")


	# ---------------------------------------
	# 立体校正
	left_img_path = "left/left-10.jpg"
	right_img_path = left_img_path.replace("left" , "right")

	rectify_before , rectified = stereo_calib.stereo_rectify(left_img_path , right_img_path)

	MatrixL , distcoeffL , MatrixR , distcoeffR = stereo_calib.getNewIntristicMatrix()
	print('-----------------------------------------------')
	print('左相机内参：')
	print(MatrixL)
	print('左相机畸变系数:')
	print(distcoeffL)
	print('-----------------------------------------------')
	print('右相机内参：')
	print(MatrixR)
	print('右相机畸变系数:')
	print(distcoeffR)
	print('-----------------------------------------------')
	R , T = stereo_calib.getRotaionTranslation()
	print('两相机旋转矩阵：')
	print(R)
	print('平移矩阵:')
	print(T)
	print('-----------------------------------------------')

	res_err = stereo_calib.getCalibError()
	print('reprojection err:' , res_err)


	cv2.imshow("before" , rectify_before)
	cv2.imshow("rectified" , rectified)
	cv2.imwrite("before.jpg" , rectify_before)
	cv2.imwrite("rectified.jpg" , rectified)
	cv2.waitKey(0)





