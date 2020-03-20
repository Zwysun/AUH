# -*- coding: utf-8 -*-
import os
import cv2
import gc
import time
from multiprocessing import Process, Manager

def capframe(stack, cam, top):

    print('Process to write: %s' % os.getpid())
    cap = cv2.VideoCapture(cam)
    cap.set(cv2.CAP_PROP_FPS, 60)
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
    # 启动子进程pr，读取:
    #pr.start()
    while True:
    	#time.sleep(1)
        while len(q) == 0:
        	pass
        value = q.pop()
        cv2.imshow("img", value)
        key = cv2.waitKey(1) & 0xFF
        if key == ord('q'):
            break
    #pr.join()
    # pw进程里是死循环，无法等待其结束，只能强行终止:
    pw.terminate()
