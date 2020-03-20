#!/usr/bin/python3
#coding=utf-8
import caffe
import cv2
import numpy as np


def detect_alpha(cap1):
    
    alphabet_class=0x00000000
    ok,frame=cap1.read() # ok��ʾ����ͷ��ȡ״̬��frame��ʾ����ͷ��ȡ��ͼ��
    if not ok :
        return alphabet_class
#cut
    path = "./"     #根目录
    #img = cv2.imread(path+"temp/1.jpg")
    img = frame
    #img = cv2.imread(path+"test_pic/012.png")
    if img is None:
        return alphabet_class    
    cv2.imshow("image", img)
    cv2.waitKey(10)
    
    

    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    blurred = cv2.GaussianBlur(gray, (3, 3), 0)

    #gradX = cv2.Sobel(blurred, ddepth=cv2.CV_32F, dx=1, dy=0)
    #gradY = cv2.Sobel(blurred, ddepth=cv2.CV_32F, dx=0, dy=1)
    #gradient = cv2.subtract(gradX, gradY)
    #gradient = cv2.convertScaleAbs(gradient)

    #cv2.imshow("sobel_img", gradient)
    #cv2.waitKey(10)
    


    thresh = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY_INV,5,10)

    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (3, 3))
    #closed = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, kernel)
    closed = cv2.dilate(thresh,kernel)


    cv2.imshow("closed_img", closed)
    cv2.waitKey(10)
    #cv2.waitKey(0)

    h = cv2.findContours(closed, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
    contours = h[1]
    num = len(contours)
    #judge alpha exist or not
    if num==0:
        return alphabet_class
    if len(contours[0])<50:
        return alphabet_class
    
    
    
    
    imgfin = img.copy()

    for i in range(0, num):
        cnt = contours[i]
        x,y,w,h = cv2.boundingRect(cnt)
        #print(cnt)
        #box = np.int0(cv2.boxPoints(rect))
        #print(box)
        #draw_img = cv2.drawContours(imgfin, [box], -1, (0, 0, 255), 2)
        cv2.rectangle(imgfin,(x,y),(x+w,y+h),(0, 0, 255), 2)
        #Xs = [ii[0] for ii in box]
        #Ys = [ii[1] for ii in box]
        #x1 = min(Xs)
        #x2 = max(Xs)
        #y1 = min(Ys)
        #y2 = max(Ys)
        #hight = y2 - y1
        #width = x2 - x1
        crop_img = img[y:y+h,x:x+w]
        
        pi = cv2.resize(crop_img,(16,16))
        cv2.imwrite(path + 'detect_results/' + str(num-i) + '.png', pi)

    
    cv2.imshow("draw_img", imgfin)
    cv2.waitKey(10)
    #cv2.waitKey(0)


    #cv2.destroyAllWindows()




    #detect
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

    
    for i in range(0, num):
        im=caffe.io.load_image(path + 'detect_results/' + str(i+1) + '.png')                   #加载图片
        net.blobs['data'].data[...] = transformer.preprocess('data',im)      #执行上面设置的图片预处理操作，并将图片载入到blob中

        #执行测试
        out = net.forward()

        prob= net.blobs['Softmax1'].data[0].flatten() #取出最后一层（Softmax）属于某个类别的概率值，并打印
        #print(prob)
        output_str=labels[prob.argsort()[-1]]  #将概率值排序，取出最大值所在的序号
        alphabet_class += (ord(output_str)-ord('@'))<<(8*i)
        #print('the class is:',labels[order])   #将该序号转换成对应的类别名称，并打印
    return alphabet_class

if __name__ == '__main__': # __name__ �ǵ�ǰģ��������ģ�鱻ֱ������ʱģ����Ϊ __main__ ����仰����˼���ǣ���ģ�鱻ֱ������ʱ�����´���齫�����У���ģ���Ǳ�����ʱ������鲻�����С�
    
    cap1=cv2.VideoCapture(1)
    missframe_count=50
    while missframe_count:
        cap1.read()
        missframe_count=missframe_count-1
    while cap1.isOpened():
        result=detect_alpha(cap1)
        st='%06x'%result
        print(st)
        print(type(order))
        print(order)
    cap1.release()
    #result=detect_alpha(cap1)


