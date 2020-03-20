#!/usr/bin/python3
# coding=utf-8
import serial
import binascii
import cv2
import time
from opencv_detect_redline import readframe
from caffe_recognize_texts import detect_alpha
from detectball import detectyb
import numpy as np
import os
import gc
from multiprocessing import Process, Manager, Value, Lock

##2019/7/8 13:20
#加入进程锁防止error
##2019/7/7 16:58
#将识别黄球转弯信息放入独立进程，识别灯带减少一个输出
##2019/7/5 19:45
#加入转弯处判断
##2019/7/5 17:11
#加入黄球识别

# def capframe(stack, cam, top):

#     print('Process to write: %s' % os.getpid())
#     cap = cv2.VideoCapture(cam)
#     #cap.set(cv2.CAP_PROP_FPS, 60)
#     while True:
#         ok, img = cap.read()
#         if ok:
#             stack.append(img)
#             if len(stack) >= top:
#                 del stack[:]
#                 gc.collect()

def capframe(stack1, stack2, lock, top):
    print('Camera Capture Process On, ID: %s' % os.getpid())
    cap1 = cv2.VideoCapture(0)
    cap2 = cv2.VideoCapture(1)
    #cap.set(cv2.CAP_PROP_FPS, 60)
    while True:
        #time.sleep(0.001)
        ok1, img1 = cap1.read()
        ok2, img2 = cap2.read()
        if ok1&ok2:
            lock.acquire()
            stack1.append(img1)
            if len(stack1) >= top:
                del stack1[:]
                gc.collect()
            lock.release()
            stack2.append(img2)
            if len(stack2) >= top:
                del stack2[:]
                gc.collect()

def detectyellowball(stack, flag, lock):

    print('YellowBall Detect Process On, ID: %s' % os.getpid())
    while True:
        #time.sleep(0.001)
        # while len(stack) == 0:
        #     pass
        # frame = stack[-1]
        lock.acquire()
        if len(stack) !=0:
            frame = stack[-1]
            lock.release()
        else:
            lock.release()
            continue
        hue_image0 = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        low_range0 = np.array([20, 124, 103])
        high_range0 = np.array([32, 255, 255])
        th0 = cv2.inRange(hue_image0, low_range0, high_range0)

        M = cv2.moments(th0)
        if M["m00"]/255>1000:
            flag.value = int(1000)
        else:
            flag.value = int(0)

        # ok, img = cap.read()
        # if ok:
        #     stack.append(img)
        #     if len(stack) >= top:
        #         del stack[:]
        #         gc.collect()
                
# def capframe2(stack, cam, top):

#     print('Process to write: %s' % os.getpid())
#     cap = cv2.VideoCapture(cam)
#     #cap.set(cv2.CAP_PROP_FPS, 60)
#     while True:
#         ok, img = cap.read()
#         if ok:
#             stack.append(img)
#             if len(stack) >= top:
#                 del stack[:]
#                 gc.collect()

