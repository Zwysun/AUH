
import time
import cv2
import light2line_v2
import pic2abc
import det_ball
import numpy as np
#from PyQt5.QtCore import QTimer, QCoreApplication
table = "ABCDEFGHIJKLM"
''''''
import serial
import struct
serial = serial.Serial('/dev/ttyAMA0', 115200, timeout=2)
if serial.isOpen():
    pass
else:
    print('serial not open!')


def control_send(ang,distance,have_ball_flag):
    # we choose < becaues both x86 (PC) and ARM (stm32) is little endian
    ang_byte_4 = struct.pack('<f', ang)
    dis_byte_4 = struct.pack('<f', distance)
    data = b'\xfe\x0f\x01\x23' + ang_byte_4 + dis_byte_4 + b'\xaa\xaa\x0a'
    serial.write(data)

    if have_ball_flag:
        data = b'\xfe\x08\x01\x26\xab\xaa\xaa\x0a'
        serial.write(data)
        serial.write(data)
        serial.write(data)
    else:
        pass


def abc_send(abc_str):
    data = b'\xfe\x0f\x01\x25'
    for i in range(4):
        abc_bytes = abc_str[i].encode('utf-8')
        data = data + abc_bytes
    
    data = data + b'\xaa\xaa\x0a'    
    serial.write(data)

def make_video():
    cap0 = cv2.VideoCapture(0)  # 默认的摄像头
    frame_num=0
    ball_count = 0
    begin_ball = 0
    stop_falg = 0
    det_ball_flag = 1
    while (cap0.isOpened()):#and cap1.isOpened()):
        frame_num += 1
        success0, frame0 = cap0.read()

        if success0 :#and success1:
            cv2.imshow('video0', frame0)
            # 等待按键q操作关闭摄像头
            
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
        else:
            break
        if frame_num%5 == 0:
            #rec_abc_2()
            ang, distance, im_b= light2line_v2.get_ang_dis_HT_v3(cv2.cvtColor(frame0, cv2.COLOR_BGR2GRAY))
            if det_ball_flag:
                have_ball_flag,distance_ball,img3_b = det_ball.rec_ball_v1(frame0) #before ball
            else:
                have_ball_flag = 0
                
                
            if have_ball_flag:
                det_ball_falg=0  #di yi ci buzhuo dao huangqiu shi jinru , zai tong yi huangqiu chu,dierci bu zhuo dao shi buhui jinru
                ball_count += 1
                control_send(ang, distance, have_ball_flag)
                rec_abc_2()
            
            if det_ball_flag == 0 and frame_num % 150 ==0:
                det_ball_flag = 1
                frame_num=0
               
            '''
            if have_ball_flag ==1 and begin_ball == 0:
                ball_count += 1     # 刚开始进入黄球
                begin_ball = 1      
                stop_flag = 1
            elif have_ball_flag == 1 and begin_ball ==1:
                stop_flag = 0       # 在黄球的过程中
            elif have_ball_flag == 0 and begin_ball == 1:
                begin_ball = 0      # 刚离开黄球
                stop_flag = 0
            else:
                stop_flag = 0         # 进入黄球前和离开黄球后
            '''
            
            control_send(ang, distance, have_ball_flag)
            '''
            if ball_count ==1:
                rec_abc() #识别字母并发送
                ball_count =2
                #pass
            elif ball_count == 2:
                ball_count = 2
            else:
                pass
            '''
            
            '''
            if ball_count ==3:
                #rec_abc() #识别字母并发送
                ball_count =4
                #pass
            elif ball_count == 4:
                ball_count = 4
            else:
                pass
            '''


            #cv2.imshow('video1_binary', img3_b)
            #cv2.imshow('video0', frame0)
            #print(stop_flag)
            #cv2.imwrite('pic/sw3'+str(frame_num)+'s.jpg',frame0)
            #cv2.cv.SaveImage('\pic\ '+str(frame_num)+'.jpg',frame0)
            #frame_num = 0

    cap0.release()
    #cap1.release()
    cv2.destroyAllWindows()

def rec_abc():
    cap1 = cv2.VideoCapture(1)
    frame_num = 0
    abc_count = np.zeros((13, 4))
    while (cap1.isOpened()):
        #frame_num+=1
        success1, frame1 = cap1.read()
        if success1:
            frame_num += 1
            #cv2.imshow('video1',frame1)
        else:
            break

        #if frame_num%5 == 0 and frame_num < 11:
        if frame_num < 3:
            _,abcindex = pic2abc.getabc_v2(frame1) # 共识别16次
            if len(abcindex) == 4:
                #print(abcindex)
                abc_count[abcindex[0], 0] += 1
                abc_count[abcindex[1], 1] += 1
                abc_count[abcindex[2], 2] += 1
                abc_count[abcindex[3], 3] += 1
            else:
                abc_count[0,0]+=1
                abc_count[0,1]+=1
                abc_count[0,2]+=1
                abc_count[0,3]+=1
        
            '''
            try:
                _,abcindex = pic2abc.getabc_v2(frame1) # 共识别16次
                #print(abcindex)
                abc_count[abcindex[0], 0] += 1
                abc_count[abcindex[1], 1] += 1
                abc_count[abcindex[2], 2] += 1
                abc_count[abcindex[3], 3] += 1
                #abc_send(abc_str)
            except:
                abc_count[0,0]+=1
                abc_count[0,1]+=1
                abc_count[0,2]+=1
                abc_count[0,3]+=1
            '''
        elif frame_num>=3:
            break
    
    abc_str = []
    for i in range(4):
        abc_str.append(table[np.argmax(abc_count[:, i])])

    abc_send(abc_str)

    cap1.release()
    cv2.destroyAllWindows()

