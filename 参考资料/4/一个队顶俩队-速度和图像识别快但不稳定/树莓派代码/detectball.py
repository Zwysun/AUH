#!/usr/bin/env python
#coding=utf-8
import cv2
import numpy as np
import time

##2019/7/6 20:24
#修改颜色范围（清水水池）
##2019/7/5 17:11


def detectyb(frame):

	# 
	# t1 = time.clock()
	H, W, _=frame.shape

	hue_image = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
	low_range1 = np.array([20, 124, 103])
	high_range1 = np.array([50, 255, 255])
	th1 = cv2.inRange(hue_image, low_range1, high_range1)

	kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (7, 7))
	th1 = cv2.morphologyEx(th1, cv2.MORPH_CLOSE, kernel)

	#cv2.imshow("th",th1)
	#cv2.waitKey(0)
	M = cv2.moments(th1)
	#print(M["m00"]/255)
	if M["m00"]<500*255:
		return 1000, 1000
	cX=M["m10"]/M["m00"]-W/2
	cY=M["m01"]/M["m00"]-H/2

	# t2 = time.clock()
	# total_time = t2-t1
	# print("t1:"+str(total_time))
	return int(cX), int(cY)
	


if __name__ == '__main__':

	frame0 = cv2.imread('C:/Users/liuzhongtian/Desktop/002.jpg')
	frame = cv2.resize(frame0,(640,480))
	x_error, y_error = detectyb(frame)
	print("X_error = ",x_error,"Y_error = ",y_error)

