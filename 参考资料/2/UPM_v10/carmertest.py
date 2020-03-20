import cv2
#初期化USB摄像头
cap = cv2.VideoCapture(1)
while( cap.isOpened() ):
	#USB摄像头工作时,读取一帧图像
    ret, frame = cap.read()
    #显示图像窗口在树莓派的屏幕上
    cv2.imshow('Capture',frame)
    #按下q键退出
    key = cv2.waitKey(1)
    #print( '%08X' % (key&0xFFFFFFFF) )
    if key & 0x00FF  == ord('q'):
        break
# 释放资源和关闭窗口
cap.release()
cv2.destroyAllWindows()