def rec_abc_2():
    abc_count = np.zeros((13, 4))
    abc_str = []
    for i in range(4):
        abc_str.append(table[np.argmax(abc_count[:, i])])

    abc_send(abc_str)
    
def make_video2():
    cap0 = cv2.VideoCapture(0)  # 默认的摄像头
    success0, frame0 = cap0.read()
    #cap1 = cv2.VideoCapture(0)


    while (cap0.isOpened()):#and cap1.isOpened()):
        '''
        while cap0.grab():
            #print('pass')
            pass
            #print('22')
            '''
        #for ii in range(10):
        cap0.grab()

        cap0.retrieve(frame0)
        cv2.imshow('video0', frame0)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

        #frame1= np.transpose(frame0, axes=(1, 0, 2))
        #frame1 = cv2.flip(frame1,1)

    cap0.release()
    #cap1.release()
    cv2.destroyAllWindows()



if __name__ == '__main__':
    make_video()




'''
class CameraShow():
    def __init__(self, parent=None):
        # 在实例化CamShow类时，会自动调用它的初始化函数，类似于C++中的构造函数
        self.PrepParameters()  # 参数初始化
        self.Timer = QTimer()
        #self.Timer=time.time()
        self.Timer.timeout.connect(self.TimerOutFun)

    def PrepCamera(self):  # 摄像头初始化函数
        try:
            self.camera = cv2.VideoCapture(0)
            #  调用OpenCV的VideoCapture函数打开摄像头，并使用变量self.camera代表该摄像头，参数0意味着打开笔记本自带的摄像头
            print('Oboard camera connected.')
            #  append函数意思是在现有的内容后继续添加内容，
            #  除此之外，textedit控件还有setPlainText函数，意识是用括号中的文本覆盖原来的文本
        except Exception as e:
            print(str(e))
            #  当程序出错时，程序不会自动退出，而且我们可以通过文本框MsgTE查看出错信息

    def PrepParameters(self):
        self.Image_num = 0  # 读取图片的次数(用于计算帧率)


    def StartCamera(self):

        self.Timer.start(1)
        self.timelb = time.process_time()
        # 计时周期为1ms，即每隔1ms程序会自动调用一次TimerOutFun。
        # 我们将图像的具体读取和显示放到TimerOutFun中，这样就可以实现图像的实时读取和显示了

    def TimerOutFun(self):
        #  我们将需要对每一帧图像都重复执行的操作放在函数TimerOutFun中，
        # 该函数的功能为：从摄像头读取图像，调用ColorAdjust函数来调整图片的颜色（这个功能本程序没做），
        # 调用DispImg函数来显示图像，保存视频，获取并显示摄像头帧频和图像尺寸
        success, img = self.camera.read()
        if success:
            img = np.transpose(img,axes=(1,0,2))
            self.Image=cv2.flip(img, 1)
            #self.Image = img

            self.DispImg()  # 这是自定义的函数 ,显示一帧图像
            #cv2.imshow('Capture',self.Image)
            # self.RecImg()# this function is good in PC but cannot work in PI

            self.Image_num += 1
            if self.RecordFlag:  # RecordFlag为0时不保存视频，为1才时开始保存视频
                self.video_writer.write(img)  # opencv的存储函数
            if self.Image_num % 10 == 9:
                frame_rate = 10 / (time.process_time() - self.timelb)
                self.FmRateLCD.display(frame_rate)  # 显示帧率
                #print(np.shape(img))
        else:
            print('Image obtaining failed.')

    def DispImg(self):

        # if self.GrayImgCkB.isChecked(): # 本程序没有做灰度
        # img = cv2.cvtColor(self.Image, cv2.COLOR_BGR2GRAY)
        # else:
        # Label可接受的图像类型为QPixmap，且其颜色通道依次为R、G、B；而通过OpenCV的read函数读取
        # 的图像是3维矩阵格式的，且颜色通道依次为B、G、R，因此需要先调用cvtColor函数对矩阵的颜
        # 色进行调整，然后再调用array2qimage函数将其转为QImage格式，再通过QPixmap函数转
        # 为QPixmap格式进行显示。
        img = cv2.resize(self.Image, (480,640))

        #_,_,p1,p2,contours,im_b=light2line.getkt(cv2.cvtColor(img,cv2.COLOR_BGR2GRAY))
        #p1=(0,0)
        #p2=(300,300)
        #img =cv2.line(img, p1, p2, (0,255,0),3)

    def RecImg(self):  #

        im_gray = cv2.cvtColor(self.Image, cv2.COLOR_BGR2GRAY)
        k,t=light2line.getkt(im_gray)



if __name__ == '__main__': #会先执行本文件上面全局部分的代码，然后再执行这个main
    #app = QApplication(sys.argv)#当这个py文件被其他文件import时，只会执行上面的全局部分的代码，不会执行main
    ui = CameraShow()
    ui.show()  # this function maybe in the father class
    #sys.exit(app.exec_())
'''