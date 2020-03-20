import cv2
import numpy as np
import pic_cut
import pic2abc
from matplotlib import pyplot as plt
kernelX = cv2.getStructuringElement(cv2.MORPH_RECT, (6, 1))
kernelX2 = cv2.getStructuringElement(cv2.MORPH_RECT, (10, 1))
kernelY = cv2.getStructuringElement(cv2.MORPH_RECT, (10, 10))

img3 = cv2.imread('../abcpic/620ab.jpg')
img = cv2.cvtColor(img3,cv2.COLOR_BGR2GRAY)
#img_s = cv2.cvtColor(img3,cv2.COLOR_BGR2HSV)
#img_s=img_s[:,:,1]
GBlur = cv2.GaussianBlur(img, (5, 5), 0)
cv2.imshow('gray',img)
th,im_b_g=cv2.threshold(GBlur, 0, 255, cv2.THRESH_OTSU)
#th,im_b_g=cv2.threshold(GBlur, 120, 255, cv2.THRESH_BINARY)
#th_s,im_b_s=cv2.threshold(img_s, 0, 255, cv2.THRESH_OTSU)



'''
    cv2.figure(i+2)
    plt.plot(sub_pic_i,cmap='gray')
'''

_,imb2=cv2.threshold(img, 100, 255, cv2.THRESH_BINARY)
#cv2.imshow('imb',imb2)
#canny = cv2.Canny(img, threshold1=10,threshold2=500,edges=None,apertureSize=(5,5))
canny = cv2.Canny(img, 10,500)
cv2.imshow('canny',canny)
#cv2.imshow('img_s',img_s)
#cv2.imshow('img_sb'+str(th_s),im_b_s)
img_di = cv2.dilate(canny, kernelX, iterations=4)
img_close = cv2.morphologyEx(canny,cv2.MORPH_CLOSE,kernelY)
#cv2.imshow('img_di',img_di)
#cv2.imshow('close',img_close)

'''
img_er = cv2.erode(img_di,kernelY,iterations=3)
cv2.imshow('img_er',img_er)
img_di2 = cv2.dilate(img_er,kernelX2,iterations=8)
cv2.imshow('img_di2',img_di2)
'''

img_co, _, stats, centroids = cv2.connectedComponentsWithStats(img_close.copy())
print('stats.shape=',stats.shape) # stats.shape = (14,5) or (N,5)
#print('img_co.shape=',np.shape(img_co))
# stats[i,:]=[x_i,y_i,width_i,heigth_i,area_i]
img_eqhist = cv2.equalizeHist(img)
cv2.imshow('img_eqhist',img_eqhist)
plt.figure(1)
plt.imshow(img_close,cmap='gray')
index_arg = np.argsort(stats[:,0])
print(index_arg)
for i in index_arg:
    area= stats[i,4]
    x_i=stats[i,0]
    y_i=stats[i,1]
    width_i = stats[i,2]
    heigth_i=stats[i,3]
    wRh = width_i/heigth_i
    print(i, 'th,area=', area, 'width/height=', wRh)
    # width/height <0.5 or >0.8是字母
    plt.plot(stats[i, 0], stats[i, 1], 'bo', label='aa')

    if area > 200 and area < 10000:  # and (wRh <0.5 or wRh > 0.8):
        plt.hlines(y_i, x_i, x_i + width_i, 'r', label='as')
        plt.vlines(x_i, y_i, y_i + heigth_i, 'r', label='bh')
        sub_pic_i = img_eqhist[y_i:y_i + heigth_i, x_i:x_i + width_i]
        #sub_pic_i = 255 - sub_pic_i
        cv2.imshow('sub' + str(i), sub_pic_i)
        th_i,sub_pic_i=cv2.threshold(sub_pic_i, 0, 255, cv2.THRESH_OTSU)
        cv2.imshow('sub_th' + str(i), sub_pic_i)

        abcstr = pic2abc.get_single_abc_cv2(sub_pic_i)
        print(i, abcstr)

'''
lines = cv2.HoughLines(canny,1,np.pi/180,80)
lines1 = lines[:,0,:]
plt.figure(2)
plt.imshow(img,cmap='gray')
xx=range(0,600,1)
for rho,theta in lines1[:]:
    if theta >0 :
        yy = (rho - xx * np.cos(theta)) / np.sin(theta)
        plt.plot(xx, yy, 'r-', label='ss')
'''
plt.show()


'''
im_L,L=pic_cut.cut(img)
print('L=',L)
for i in range(L):
    cv2.imshow('img' + str(i),im_L[i])
    abcstr, _ = pic2abc.getabc(im_L[i])
    print(abcstr)
    print('-----' + str(i) + '-----------')
    if i>10:
        break
'''


cv2.waitKey(0)
'''
img = cv2.imread("mutiabc.jpg")
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
print(np.shape(img))
print(np.shape(gray))
blurred = cv2.GaussianBlur(gray, (9, 9),0)

gradX = cv2.Sobel(gray, ddepth=cv2.CV_32F, dx=1, dy=0)
gradY = cv2.Sobel(gray, ddepth=cv2.CV_32F, dx=0, dy=1)

gradient = cv2.subtract(gradX, gradY)
gradient = cv2.convertScaleAbs(gradient)


blurred = cv2.GaussianBlur(gradient, (9, 9),0) # 对梯度图像再次高斯模糊
(_, thresh) = cv2.threshold(blurred, 90, 255, cv2.THRESH_BINARY)

kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (25, 25))
closed = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, kernel)

closed = cv2.erode(closed, None, iterations=4)
closed = cv2.dilate(closed, None, iterations=4)

(_,cnts,_) = cv2.findContours(closed.copy(),
                                cv2.RETR_LIST,
                                cv2.CHAIN_APPROX_SIMPLE)
	#参数一： 二值化图像
	#closed.copy(),
	#参数二：轮廓类型
     #cv2.RETR_EXTERNAL,             #表示只检测外轮廓
    # cv2.RETR_CCOMP,                #建立两个等级的轮廓,上一层是边界
    # cv2.RETR_LIST,                 #检测的轮廓不建立等级关系
    # cv2.RETR_TREE,                 #建立一个等级树结构的轮廓
    # cv2.CHAIN_APPROX_NONE,         #存储所有的轮廓点，相邻的两个点的像素位置差不超过1
    #参数三：处理近似方法
     #cv2.CHAIN_APPROX_SIMPLE,         #例如一个矩形轮廓只需4个点来保存轮廓信息
    # cv2.CHAIN_APPROX_TC89_L1,
    # cv2.CHAIN_APPROX_TC89_KCOS
    #)
c = sorted(cnts, key=cv2.contourArea, reverse=True)[0]

# compute the rotated bounding box of the largest contour
rect = cv2.minAreaRect(c)
box = np.int0(cv2.boxPoints(rect))

# draw a bounding box arounded the detected barcode and display the image
draw_img = cv2.drawContours(img.copy(), [box], -1, (0, 0, 255), 3)
cv2.imshow("draw_img", draw_img)


'''

'''
abcresult = pic2abc.getabc(im_matrix)


print(np.shape(im_matrix))
im_matrix = np.sum(im_matrix,2)/3
im_matrix = im_matrix[:,:]
print(np.shape(im_matrix))

print(abcresult)
'''