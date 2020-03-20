#!/usr/bin/python3
# coding=utf-8
import serial
import binascii
import cv2
import time
from opencv_detect_redline import readframe
from caffe_recognize_texts import detect_alpha
import numpy as np
import os
import gc
from multiprocessing import Process, Manager


def capframe(stack, cam, top):

    print('Process to write: %s' % os.getpid())
    cap = cv2.VideoCapture(cam)
    #cap.set(cv2.CAP_PROP_FPS, 60)
    while True:
        ok, img = cap.read()
        if ok:
            stack.append(img)
            if len(stack) >= top:
                del stack[:]
                gc.collect()

if __name__ == '__main__':
    # 父进程创建缓冲栈，并传给各个子进程：
    q = Manager().list()
    pw = Process(target=capframe, args=(q, 0, int(100)))
    #pr = Process(target=read, args=(q,))
    # 启动子进程pw，写入:
    pw.start()
    
    ser = serial.Serial("/dev/ttyAMA0",115200)
    #ser.open() #if the port has already been opened, an error will appears here
    depth=10
    head=0xfe

    length_nav=0x00 #6+4*2 char
    length_register=0x00 #6+1char
    length_alphabet=0x00 #6+4 char

    count=0x00#no use till now
    nn=0

    ID_nav=0x23
    ID_register=0x24
    ID_alphabet=0x25


    #angle=0x11111111 #the size is 4 char
    #distance=0x00001111 #the size is 4 char
    #register_information= #the size is 1 char
    #alphabet_class= # the size is 2 char


    CRC=0x00#no use till now

    tile=0x0a
    #####################################################

    # very allergic to noise
    #cap=cv2.VideoCapture(0)#open camera
    cap1=cv2.VideoCapture(1)
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
        data=ord(ser.read(1))# only read specific numbers of bytes,and convert the bytes variable to int
        #ok,frame=cap.read()
        #cv2.imshow('result',out[0])
        #cv2.waitKey(1)
        if data==0:
            print('calculate angle')
            
            #frame=np.mat(out[0])
            while len(q) == 0:
                pass
            frame = q.pop()
            angle,distance,theta = readframe(frame,nn)
            if angle<0:
                angle=0xffffffff+angle+1
            if distance<0:
                distance=0xffffffff+distance+1    
            nav_data="%x%02x%02x%02x%08x%08x%02x%02x%02x"%(head,length_nav,count,ID_nav,angle,distance,CRC,tile,tile)
            print (nav_data)
            ser.write(bytes.fromhex(nav_data))
            
            nn+=1
            
            img = cv2.imread('/home/pi/Desktop/temp/1.jpg')
            depth_str = "%d" % (depth)
            cv2.imwrite('/home/pi/Desktop/depth_pic/' + depth_str + '.jpg', img)

        elif data==1:
            print('analyze the letter')
            
            alphabet_class=detect_alpha(cap1)
            alphabet_data="%x%02x%02x%02x%08x%02x%02x%02x"%(head,length_alphabet,count,ID_alphabet,alphabet_class,CRC,tile,tile)
            print(alphabet_data)
            ser.write(bytes.fromhex(alphabet_data))
        
        elif data==2:
            break
        
        else:
            depth=data
            print(depth)
    # pw进程里是死循环，无法等待其结束，只能强行终止:
    pw.terminate()
    cap1.release()
    #serial.Serial.close(t)







#register_data="%x%02x%02x%02x%02x%02x%02x"%(head,length_register,count,ID_register,,CRC,tile)

    