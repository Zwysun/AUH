#!/usr/bin/env python
#coding=utf-8


import cv2

cap = cv2.VideoCapture(1)
missframe_cout=30
while missframe_cout:
    cap.read()
    missframe_cout=missframe_cout-1
while cap.isOpened():
    
    for i in range(10):
        ok,frame=cap.read()
        
        cv2.waitKey(1000)
        cv2.imwrite('./capture_letter/'+str(i)+'.jpg',frame)
    cap.release()
    



