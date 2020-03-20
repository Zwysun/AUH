import cv2
from matplotlib import pyplot as plt
import numpy as np
#from skimage import measure
# 腐蚀膨胀所用到的核
img3=cv2.imread('pic_PI/line8.jpg')
img3T=img3.T
print(np.shape(img3T))
img = cv2.cvtColor(img3,cv2.COLOR_BGR2GRAY)
#img=img.T
#th1,_=cv2.threshold(img, 0, 255, cv2.THRESH_OTSU)
#img[img<th1]=0
cv2.imshow('im2',img)
'''
plt.figure(1)
aa,_,_=plt.hist(img.ravel(),256)
print('aa=',aa)
thi=0
for i in range(len(aa)):
    if aa[-i-1]!=0 and aa[-i]/aa[-i-1] >100:
        thi=255-i-1
        break


print('thi=',thi)
cv2.imshow('img',img)
'''
img = cv2.GaussianBlur(img, (3, 3), 0)


th, im_b = cv2.threshold(img,230,255,cv2.THRESH_BINARY)
#th,im_b=cv2.threshold(img, 0, 255, cv2.THRESH_OTSU)

print('Otsu_th=',th)
cv2.imshow('th',im_b)
[h,w]=np.shape(im_b)
print(h,w,h*w)
contours, hier = cv2.findContours(im_b.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
if len(contours)==0 or len(np.shape(contours))!=1:
    print('error')
    cv2.waitKey(0)
    exit(1)
cv2.drawContours(img3,contours,-1,(255,0,0),2)
cv2.imshow('img',img3)
print(np.shape(contours))
print(contours)
con=[]
xc=[]
yc=[]
for i in range(len(contours)):
    con.append(np.squeeze(contours[i]))

print('shape of con',np.shape(con))
for c in con:
    if len(np.shape(c))>=2:
        xc.append(np.sum(c[:,0])/np.shape(c)[0])
        yc.append(np.sum(c[:,1])/np.shape(c)[0])

xc=np.transpose(np.mat(xc))
yc=np.transpose(np.mat(yc))
assert np.shape(xc)==(len(xc),1)
A=np.hstack((xc,np.ones((len(xc),1))))
print(A)
assert np.shape(A)==(len(xc),2)

#kt=np.linalg.inv(np.dot(A.T,A))#.I
kt=((A.T*A).I)*A.T*yc
print(kt)
'''
xx=np.arange(200, 600, 1)
yy=kt[0,0]*xx+kt[1,0]
'''
print('xc_min=',np.min(xc))
xx=[xc[1,0],xc[-1,0]]
yy=[kt[0,0]*xx[0]+kt[1,0],kt[0,0]*xx[1]+kt[1,0]]
point1=(int(np.min(xc)),int(kt[0,0]*np.min(xc)+kt[1,0]))
point2=(int(np.max(xc)),int(kt[0,0]*np.max(xc)+kt[1,0]))
print(yy)
img4=cv2.line(img3, point1, point2, (0,255,0),2)
cv2.imshow('ssa',img4)
plt.figure(2)
plt.plot(xc,yc,'bo',label="point")
plt.imshow(im_b,cmap='gray')
plt.plot(xx,yy,'r-',label='ss')

_, labels, stats, centroids = cv2.connectedComponentsWithStats(im_b.copy())
print('cenroids.shape=',centroids.shape)
xc2=centroids[:,0]
xc2=np.mat(xc2).T
yc2=centroids[:,1]
yc2=np.mat(yc2).T
print('xc2.shape',xc2.shape,'lenxc2=',len(xc2))
plt.figure(3)
plt.plot(xc2,yc2,'bo',label="xc2yc2")
plt.imshow(im_b,cmap='gray')

plt.show()

'''
index=(im_b==255)
x_sum=np.sum(index,axis=0) # 把x_sum中不为0的下标加起来，除以h得到
print(np.shape(x_sum))
xc=[]
x_index=np.argwhere(x_sum>6)
x_index=np.squeeze(x_index)
x_center=np.sum(x_index)/np.shape(x_index)[0]
print(x_center)
print('x_index=',np.shape(x_index))
print('diff_x_index=',np.diff(x_index,axis=0))
x_edge=np.argwhere(np.diff(x_index,axis=0)>1)
for i in range(0,np.shape(x_edge)[0],2):
    xc.append((x_index[x_edge[i,0]]+x_index[x_edge[i+1,0]])/2)
print('xc=',xc)
print('x_edge',x_edge)

y_sum=np.sum(index,axis=1)
y_index=np.argwhere(y_sum>6)
y_center=np.sum(y_index)/np.shape(y_index)[0]
yc=[]
y_edge=np.argwhere(np.diff(y_index,axis=0)>1)
print('shape(y_edge)=',np.shape(y_edge))
for i in range(0,np.shape(y_edge)[0]-1,2):
    yc.append((y_edge[i,0]+y_edge[i+1,0])/2)


print('yc=',yc)
fig = plt.figure(2)
distance= ((x_center-320)**2 + (y_center-240)**2)**0.5
print(distance)
plt.plot(x_center,y_center,'ro',label="point")
yc.append(0)
plt.plot(xc,yc,'bo',label="point")
plt.imshow(im_b,cmap='gray')

#labels=measure.label(data,connectivity=2)

plt.show()
'''
cv2.waitKey(0)