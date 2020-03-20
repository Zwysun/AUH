import cv2
from matplotlib import pyplot as plt
import numpy as np
import light2line_v2
import  time
#img3=cv2.imread('pic_PI/line10.jpg')
img3=cv2.imread('../pic_v5/aa660rot_v3.jpg')
imgplt= cv2.cvtColor(img3,cv2.COLOR_BGR2RGB)
img = cv2.cvtColor(img3,cv2.COLOR_BGR2GRAY)
'''测试由灰度直方图获取二值化阈值'''
hist=cv2.calcHist([img],[0],None,[256],[0,255])
print('hist.shape=',hist.shape)
hist2=hist[220:255,:]
th2=np.argmax(hist2)-1+220-2
print('th2=',th2)
_,im_b = cv2.threshold(img,th2,255,cv2.THRESH_BINARY)
#th,im_b=cv2.threshold(img, 0, 255, cv2.THRESH_OTSU)
cv2.imshow('th',im_b)
'''
plt.figure(1)
plt.plot(hist)
'''
'''测试两种不同获取xcyc方法经过霍夫变换后得到的直线'''
rho,ang,dis,imb,theta = light2line_v2.get_ang_dis_HT_v2(img)


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

rho_v3,theta_v3 = light2line_v2.hough_v3(xc,yc)
print('inv3:',rho_v3,theta_v3*180/np.pi)


plt.figure(3)
for i in range(len(xc)):
    plt.plot(rho_all[i,:].T,'k-')


print(rho_all[1,:].shape)
'''
plt.figure(4)
plt.plot(var_rho.T)
'''
vot=np.zeros((800,270))
vot2=np.zeros((800,270))
#tStart = time.clock()
for i in range(len(xc)):
    for j in range(270):
        if rho_all[i,j] >0:
            index=rho_all[i,j].astype(np.int)
            vot[index,j]+=1


#tend = time.clock()
#print('vot1time=',tend-tStart)

#tStart = time.clock()
for i in range(len(xc)):
    index_p=rho_all[i,:]>0
    temp=rho_all[i,:]
    temp2=np.multiply(temp , index_p)
    vot2[temp2.astype(np.int)]+=1 # rho_all 中第i,j个元素是几就在vot的第几行第j列+1，所以rho的分辨率是1
                                        # 如果要更加精细的分辨率，则需要更复杂的算法
'''
#tend = time.clock()
#print('vot2time=',tend-tStart)

print('temp2.shape=',temp2.shape)
print('temp2=',temp2)
print('index_p.shape=',index_p.shape)


for i in range(len(xc)):
    for j in range(270):
        if rho_all[i,j] >0:
            index=rho_all[i,j].astype(np.int)
            vot[index,j]+=1
print(vot2,'vot2')
print(np.sum(np.min(vot-vot2)))
'''


print('vot.shape=',vot.shape)
max_index = np.where(vot == np.max(vot))
print('rho_i=index_rho_i=',max_index[0][0],'index_theta_i=',max_index[1][0])

vot_clum =vot.reshape(800*270,order='F')
print(vot_clum.shape)
indexall=np.argpartition(vot_clum, -2)[-2:]
print('indexall.shape=',indexall.shape,'indexall=',indexall)
theta_all = np.linspace(-np.pi / 2, np.pi, 270)
plt.figure(2)
plt.imshow(imgplt)#,cmap='gray')
xx=range(375,385,1)
for i in range(len(indexall)):
    index_thetai, index_rhoi = divmod(indexall[i], vot.shape[0])
    print('rho_i=indexrho_i=',index_rhoi , 'index_theta_i=',index_thetai )
    rhoi=index_rhoi
    thetai=theta_all[index_thetai]
    yy = (rhoi - xx * np.cos(thetai)) / np.sin(thetai)
    plt.plot(xx, yy, 'b-', label='ss')



'''    
index_theta1,index_rho1=divmod(indexall[0], vot.shape[0])
index_theta2,index_rho2=divmod(indexall[1], vot.shape[0])
print(index_rho1*2,index_theta1*2)
print(index_rho2*2,index_theta2*2)
#print(rho_all[index_rho,index_theta])

rho1=index_rho1*2
rho2=index_rho2*2
theta1=theta_all[index_theta1*2]
theta2=theta_all[index_theta2*2]


yy=(rho1-xx*np.cos(theta1))/np.sin(theta1)
plt.plot(xx,yy,'r-',label='ss')
xx2=xx
yy2=(rho2-xx*np.cos(theta2))/np.sin(theta2)
plt.plot(xx2,yy2,'b-',label='ss')
'''
plt.figure(5)
plt.pcolor(vot)
plt.colorbar()
plt.figure(6)
plt.pcolor(vot2)
plt.colorbar()
'''
cv2.imshow('ss',im_vot)
fig = plt.figure()
ax = Axes3D(fig)
ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap='rainbow')
ax=plt.subplot(111,projection='3d')
ax.scatter(X, Y, Z ,c="r")
'''

plt.show()

'''
plt.figure(1)
aa,_,_=plt.hist(img.ravel(),256)
print(aa.shape)
aa2=aa[220:255]
th2=np.argmax(aa2)-1

print('aa2=',aa2)
print('aa2.shape=',aa2.shape,'th2=',th2)
print('aa.shape=',aa.shape)
h_avg=np.array([1,1,1,1,1])/5
print(h_avg.shape)
aa_avg=np.convolve(aa,h_avg,mode='same')
print(aa_avg.shape)
plt.figure(2)
plt.plot(aa2)
plt.figure(3)
plt.plot(aa)
plt.show()
'''
cv2.waitKey(0)