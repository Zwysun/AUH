#!/usr/bin/python3
#coding=utf-8
import caffe
import cv2
import numpy as np
import math

##2019/7/8 16:11
#适配树莓派
##2019/7/8 14:27
#更改预处理方式
##2019/7/7 19:31
#加入4个排序
##2019/7/7 17:07
#限制输出为4个
def detect_alpha(frame):
    
    #ok,frame=cap1.read()
    #if not ok :
    #    return alphabet_class
#cut
    #path = "./"     #根目录
    #img = cv2.imread(path+"temp/1.jpg")
    # img = frame
    # #img = cv2.imread(path+"test_pic/012.png")
    # if img is None:
    #     return alphabet_class
    # cv2.imshow("image", img)
    # cv2.waitKey(10)
    
    

    # gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # blurred = cv2.GaussianBlur(gray, (3, 3), 0)

    # #gradX = cv2.Sobel(blurred, ddepth=cv2.CV_32F, dx=1, dy=0)
    # #gradY = cv2.Sobel(blurred, ddepth=cv2.CV_32F, dx=0, dy=1)
    # #gradient = cv2.subtract(gradX, gradY)
    # #gradient = cv2.convertScaleAbs(gradient)

    # #cv2.imshow("sobel_img", gradient)
    # #cv2.waitKey(10)
    


    # thresh = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY_INV,5,10)

    # kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (3, 3))
    # #closed = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, kernel)
    # closed = cv2.dilate(thresh,kernel)


    # cv2.imshow("closed_img", closed)
    # cv2.waitKey(10)
    # #cv2.waitKey(0)

    # h = cv2.findContours(closed, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
    # contours = h[1]
    # num = len(contours)
    # #judge alpha exist or not
    # if num==0:
    #     return alphabet_class
    # if len(contours[0])<50:
    #     return alphabet_class
    
    
    
    
    # imgfin = img.copy()

    # for i in range(0, num):
    #     cnt = contours[i]
    #     x,y,w,h = cv2.boundingRect(cnt)
    #     #print(cnt)
    #     #box = np.int0(cv2.boxPoints(rect))
    #     #print(box)
    #     #draw_img = cv2.drawContours(imgfin, [box], -1, (0, 0, 255), 2)
    #     cv2.rectangle(imgfin,(x,y),(x+w,y+h),(0, 0, 255), 2)
    #     #Xs = [ii[0] for ii in box]
    #     #Ys = [ii[1] for ii in box]
    #     #x1 = min(Xs)
    #     #x2 = max(Xs)
    #     #y1 = min(Ys)
    #     #y2 = max(Ys)
    #     #hight = y2 - y1
    #     #width = x2 - x1
    #     crop_img = img[y:y+h,x:x+w]
        
    #     pi = cv2.resize(crop_img,(16,16))
    #     cv2.imwrite(path + 'detect_results/' + str(num-i) + '.png', pi)

    
    # cv2.imshow("draw_img", imgfin)
    # cv2.waitKey(10)
    # #cv2.waitKey(0)


    # #cv2.destroyAllWindows()
    alphabet_class=0x00000000
    # sp = frame.shape
    # #cv2.imwrite('' + str(nn) + '.jpg', frame)
    # H = sp[0]
    # W = sp[1]

    # hue_image = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    # low_range1 = np.array([110, 15, 150])
    # high_range1 = np.array([180, 30, 200])
    # th1 = cv2.inRange(hue_image, low_range1, high_range1)
    # low_range2 = np.array([100, 10, 70])
    # high_range2 = np.array([150, 110, 140])
    # th2 = cv2.inRange(hue_image, low_range2, high_range2)
    # th = th1+th2

    # #cv2.imwrite("./display/05.jpg",th)
    # cv2.imshow("detect", th)
    # cv2.waitKey(0)

    # kernel = cv2.getStructuringElement(cv2.MORPH_RECT,(17,17))
    # closed = cv2.morphologyEx(th, cv2.MORPH_CLOSE, kernel)

    # #cv2.imwrite("./display/04.jpg",closed)
    # #cv2.imshow("closed", closed)
    # #cv2.waitKey(0)

    # _, contours, hierarchy = cv2.findContours(closed,cv2.RETR_TREE,cv2.CHAIN_APPROX_NONE)
    
    # if len(contours)==0:
    #     return alphabet_class

    # cnt=contours[0]
    # temp=cnt.shape[0]
    # for i in range(len(contours)):
    #     if contours[i].shape[0]>temp:
    #         temp = contours[i].shape[0]
    #         cnt = contours[i]
    
    # if temp<1000:
    #     return alphabet_class

    # black0=np.zeros((H,W),dtype=np.uint8)
    # black = cv2.copyMakeBorder(black0,160,160,80,80, cv2.BORDER_CONSTANT,value=[0,0,0])

    # rect = cv2.minAreaRect(cnt)
    # box = cv2.boxPoints(rect)
    # box = np.int0(box)
    # box = box + np.matrix('80,160;80,160;80,160;80,160')

    # cv2.drawContours(black,[box],0,(255,255,255),-1)
    # angle = rect[2]+90
    # #cv2.imwrite("./display/03.jpg",black)
    # #print(angle)
    # #cv2.imshow("final", black)
    # #cv2.waitKey(0)
    # frame = cv2.copyMakeBorder(frame,160,160,80,80, cv2.BORDER_REPLICATE)
    # center = (400, 400) #1
    # M = cv2.getRotationMatrix2D(center, angle, 1.0) #12
    # rotated = cv2.warpAffine(black, M, (800, 800)) #13

    # rotated2 = cv2.warpAffine(frame, M, (800, 800)) #13
    # #cv2.imwrite("./display/02.jpg",rotated)
    # #cv2.imshow("rot", rotated)
    # #cv2.waitKey(0)

    # _, contours2, hierarchy2 = cv2.findContours(rotated,cv2.RETR_TREE,cv2.CHAIN_APPROX_NONE)
    # cnt=contours2[0]
    # x,y,w,h = cv2.boundingRect(cnt)
    # crop_img = rotated2[y:y+h,x:x+w]
    #cv2.imshow("crop", crop_img)
    #cv2.waitKey(0)
    #cv2.imwrite("./display/01.jpg",crop_img)
    sp = frame.shape
    #cv2.imwrite('' + str(nn) + '.jpg', frame)
    H = sp[0]
    W = sp[1]

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    ret,thresh1 = cv2.threshold(gray,60,255,cv2.THRESH_BINARY_INV)
    # cv2.imshow("thresh", thresh1)
    # cv2.waitKey(0)
    thresh2 = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY_INV,25,10)
    # cv2.imshow("thresh", thresh2)
    # cv2.waitKey(0)
    thresh = cv2.bitwise_and(thresh1,thresh2)
    #cv2.imshow("thresh", thresh)
    #cv2.waitKey(0)

    _, contours0, hierarchy0 = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
    #c_max = []
    for i in range(len(contours0)):
        cnt = contours0[i]
        area = cv2.contourArea(cnt)
        if (area < 300):
            c_min = []
            c_min.append(cnt)
            cv2.drawContours(thresh, c_min, -1, (0, 0, 0), cv2.FILLED)
    # cv2.imshow("drawblack", thresh)
    # cv2.waitKey(0)

    kernel0 = cv2.getStructuringElement(cv2.MORPH_RECT,(100,5))
    closed = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, kernel0)
    #cv2.imshow("close", closed)
    #cv2.waitKey(0)

    _, contours1, hierarchy1 = cv2.findContours(closed,cv2.RETR_TREE,cv2.CHAIN_APPROX_NONE)
    if len(contours1) == 0:
        return alphabet_class
    cnt=contours1[0]
    temp=cnt.shape[0]

    for i in range(len(contours1)):
        if contours1[i].shape[0]>temp:
            temp = contours1[i].shape[0]
            cnt = contours1[i]
    if temp<500:
        return alphabet_class


    black0=np.zeros((H,W),dtype=np.uint8)
    black = cv2.copyMakeBorder(black0,160,160,80,80, cv2.BORDER_CONSTANT,value=[0,0,0])
    rect = cv2.minAreaRect(cnt)
    box = cv2.boxPoints(rect)
    box = np.int0(box)
    box = box + np.matrix('80,160;80,160;80,160;80,160')
    cv2.drawContours(black,[box],0,(255,255,255),-1)
    # cv2.imshow("black",black)
    # cv2.waitKey(0)

    kernel1 = cv2.getStructuringElement(cv2.MORPH_RECT,(40,20))
    dilated = cv2.dilate(black,kernel1)
    #cv2.imshow("dilate",dilated)
    #cv2.waitKey(0)

    angle = rect[2]
    #cv2.imwrite("./display/03.jpg",black)
    #print(angle)
    #cv2.waitKey(0)
    #cv2.imshow("final", black)
    #cv2.waitKey(0)
    frame = cv2.copyMakeBorder(frame,160,160,80,80, cv2.BORDER_REPLICATE)
    center = (400, 400) #1
    M = cv2.getRotationMatrix2D(center, angle, 1.0) #12
    rotated = cv2.warpAffine(dilated, M, (800, 800)) #13
    rotated2 = cv2.warpAffine(frame, M, (800, 800)) #13
    #cv2.imwrite("./display/02.jpg",rotated)
    #cv2.imshow("rot", rotated)
    #cv2.waitKey(0)
    _, contours2, hierarchy2 = cv2.findContours(rotated,cv2.RETR_TREE,cv2.CHAIN_APPROX_NONE)
    if len(contours2) == 0:
        return alphabet_class
    cnt=contours2[0]
    x,y,w,h = cv2.boundingRect(cnt)
    crop_img = rotated2[y:y+h,x:x+w]
    # cv2.imshow("crop",crop_img)
    # cv2.waitKey(0)

    gray = cv2.cvtColor(crop_img, cv2.COLOR_BGR2GRAY)
    thresh = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY_INV,31,10)
    kernel0 = cv2.getStructuringElement(cv2.MORPH_RECT,(5,5))
    thresh = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, kernel0)
    # cv2.imshow("thresh", thresh)
    # cv2.waitKey(0)

    _, contours3, hierarchy3 = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
    #c_max = []
    c_min = []
    for i in range(len(contours3)):
        cnt = contours3[i]
        area = cv2.contourArea(cnt)
        #print(area)
        if (area < 1000):
            c_min.append(cnt)
    cv2.drawContours(thresh, c_min, -1, (0, 0, 0), cv2.FILLED)

    #thresh=cv2.bitwise_not(thresh)
    kernel2 = cv2.getStructuringElement(cv2.MORPH_RECT,(11,11))
    thresh = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, kernel2)

    #cv2.imshow("final", thresh)
    #cv2.waitKey(0)

    _, contours4, hierarchy4 = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

    num = len(contours4)
    #print(num)
    ##排序
    cnt_sorted=[]
    for i in range(num):
        cnt = contours4[i]
        x, y, w, h = cv2.boundingRect(cnt)
        #print(x)
        cnt_sorted.append([cnt, x])
    cnt_sorted.sort(key=lambda elem: elem[-1])
    for i in range(num):
        temp = cnt_sorted[i]
        #print(temp.pop())
        cnt_sorted[i] = temp[0]

    for i in range(num):
        cnt = cnt_sorted[i]
        x, y, w, h = cv2.boundingRect(cnt)
        #cv2.rectangle(imgfin, (x, y), (x + w, ycaffe + h), (0, 0, 255), 2)
        crop0 = thresh[y:y + h, x:x + w]
        #print(int((1.5*math.sqrt(h*h+w*w)-w)/2), int((1.5*math.sqrt(h*h+w*w)-w)/2), int((1.5*math.sqrt(h*h+w*w)-h)/2), int((1.5*math.sqrt(h*h+w*w)-h)/2))
        crop = cv2.copyMakeBorder(crop0, int((1.2*math.sqrt(h*h+w*w)-h)/2), int((1.2*math.sqrt(h*h+w*w)-h)/2), int((1.2*math.sqrt(h*h+w*w)-w)/2), int((1.2*math.sqrt(h*h+w*w)-w)/2), cv2.BORDER_CONSTANT,value=[0,0,0])
        pi0 = cv2.GaussianBlur(cv2.bitwise_not(crop),(33,33),0)
        pi = cv2.resize(pi0, (64，64))
        cv2.imwrite('./detect_results/' + str(num-i) + '.png', pi)



    #detect
    path = "./"     #根目录
    model = 'trained_model_64'
    deploy= path + model + '/deploy.prototxt'    #deploy文件
    caffe_model= path + model + '/lenet_iter_2070.caffemodel'   #训练好的 caffemodel
    #img ='test_pics/abc.png'    #随机找的一张待测图片
    labels_filename = path + model + '/labels.txt'  #类别名称文件，将数字标签转换回类别名称

    net = caffe.Net(deploy,caffe_model,caffe.TEST)   #加载model和network
    labels = np.loadtxt(labels_filename, str, delimiter='\t')   #读取类别名称文件

    #图片预处理设置
    transformer = caffe.io.Transformer({'data': net.blobs['data'].data.shape})  #设定图片的shape格式(1,3,28,28)
    transformer.set_transpose('data', (2,0,1))    #改变维度的顺序，由原始图片(28,28,3)变为(3,28,28)
    #transformer.set_mean('data', np.load(mean_file).mean(1).mean(1))    #减去均值，前面训练模型时没有减均值，这儿就不用
    transformer.set_raw_scale('data', 255)    # 缩放到【0，255】之间
    transformer.set_channel_swap('data', (2,1,0))   #交换通道，将图片由RGB变为BGR

    
    for i in range(4):
        im=caffe.io.load_image(path + 'detect_results/' + str(i+1) + '.png')                   #加载图片
        net.blobs['data'].data[...] = transformer.preprocess('data',im)      #执行上面设置的图片预处理操作，并将图片载入到blob中

        #执行测试
        out = net.forward()

        prob= net.blobs['Softmax1'].data[0].flatten() #取出最后一层（Softmax）属于某个类别的概率值，并打印
        #print(prob)
        output_str=labels[prob.argsort()[-1]]  #将概率值排序，取出最大值所在的序号
        alphabet_class += (ord(output_str)-ord('@'))<<(8*i)
        print('the class is:',output_str)   #将该序号转换成对应的类别名称，并打印
    return alphabet_class

if __name__ == '__main__': # __name__ �ǵ�ǰģ��������ģ�鱻ֱ������ʱģ����Ϊ __main__ ����仰����˼���ǣ���ģ�鱻ֱ������ʱ�����´���齫�����У���ģ���Ǳ�����ʱ������鲻�����С�
    
    # cap1=cv2.VideoCapture(1)
    # missframe_count=50
    # while missframe_count:
    #     cap1.read()
    #     missframe_count=missframe_count-1
    # while cap1.isOpened():
    #     ok,frame=cap1.read()
    #     if ok :
    #         result=detect_alpha(frame)
    #         st='%06x'%result
    #         print(st)
    #         #print(type(order))
    #         #print(order)
    # cap1.release()
    # #result=detect_alpha(cap1)
    frame = cv2.imread('./Debug&Config/capture_letter/730ab.jpg')
    alphabet_class = detect_alpha(frame)
    print(alphabet_class)


