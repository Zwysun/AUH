import cv2
import numpy as np
from matplotlib import pyplot as plt

img = cv2.imread('./capture_letter/730ab.jpg')
#使用cvtColor转换为HSV图
out_img_HSV=cv2.cvtColor(img,cv2.COLOR_BGR2HSV)#将图片转换为灰度图
hsvChannels=cv2.split(out_img_HSV)  #将HSV格式的图片分解为3个通道

cv2.imshow('Value',hsvChannels[2]) #显示Value分量
cv2.waitKey(0)

kernel = np.array([[-1,-1,-1], [-1,9,-1], [-1,-1,-1]], np.float32) #锐化
gray = cv2.filter2D(hsvChannels[2], -1, kernel=kernel)
ret,thresh1 = cv2.threshold(gray,80,255,cv2.THRESH_BINARY_INV)
cv2.imshow("thresh", thresh1)
cv2.waitKey(0)
# thresh2 = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY_INV,25,10)
# cv2.imshow("thresh", thresh2)
# cv2.waitKey(0)
# thresh = cv2.bitwise_and(thresh1,thresh2)
# cv2.imshow("thresh", thresh)
# cv2.waitKey(0)