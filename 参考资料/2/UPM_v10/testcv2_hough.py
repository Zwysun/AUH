import cv2
from matplotlib import pyplot as plt
import numpy as np
import light2line_v2

img3=cv2.imread('pic_PI/line10.jpg')

img = cv2.cvtColor(img3,cv2.COLOR_BGR2GRAY)

cv2.imshow('im3',img3)

img = cv2.GaussianBlur(img, (3, 3), 0)

img_hsv = cv2.cvtColor(img3,cv2.COLOR_BGR2HSV)
img_s = img_hsv[:,:,2]
cv2.imshow('im_h',img_s)
th_ostu,im_b = cv2.threshold(img_s,0,255,cv2.THRESH_OTSU)
cv2.imshow('imb',im_b)
kernelX = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (30, 30))
img_de = cv2.erode(im_b, kernelX, anchor=(-1, -1), iterations=4)
cv2.imshow('im_de',img_de)
'''测试由灰度直方图获取二值化阈值
hist=cv2.calcHist([img],[0],None,[256],[0,255])
print('hist.shape=',hist.shape)
hist2=hist[220:255,:]
th2=np.argmax(hist2)-1+220-2
print('th2=',th2)
_,im_b = cv2.threshold(img,th2,255,cv2.THRESH_BINARY)
#th,im_b=cv2.threshold(img, 0, 255, cv2.THRESH_OTSU)
cv2.imshow('th',im_b)

plt.figure(1)
plt.plot(hist)

测试两种不同获取xcyc方法经过霍夫变换后得到的直线
rho,ang,dis,imb,theta = light2line_v2.get_ang_dis_HT_v2(img)


测试用霍夫变换得到两条直线
_, _, _, centroids = cv2.connectedComponentsWithStats(im_b.copy())
#print('cenroids.shape=', centroids.shape)
xc = centroids[:, 0]
yc = centroids[:, 1]
imxy = np.zeros((640,480),np.uint8)
for i in range(len(xc)):
    imxy[yc[i].astype(np.int),xc[i].astype(np.int)]=255


lines = cv2.HoughLines(imxy,1,np.pi/180,4)
lines1 = lines[:,0,:]
plt.figure(2)
plt.imshow(imb,cmap='gray')
xx=range(160,200,1)
for rho,theta in lines1[:]:
    yy = (rho - xx * np.cos(theta)) / np.sin(theta)
    plt.plot(xx, yy, 'r-', label='ss')

print(type(imxy),imxy.shape)
print(type(im_b),imb.shape)
cv2.imshow('imxy',imxy)
print(lines)
plt.show()
'''
cv2.waitKey(0)