if __name__ == '__main__':
    # 父进程创建缓冲栈，并传给各个子进程：
    q1 = Manager().list()
    q2 = Manager().list()
    flag=Value("d",int(0))
    l1 = Lock()
    pw = Process(target=capframe, args=(q1, q2, l1, int(100)))
    pw2 = Process(target=detectyellowball, args=(q1, flag, l1, ))

    #pw2 = Process(target=capframe2, args=(q2, 1, int(100)))
    #pr = Process(target=read, args=(q,))
    # 启动子进程pw，写入:
    pw.start()
    pw2.start()
    
    ser = serial.Serial("/dev/ttyAMA0",115200)
    #ser.open() #if the port has already been opened, an error will appears here
    depth=10
    head=0xfe

    length_nav=0x00 #6+4*2 char
    length_register=0x00 #6+1char
    length_alphabet=0x00 #6+4 char
    length_localization=0x00 #

    count=0x00#no use till now
    #nn=0

    ID_nav=0x23
    ID_register=0x24
    ID_alphabet=0x25
    ID_localization=0x26

    #angle=0x11111111 #the size is 4 char
    #distance=0x00001111 #the size is 4 char
    #register_information= #the size is 1 char
    #alphabet_class= # the size is 2 char
    #x_error=0x00000000 # the size is 4 char
    #y_error=0x00000000 # the size is 4 char

    CRC=0x00#no use till now

    tile=0x0a
    #####################################################

    # very allergic to noise
    #cap=cv2.VideoCapture(0)#open camera
    #cap1=cv2.VideoCapture(1)
    #cap.set(cv2.CAP_PROP_FPS, 60)# set fps
    #missframe_count=20
    #while missframe_count:
        #cap.read()
        #cap1.read()
        #missframe_count=missframe_count-1
    while True:  #循环重新启动串口
        #data=ser.readline() #read ultil 0x0a(\n) is met
        #time.sleep(0.1)
        #data=10
        #multiprocessing
        #m = Manager()
        #out = m.list()
        #array_dim = (480, 640, 3)
        #img_ndarray = np.zeros(array_dim)
        #out.append(img_ndarray.tolist())
        #p = Process(target=ShowPic, args=(out, ))
        #p.start()
        #p.join()     

        #multiprocessingend
        #data=100
        #while len(q1) == 0:
        #    pass
        #frame = q1[-1]
        data=ord(ser.read(1))# only read specific numbers of bytes,and convert the bytes variable to int
        #ok,frame=cap.read()
        #cv2.imshow('result',frame)
        #cv2.waitKey(1)
        if data==0:
            print('calculate angle')

            #frame=np.mat(out[0])
            while len(q1) == 0:
                pass
            frame = q1[-1]
            angle,distance = readframe(frame)
            turn_flag = int(flag.value)
            if angle<0:
                angle=0xffffffff+angle+1
            if distance<0:
                distance=0xffffffff+distance+1    
            nav_data="%x%02x%02x%02x%08x%08x%08x%02x%02x%02x"%(head,length_nav,count,ID_nav,angle,distance,turn_flag,CRC,tile,tile)
            print (nav_data)
            ser.write(bytes.fromhex(nav_data))

            #nn+=1

            #img = cv2.imread('/home/pi/Desktop/temp/1.jpg')
            #depth_str = "%d" % (depth)
            #cv2.imwrite('/home/pi/Desktop/depth_pic/' + depth_str + '.jpg', img)

        elif data==1:
            print('analyze the letter')
            while len(q2) == 0:
                pass
            frame2 = q2[-1]
            alphabet_class=detect_alpha(frame2)
            alphabet_data="%x%02x%02x%02x%08x%02x%02x%02x"%(head,length_alphabet,count,ID_alphabet,alphabet_class,CRC,tile,tile)
            print(alphabet_data)
            for _ in range(5):
                time.sleep(1)
                ser.write(bytes.fromhex(alphabet_data))
        
        elif data==2:
            break
        elif data==3:
            print('canning floating ball')
            while len(q1) == 0:
                pass
            frame = q1[-1]
            x_error, y_error = detectyb(frame)
            if x_error<0:
                x_error=0xffffffff+x_error+1
            if y_error<0:
                y_error=0xffffffff+y_error+1
            localization_data="%x%02x%02x%02x%08x%08x%02x%02x%02x"%(head,length_localization,count,ID_localization,x_error,y_error,CRC,tile,tile)
            print(localization_data)
            ser.write(bytes.fromhex(localization_data))
        else:
            depth=data
            print(depth)

    # pw进程里是死循环，无法等待其结束，只能强行终止:
    pw.terminate()
    pw2.terminate()
    #cap1.release()
    #serial.Serial.close(t)







#register_data="%x%02x%02x%02x%02x%02x%02x"%(head,length_register,count,ID_register,,CRC,tile)

    