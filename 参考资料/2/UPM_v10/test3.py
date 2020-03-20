import cv2
from matplotlib import pyplot as plt
import numpy as np
import light2line_v2
import  time
# 检测灯带的测试文件
#img3=cv2.imread('pic_PI/line10.jpg')
#img3=cv2.imread('../pic_v5/aa630rot_v3.jpg')
img3=cv2.imread('../light_pic/bb655rot_v3.jpg')
imgplt= cv2.cvtColor(img3,cv2.COLOR_BGR2RGB)
img = cv2.cvtColor(img3,cv2.COLOR_BGR2GRAY)

cv2.imshow('im2',img)

img = cv2.GaussianBlur(img, (3, 3), 0)
'''测试由灰度直方图获取二值化阈值'''
hist=cv2.calcHist([img],[0],None,[256],[0,255])
print('hist.shape=',hist.shape)
hist2=hist[220:255,:]
th2=np.argmax(hist2)-1+220-2
print('th2=',th2)
_,im_b = cv2.threshold(img,th2,255,cv2.THRESH_BINARY)
#th,im_b=cv2.threshold(img, 0, 255, cv2.THRESH_OTSU)
cv2.imshow('th',im_b)



'''测试用霍夫变换得到两条直线'''
_, _, _, centroids = cv2.connectedComponentsWithStats(im_b.copy())
#print('cenroids.shape=', centroids.shape)
xc = centroids[:, 0]
yc = centroids[:, 1]
xc = np.mat(xc).T
yc = np.mat(yc).T

rho1,theta1,rho2,theta2,rho_all,index1,index2,var_rho=light2line_v2.hough_2(xc,yc)
print('rho1=',rho1,'index1=',index1,'theta1=',theta1*180/np.pi)
print('rho2=',rho2,'index2=',index2,'theta2=',theta2*180/np.pi)


plt.figure(3)
for i in range(len(xc)):
    plt.plot(rho_all[i,:].T,'k-')


print(rho_all[1,:].shape)

vot=np.zeros((800,270))
#tStart = time.clock()
for i in range(len(xc)):
    for j in range(270):
        if rho_all[i,j] >0:
            index=rho_all[i,j].astype(np.int)
            vot[index,j]+=1




print('vot.shape=',vot.shape)
plt.figure(4)
plt.pcolor(vot)
plt.colorbar()

max_index = np.where(vot == np.max(vot))
index_rho_1 = max_index[0][0]
index_theta_1 = max_index[1][0]
print('rho_1=index_rho_1=',index_rho_1,'index_theta_1=',index_theta_1)
vot[index_rho_1,index_theta_1] = 0

while 1:
    max_index = np.where(vot == np.max(vot))
    if np.abs(index_rho_1 - max_index[0][0]) <10 :#and np.abs(index_theta_1 -max_index[1][0]) <10:
        vot[max_index[0][0], max_index[1][0]] = 0
    else:
        index_rho_2 = max_index[0][0]
        index_theta_2 = max_index[1][0]
        break

print('rho_2=index_rho_2=',index_rho_2,'index_theta_2=',index_theta_2)
plt.figure(5)
plt.imshow(imgplt)#,cmap='gray')
theta_all = np.linspace(-np.pi / 2, np.pi, 270)
xx=range(399,402,1)
yy = (index_rho_1 - xx * np.cos(theta_all[index_theta_1])) / np.sin(theta_all[index_theta_1])
plt.plot(xx, yy, 'b-', label='ss')
xx=range(10,100,1)
yy = (index_rho_2 - xx * np.cos(theta_all[index_theta_2])) / np.sin(theta_all[index_theta_2])
plt.plot(xx, yy, 'b-', label='ss')



plt.show()
cv2.waitKey(0)