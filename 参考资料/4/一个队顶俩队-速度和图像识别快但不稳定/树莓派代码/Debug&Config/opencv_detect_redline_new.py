#!/usr/bin/python3
# coding=utf-8
import numpy as np
import math
import cv2
def readframe(frame):
    
    ##7.27 改动三处
    ##7.7.17.00
    #去掉nn，去掉拐弯识别，去掉最后一个输出
    ##7.6.14.49
    #加入转弯区域识别（黄球）

    #ok,frame=cap.read() # ok��ʾ����ͷ��ȡ״̬��frame��ʾ����ͷ��ȡ��ͼ��
    #if not ok :
    #    return
    #frame = cv2.imread('C:/Users/liuzhongtian/Desktop/001.png')
    #frame = np.rot90(frame0)

    # sp = frame.shape
    # # cv2.imwrite('/home/pi/Desktop/capture_pics/' + str(nn) + '.jpg', frame)
    # H = sp[0]
    # W = sp[1]
    # hue_image = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    # low_range1 = np.array([0, 43, 215])
    # high_range1 = np.array([25, 255, 255])
    # th1 = cv2.inRange(hue_image, low_range1, high_range1)
    # low_range2 = np.array([156, 43, 215])
    # high_range2 = np.array([180, 255, 255])
    # th2 = cv2.inRange(hue_image, low_range2, high_range2)
    # th = th1 + th2

    # kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (27, 27))
    # closed = cv2.morphologyEx(th, cv2.MORPH_CLOSE, kernel)

    # minLineLength = 500
    # maxLineGap = 20
    # thresh = 250

    # lines = cv2.HoughLinesP(closed, 1, np.pi / 180, thresh, minLineLength, maxLineGap)

    # #cv2.imwrite('/home/pi/Desktop/temp/1.jpg', frame)

    # if lines is None:
    #     # print(angle_out)
    #     # print(distance_out)
    #     # print(theta_out)
    #     return angle_out, distance_out, theta_out
    # x0 = float(0.00000)
    # y0 = float(0.00000)
    # x1 = float(0.00000)
    # y1 = float(0.00000)
    # x2 = float(0.00000)
    # y2 = float(0.00000)
    # x3 = float(0.00000)
    # y3 = float(0.00000)
    # cal = int(0)
    # sel = [0, 0]
    # i = int(0)

    # num = lines.shape[0]
    # print(num)
    # for x0, y0, x1, y1 in lines[0]:
    #     pass
    # for cal in range(1, num):
    #     if i == 0:
    #         for x2, y2, x3, y3 in lines[cal]:
    #             if ((y1 - y0) * (y3 - y2) + (x1 - x0) * (x3 - x2)) == 0:
    #                 tan = 0
    #             else:
    #                 tan = abs(
    #                     ((y1 - y0) * (x3 - x2) - (y3 - y2) * (x1 - x0)) / (
    #                                 (y1 - y0) * (y3 - y2) + (x1 - x0) * (x3 - x2)))
    #             theta = math.atan(tan) * 180 / np.pi
    #             if (theta > 30):
    #                 sel[1] = cal
    #                 i = 1
    #                 break
    #     else:
    #         break

    # for x0, y0, x1, y1 in lines[sel[0]]:
    #     cv2.line(frame, (x0, y0), (x1, y1), (0, 0, 0), 2)
    # theta = 0
    # if (sel[1] > 0):
    #     for x2, y2, x3, y3 in lines[sel[1]]:
    #         cv2.line(frame, (x2, y2), (x3, y3), (0, 255, 0), 2)
    #         # cv2.imshow("result",frame)
    #         # cv2.waitKey(0)
    #         tan = abs(((y1 - y0) * (x3 - x2) - (y3 - y2) * (x1 - x0)) / ((y1 - y0) * (y3 - y2) + (x1 - x0) * (x3 - x2)))
    #         theta = math.atan(abs(tan)) * 180 / np.pi
    #         if theta > 0:
    #             row = closed.shape[0]
    #             col = closed.shape[1]
    #             for i in range(1, row):
    #                 if closed[i, 1] > 0:
    #                     cal = cal + 1
    #                 if closed[i, 5] > 0:
    #                     cal = cal + 1
    #                 if closed[i, 10] > 0:
    #                     cal = cal + 1
    #             if cal > 30:  # 这个阈值得根据场地拍摄情况进行选取
    #                 theta = -theta

    # theta_out = int(1000 * theta)  # 弧度制，乘1000保证精度
    # if y1 == y0:
    #     distance = 0
    #     angle = np.pi / 2
    # else:
    #     distance = (x1 - x0) * (H / 2 - y0) / (y1 - y0) + x0 - W / 2
    #     angle = math.atan((x0 - x1) / (y1 - y0))
    # distance_out = int(distance)
    # angle_out = int(1000 * angle)  # 弧度制，乘1000保证精度

    # print(angle_out)
    # print(distance_out)
    # print(theta_out)

    angle_out = int(0)
    distance_out = int(0)
    #theta_out = int(0)

    # hue_image0 = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    # low_range0 = np.array([20, 124, 103])
    # high_range0 = np.array([32, 255, 255])
    # th0 = cv2.inRange(hue_image0, low_range0, high_range0)

    # M = cv2.moments(th0)
    # if M["m00"]/255>10000:
    #     return angle_out, distance_out, 1000

    sp = frame.shape
    H = sp[0]
    W = sp[1]

    hue_image = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    low_range1 = np.array([0, 43, 215])
    high_range1 = np.array([25, 255, 255])
    th1 = cv2.inRange(hue_image, low_range1, high_range1)
    low_range2 = np.array([156, 43, 215])
    high_range2 = np.array([180, 255, 255])
    th2 = cv2.inRange(hue_image, low_range2, high_range2)
    th = th1 + th2

    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (21, 21))
    closed = cv2.morphologyEx(th, cv2.MORPH_CLOSE, kernel)

    cv2.imshow("closepics",closed)
    cv2.waitKey(0)

    contours, hierarchy = cv2.findContours(closed,cv2.RETR_TREE,cv2.CHAIN_APPROX_NONE)
    
    if len(contours)==0:
        return angle_out, distance_out
    
    # cnt=contours[0]
    # cnt1=contours[1]
    # print(cnt)
    # print(cnt1)
    # c = np.vstack((cnt, cnt1))
    # print(c)

    # temp=cnt.shape[0]
    # for i in range(len(contours)):
    #     if contours[i].shape[0]>temp:
    #         temp=contours[i].shape[0]
    #         cnt=contours[i]
    # if temp<500:
    #     return angle_out, distance_out

    atemp = contours[0]
    for i in range(1,len(contours)):
    	atemp = np.vstack((atemp, contours[i]))
    print(atemp)

    if atemp.shape[0]<500:
    	return angle_out, distance_out
    [vx,vy,x,y] = cv2.fitLine(atemp,cv2.DIST_L2,0,0.01,0.01)

    #[vx,vy,x,y] = cv2.fitLine(cnt,cv2.DIST_L2,0,0.01,0.01)
    if vy==0:
        angle=np.pi/2
    else:
        angle = -math.atan(vx/vy)
        distance = (vx*(H/2-y)/vy+x-W/2)*math.cos(angle)
    angle_out = int(1000*angle)
    distance_out = int(distance)
    print(angle_out, distance_out)
    return angle_out, distance_out

if __name__ == '__main__': # __name__ �ǵ�ǰģ��������ģ�鱻ֱ������ʱģ����Ϊ __main__ ����仰����˼���ǣ���ģ�鱻ֱ������ʱ�����´���齫�����У���ģ���Ǳ�����ʱ������鲻�����С�
    frame = cv2.imread('C:/Users/Administrator/Desktop/capture_green/85.jpg')
    readframe(frame)
    # cap=cv2.VideoCapture(0)
    # missframe_count=10
    # while missframe_count:
    #     cap.read()
    #     missframe_count=missframe_count-1
    # while cap.isOpened():
    #     angle_out,distance_out,theta_out=readframe(cap)
    #     print(type(angle_out))
    #     print(angle_out,distance_out,theta_out)
    # cap.release()
