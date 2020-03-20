import cv2
import numpy as np
import math
from matplotlib import pyplot as plt



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
    distance=((xcc-320)**2+(ycc-240)**2)**0.5

    rho,theta=hough(xc,yc)

    if theta<np.pi/2:
        ang=theta
    elif np.pi/2<=theta and theta<=np.pi:
        ang=theta-np.pi
    else:
        ang = 20

    return rho,ang,distance,im_b,theta

def get_ang_dis_HT_v2(img_gray):
    # 返回值ang是由theta变过去的，要话直线检测对不对的话，加一个返回值theta
    hist = cv2.calcHist([img_gray], [0], None, [256], [0, 255])
    hist2 = hist[220:255, :]
    th = np.argmax(hist2) - 1 + 220 - 2

    _, im_b = cv2.threshold(img_gray, th, 255, cv2.THRESH_BINARY)
    # 阈值应该是在灰度直方图上找最后一个峰，比如在200~255范围内找一个峰值，然后以峰值对应灰度减5作为阈值
    _, _, _, centroids = cv2.connectedComponentsWithStats(im_b.copy())
    #print('cenroids.shape=', centroids.shape)
    xc = centroids[:, 0]
    yc = centroids[:, 1]
    xc = np.mat(xc).T
    yc = np.mat(yc).T
    xcc = np.sum(xc) / len(xc)
    ycc = np.sum(yc) / len(yc)
    distance=((xcc-240)**2+(ycc-320)**2)**0.5

    rho,theta=hough(xc,yc)

    if theta<np.pi/2:
        ang=-theta
    elif np.pi/2<=theta and theta<=np.pi:
        ang=np.pi-theta
    else:
        ang = 20

    return rho,ang,distance,im_b,theta



def hough(xc,yc):

    L=len(xc)
    PI=np.pi
    theta = np.linspace(-PI / 2, PI, 270)
    theta = np.mat(theta)
    assert theta.shape == (1,270)
    assert xc.shape == (L,1)
    rho = xc*np.cos(theta)+yc*np.sin(theta)
    var_rho=np.var(rho,axis=0)
    assert var_rho.shape == (1,270)
    np.argpartition(var_rho, 1)
    theta_index=np.argmin(var_rho)
    rho_c=np.mean(rho[:,theta_index])
    return rho_c,theta[0,theta_index]


def hough_2(xc,yc):

    L=len(xc)
    PI=np.pi
    theta = np.linspace(-PI / 2, PI, 270)
    theta = np.mat(theta)
    assert theta.shape == (1,270)
    assert xc.shape == (L,1)
    rho = xc*np.cos(theta)+yc*np.sin(theta)
    var_rho=np.var(rho,axis=0)
    assert var_rho.shape == (1,270)
    theta_index=np.argpartition(var_rho, 1)
    rho_c1=np.mean(rho[:,theta_index[0,0]])
    rho_c2=np.mean(rho[:,theta_index[0,1]])
    return rho_c1,theta[0,theta_index[0,0]],rho_c2,theta[0,theta_index[0,1]],rho,theta_index[0,0],theta_index[0,1],var_rho


