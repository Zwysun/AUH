import cv2
import numpy as np
import math
from matplotlib import pyplot as plt
def getkt(img_gray):
    _, im_b = cv2.threshold(img_gray, 200, 255, cv2.THRESH_BINARY)
    # 阈值应该是在灰度直方图上找最后一个峰，比如在200~255范围内找一个峰值，然后以峰值对应灰度减5作为阈值
    contours, _ = cv2.findContours(im_b.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    if len(contours) == 0 or len(np.shape(contours)) != 1:
        return 0,0,(320,240),(320,240),im_b
    con = []
    xc = []
    yc = []
    for i in range(len(contours)):
        con.append(np.squeeze(contours[i]))

    for c in con:
        if len(np.shape(c)) >= 2:
            xc.append(np.sum(c[:, 0]) / np.shape(c)[0])
            yc.append(np.sum(c[:, 1]) / np.shape(c)[0])

    xc = np.transpose(np.mat(xc))
    yc = np.transpose(np.mat(yc))
    assert np.shape(xc) == (len(xc), 1)
    A = np.hstack((xc, np.ones((len(xc), 1))))
    assert np.shape(A) == (len(xc), 2)
    kt = ((A.T * A).I) * A.T * yc
    point1 = (int(np.min(xc)), int(kt[0, 0] * np.min(xc) + kt[1, 0]))
    point2 = (int(np.max(xc)), int(kt[0, 0] * np.max(xc) + kt[1, 0]))
    return kt[0,0],kt[1,0],point1,point2,im_b

def get_angle_distance(img_gray):
    _, im_b = cv2.threshold(img_gray, 200, 255, cv2.THRESH_BINARY)
    # 阈值应该是在灰度直方图上找最后一个峰，比如在200~255范围内找一个峰值，然后以峰值对应灰度减5作为阈值
    contours, _ = cv2.findContours(im_b.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    if len(contours) == 0 or len(np.shape(contours)) != 1:
        return 0,0,im_b
    con = []
    xc = []
    yc = []
    for i in range(len(contours)):
        con.append(np.squeeze(contours[i]))

    for c in con:
        if len(np.shape(c)) >= 2:
            xc.append(np.sum(c[:, 0]) / np.shape(c)[0])
            yc.append(np.sum(c[:, 1]) / np.shape(c)[0])

    xc = np.transpose(np.mat(xc))
    yc = np.transpose(np.mat(yc))
    xcc=np.sum(xc)/len(xc)
    ycc=np.sum(yc)/len(yc)
    distance=((xcc-240)**2+(ycc-320)**2)**0.5
    assert np.shape(xc) == (len(xc), 1)
    A = np.hstack((xc, np.ones((len(xc), 1))))
    assert np.shape(A) == (len(xc), 2)
    kt = ((A.T * A).I) * A.T * yc
    alpha = math.atan(kt[0,0])
    if alpha > 0:
        theta = math.pi - alpha
    else:
        theta = -alpha - math.pi
    return theta,distance,im_b


def get_ang_dis_HT(img_gray):
    # 返回值ang是由theta变过去的，要话直线检测对不对的话，加一个返回值theta
    _, im_b = cv2.threshold(img_gray, 230, 255, cv2.THRESH_BINARY)
    # 阈值应该是在灰度直方图上找最后一个峰，比如在200~255范围内找一个峰值，然后以峰值对应灰度减5作为阈值
    contours, _ = cv2.findContours(im_b.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    if len(contours) == 0 or len(np.shape(contours)) != 1:
        return 0,0,im_b
    con = []
    xc = []
    yc = []
    for i in range(len(contours)):
        con.append(np.squeeze(contours[i]))

    for c in con:
        if len(np.shape(c)) >= 2:
            xc.append(np.sum(c[:, 0]) / np.shape(c)[0])
            yc.append(np.sum(c[:, 1]) / np.shape(c)[0])

    xc = np.transpose(np.mat(xc))
    yc = np.transpose(np.mat(yc))

    xcc=np.sum(xc)/len(xc)
    ycc=np.sum(yc)/len(yc)
    distance=((xcc-240)**2+(ycc-320)**2)**0.5

    rho,theta=hough(xc,yc)

    if theta<np.pi/2:
        ang=theta
    elif np.pi/2<=theta and theta<=np.pi:
        ang=theta-np.pi
    else:
        ang = 20

    return rho,ang,distance,im_b


def hough(xc,yc):
    #assert len(np.shape(xc))==1  # xc is array,i.e., np.shape(xc)==(12,)
    L=len(xc)
    PI=np.pi
    theta = np.linspace(-PI / 2, PI, 270)
    theta = np.mat(theta)
    assert theta.shape == (1,270)
    assert xc.shape == (L,1)
    rho = xc*np.cos(theta)+yc*np.sin(theta)
    var_rho=np.var(rho,axis=0)
    assert var_rho.shape == (1,270)
    theta_index=np.argmin(var_rho)
    rho_c=np.mean(rho[:,theta_index])
    return rho_c,theta[0,theta_index]



'''
img3=cv2.imread('pic_PI/line3.jpg')
img = cv2.cvtColor(img3,cv2.COLOR_BGR2GRAY)
cv2.imshow('img',img)

rho,ang,dis,imb=get_ang_dis_HT(img)
cv2.imshow('imb',imb)
print(rho,ang/np.pi*180,dis)
plt.figure(1)
plt.imshow(imb,cmap='gray')
xx=range(12,370,1)
yy=(rho-xx*np.cos(ang))/np.sin(ang)
plt.plot(xx,yy,'r-',label='ss')
plt.show()
cv2.waitKey(0)

'''
'''
xc=np.ones((12,1))
PI=np.pi
theta=np.linspace(-PI/2,PI,270)
theta=np.mat(theta)
print(theta.shape)
#print(theta[0,2])
a=xc*np.cos(theta)
print(np.shape(a))
var=np.var(a,axis=0)
print(var.shape)
mm=np.argmax(var)
print(np.shape(a[:,53]))
mu=np.mean(a[:,53])
print(mu)
'''
