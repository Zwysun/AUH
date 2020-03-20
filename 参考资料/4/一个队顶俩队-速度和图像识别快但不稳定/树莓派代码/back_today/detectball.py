#!/usr/bin/env python
#coding=utf-8
import cv2
import numpy as np
import time

##2019/7/5 17:11


def detectyb(frame):

	# t1 = time.clock()
	H, W, _=frame.shape

	hue_image = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
	low_range1 = np.array([20, 124, 103])
	high_range1 = np.array([32, 255, 255])
	th1 = cv2.inRange(hue_image, low_range1, high_range1)

	# cv2.imshow("th",th1)
	# cv2.waitKey(0)
	M = cv2.moments(th1)
	#print(M["m00"]/255)
	if M["m00"]==0:
		return 1000, 1000
	cX=int(M["m10"]/M["m00"])-W/2
	cY=int(M["m01"]/M["m00"])-H/2

	# t2 = time.clock()
	# total_time = t2-t1
	# print("t1:"+str(total_time))
	return cX, cY
	


if __name__ == '__main__':

	frame0 = cv2.imread('./Debug&Config/capture_ball/0001.jpg')
	H0, W0, _=frame0.shape
	frame = cv2.resize(frame0,(640,480))
	x_error, y_error = detectyb(frame)
	print("X_error = ",x_error,"Y_error = ",y_error)