def get_ang_dis_HT_v3(img_gray):
    # 返回值ang是由theta变过去的，要话直线检测对不对的话，加一个返回值theta
    hist = cv2.calcHist([img_gray], [0], None, [256], [0, 255])
    hist2 = hist[220:255, :]
    th = np.argmax(hist2) - 1 + 220 - 2

    _, im_b = cv2.threshold(img_gray, th, 255, cv2.THRESH_BINARY)
    # 阈值应该是在灰度直方图上找最后一个峰，比如在200~255范围内找一个峰值，然后以峰值对应灰度减5作为阈值
    _, _, _, centroids = cv2.connectedComponentsWithStats(im_b.copy())
    #print('cenroids.shape=', centroids.shape)
    xc = centroids[:, 0]
    yc = centroids[:, 1]
    xc = np.mat(xc).T
    yc = np.mat(yc).T
    rho_1,theta_1,rho_2,theta_2=hough_vot(xc,yc)

    if theta_1<np.pi/2:
        ang_1=-theta_1
        distance_1 = rho_1 - (320*np.cos(theta_1)+240*np.sin(theta_1))
    elif np.pi/2<=theta_1 and theta_1<=np.pi:
        ang_1=np.pi - theta_1
        distance_1 = (320*np.cos(theta_1)+240*np.sin(theta_1)) - rho_1
    else:
        ang_1 = 21
        distance_1 = 1202

    if theta_2<np.pi/2:
        ang_2=-theta_2
        distance_2 = rho_2 - (320 * np.cos(theta_2) + 240 * np.sin(theta_2))
    elif np.pi/2<=theta_2 and theta_2<=np.pi:
        ang_2=np.pi - theta_2
        distance_2 = (320 * np.cos(theta_2) + 240 * np.sin(theta_2)) - rho_2
    else:
        ang_2 = 22
        distance_2 = 1202
    
    '''
    if np.abs(ang_2) < np.abs(ang_1):
        ang = ang_2
        distance = distance_2
    else:
        ang = ang_1
        distance = distance_1
        
    '''
    if np.abs(np.abs(ang_2) - np.abs(ang_1)) < 10*np.pi/180:
        ang = ang_1
        distance = distance_1
    else:
        if np.abs(ang_2) < np.abs(ang_1):
            ang = ang_2
            distance = distance_2
        else:
            ang = ang_1
            distance = distance_1
    

    return ang,distance,im_b

def hough_vot(xc,yc):
    L=len(xc)
    theta = np.linspace(-np.pi / 2, np.pi, 270)
    theta = np.mat(theta)
    assert theta.shape == (1,270)
    assert xc.shape == (L,1)
    rho = xc*np.cos(theta)+yc*np.sin(theta)
    vot = np.zeros((800, 270))
    # tStart = time.clock()
    for i in range(len(xc)):
        for j in range(270):
            if rho[i, j] > 0:
                index = rho[i, j].astype(np.int)
                vot[index, j] += 1

    max_index = np.where(vot == np.max(vot))
    index_rho_1 = max_index[0][0]
    index_theta_1 = max_index[1][0]
    vot[index_rho_1, index_theta_1] = 0
    while 1:
        max_index = np.where(vot == np.max(vot))
        if np.abs(index_theta_1 - max_index[1][0]) < 10 :#and np.abs(index_rho_1 - max_index[0][0]) :
            vot[max_index[0][0], max_index[1][0]] = 0
        else:
            index_rho_2 = max_index[0][0]
            index_theta_2 = max_index[1][0]
            break

    return index_rho_1,theta[0,index_theta_1],index_rho_2,theta[0,index_theta_2]


'''
#vot0=np.matrix('4,5,6,3;11,9,7,1;8,12,34,2')
vot = np.zeros((3,4))
vot[0,:]=[4,5,6,3]
vot[1,:]=[11,9,7,1]
vot[2,:]=[8,12,34,2]
print(vot)

vot_clum =vot.reshape(12,order='F')

print(vot_clum)
indexall=np.argpartition(vot_clum, -2)[-2:]
print(indexall)
index_rho1,index_theta1=divmod(indexall[0], vot.shape[0])
index_rho2,index_theta2=divmod(indexall[1], vot.shape[0])
print(index_rho1,index_theta1)
print(index_rho2,index_theta2)

'''

'''


#img3=cv2.imread('../pic_v6/aa1120rot_v3.jpg')
img3=cv2.imread('../light_pic/bb650rot_v3.jpg')
img = cv2.cvtColor(img3,cv2.COLOR_BGR2GRAY)
cv2.imshow('img',img)

rho,ang,distance,im_b,theta = get_ang_dis_HT_v2(img)
print('ang=',ang*180/np.pi,'distance=',distance)

ang_1,distance_1,ang_2,distance_2,im_b = get_ang_dis_HT_v3(img)
print('ang1=',ang_1*180/np.pi,'distance_1=',distance_1,'ang_2=',ang_2*180/np.pi,'distance_2',distance_2)
cv2.waitKey(0)


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
