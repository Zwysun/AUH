import light2line
import cv2
import math
import pic2abc
from matplotlib import pyplot as plt
'''
img3=cv2.imread('pic/muti_G.png')
img = cv2.cvtColor(img3,cv2.COLOR_BGR2GRAY)
th,im_b=cv2.threshold(img,255-60, 255, cv2.THRESH_BINARY)
cv2.imshow('ssa',im_b)
abc_str,im_index=pic2abc.getabc(im_b)
print(abc_str)
'''
img3 = cv2.imread('../abcpic/730ab.jpg')
img = cv2.cvtColor(img3,cv2.COLOR_BGR2GRAY)
img_eqhist=cv2.equalizeHist(img)
cv2.imshow('ss',img_eqhist)

GBlur = cv2.GaussianBlur(img_eqhist, (5, 5), 0)
cv2.imshow('gray',img)
th,im_b_g=cv2.threshold(GBlur, 0, 255, cv2.THRESH_OTSU)
im_b_g = 255-im_b_g
cv2.imshow(str(th)+'imb_g',im_b_g)
img_co, _, stats, centroids = cv2.connectedComponentsWithStats(im_b_g.copy())
print('stats.shape=',stats.shape) # stats.shape = (14,5) or (N,5)
#print('img_co.shape=',np.shape(img_co))
# stats[i,:]=[x_i,y_i,width_i,heigth_i,area_i]
plt.figure(1)
plt.imshow(im_b_g,cmap='gray')
for i in range(stats.shape[0]):
    area= stats[i,4]
    x_i=stats[i,0]
    y_i=stats[i,1]
    width_i = stats[i,2]
    heigth_i=stats[i,3]
    wRh = width_i/heigth_i

    print(i,'th,area=',area,'width/height=',wRh)
    # width/height <0.5 or >0.8是字母
    plt.plot(stats[i,0],stats[i,1],'bo',label='aa')

    if area > 500 and area <10000 :#and (wRh <0.5 or wRh > 0.8):
        plt.hlines(y_i,x_i,x_i+width_i,'r',label='as')
        plt.vlines(x_i,y_i,y_i+heigth_i,'r',label='bh')
        sub_pic_i = im_b_g[y_i:y_i+heigth_i,x_i:x_i+width_i]
        sub_pic_i = 255 - sub_pic_i
        cv2.imshow('sub'+str(i),sub_pic_i)
        abcstr = pic2abc.get_single_abc_cv2(sub_pic_i)
        print(i,abcstr)



plt.show()
'''
img3=cv2.imread('_test0.png')
img = cv2.cvtColor(img3,cv2.COLOR_BGR2GRAY)
th,im_b=cv2.threshold(img, 0, 255, cv2.THRESH_OTSU)
abcstr = pic2abc.get_single_abc_cv2(im_b)
print(abcstr)
'''
cv2.waitKey(0)