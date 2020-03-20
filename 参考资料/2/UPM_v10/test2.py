import cv2
import numpy as np
#import pic2abc
from matplotlib import pyplot as plt
# 检测黄球的测试文件
img3 = cv2.imread('../pic_v5/aa430rot_v3.jpg')
img_b = img3[:,:,0]
img_g = img3[:,:,1]
img_r = img3[:,:,2]

img_ob = 255-img_b
th_ob,img_ob = cv2.threshold(img_ob,0,255,cv2.THRESH_OTSU)
_, _, stats, centroids = cv2.connectedComponentsWithStats(img_ob.copy())
area = stats[1:-1,4]
index=np.argmax(area)+1 # 第一大的是整张图，所以要找第二大的
print('stats.shape=',stats.shape)
print(area)
print(centroids[index,:])
xx=centroids[index,0]
yy=centroids[index,1]
distance = ((xx-320)**2 + (yy-240)**2)**0.5
print('distance=',distance)
plt.figure(1)
plt.imshow(img_ob,cmap='gray')
plt.plot(xx,yy,'bo')
#_,img_g = cv2.threshold(img_g,120,255,cv2.THRESH_BINARY)
#_,img_r = cv2.threshold(img_r,120,255,cv2.THRESH_BINARY)

cv2.imshow('img_ob'+str(th_ob),img_ob)
#img3_b = img_b<80 and img_r >120
#img3_b *=255
#img3_b[(img_b<80) and (img_r >120) and (img_g >120)] =255
img3_b = img_b*0
img3_b_index = np.logical_and( img_b<80, img_r >100)
img3_b_index = np.logical_and(img_g >100,img3_b_index)
img3_b[img3_b_index] = 255
cv2.imshow('img_b',img_b)
cv2.imshow('img_g',img_g)
cv2.imshow('img-r',img_r)

cv2.imshow('img3_b',img3_b)
_, _, stats, centroids = cv2.connectedComponentsWithStats(img3_b.copy())
area = stats[1:-1,4]
print('stats.shape=',stats.shape)
print('area.shape=',area.shape)
if stats.shape[0] ==1:
    pass
index=np.argmax(area)+1
print(centroids[index,:])
xx=centroids[index,0]
yy=centroids[index,1]
distance = ((xx-320)**2 + (yy-240)**2)**0.5
print('distance=',distance)

plt.figure(2)
plt.imshow(img3_b,cmap='gray')
plt.plot(centroids[index,0],centroids[index,1],'bo')

cv2.imshow('img3',img3)
img_s = cv2.cvtColor(img3,cv2.COLOR_BGR2HSV)[:,:,1]
th_sb,img_s_b = cv2.threshold(img_s,0,255,cv2.THRESH_OTSU)
cv2.imshow('img_s',img_s)
cv2.imshow('img_sb'+str(th_sb),img_s_b)

hist=cv2.calcHist([img_s],[0],None,[256],[0,255])

img = cv2.cvtColor(img3,cv2.COLOR_BGR2GRAY)
#im_binary[im_binary > 60] = 255
 #   im_binary[im_binary <= 60] = 0

_,im_h_b = cv2.threshold(img_s,200,255,cv2.THRESH_BINARY)
cv2.imshow('im_h_b',im_h_b)
plt.show()
cv2.waitKey(0)
cv2.destroyAllWindows()

