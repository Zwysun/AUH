# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'testabc_test.py'
#
# Created by: PyQt5 UI code generator 5.11.3
# 警告 ！！！不要对这个文件用PYUIC，否则该文件会被全部清空
# 要对.ui后缀名的文件用PYUIC
#https://blog.csdn.net/weixin_43008870/article/details/86492782
# WARNING! All changes made in this file will be lost!

from UpperMachine import Ui_CamShow
import sys
from PyQt5.QtWidgets import QApplication,QMainWindow,QFileDialog
from PyQt5.QtCore import QTimer,QCoreApplication
from PyQt5.QtGui import QPixmap
from PyQt5 import QtGui
import cv2
import qimage2ndarray
import time
import pic2abc
import pic_cut

class CamShow(QMainWindow,Ui_CamShow):
    def __init__(self,parent=None):
        #在实例化CamShow类时，会自动调用它的初始化函数，类似于C++中的构造函数
        super(CamShow,self).__init__(parent)
        #  super函数实现了类的继承，将QMainWindow,Ui_CamShow的各种属性过继给CamShow类
        self.setupUi(self)  # setupUI函数则是从Ui_CamShow中继承过来的函数，用来实现程序界面的创建和属性设置。
        # 以下是自定义函数
        self.PrepWidgets()  # 控件初始化函数
        self.PrepParameters()  # 参数初始化
        self.CallBackFunctions()
        self.Timer =QTimer()
        self.Timer.timeout.connect(self.TimerOutFun)

    def PrepWidgets(self):  # 初始化各控件
        self.PrepCamera()  # 摄像头初始化函数
        self.Bt_pause.setEnabled(False)
        self.Bt_save.setEnabled(False)
        # self.GrayImgCkB.setEnabled(False)
    def PrepCamera(self): #  摄像头初始化函数
        try:
            self.camera=cv2.VideoCapture(0)
            #  调用OpenCV的VideoCapture函数打开摄像头，并使用变量self.camera代表该摄像头，参数0意味着打开笔记本自带的摄像头
            self.MsgTE.clear()
            self.MsgTE.append('Oboard camera connected.')
            #  append函数意思是在现有的内容后继续添加内容，
            #  除此之外，textedit控件还有setPlainText函数，意识是用括号中的文本覆盖原来的文本
        except Exception as e:
            self.MsgTE.clear()
            self.MsgTE.append(str(e))
            #  当程序出错时，程序不会自动退出，而且我们可以通过文本框MsgTE查看出错信息
    def PrepParameters(self):
        self.RecordFlag=0  # 为0时不保存视频，为1才时开始保存视频
        self.save_path='D:/浙大的东西/课程综合设计/上位机/python上位机/pic/'  # 默认的文件存储路径
        # FilePathLE控件名字     save_path:变量名
        self.FilePathLE.setText(self.save_path)  # 将路径名显示在文本框
        self.Image_num=0  # 读取图片的次数(用于计算帧率)
        self.R=1
        self.G=1
        self.B=1
        #  摄像头参数读取函数为self.camera.get()，括号中的不同序号代表不同的参数，
        #  具体的对应关系可见链接https://baike.baidu.com/item/cvGetCaptureProperty/1637973?fr=aladdin
        '''
        self.ExpTimeSld.setValue(self.camera.get(15))
        self.SetExposure()  # 曝光度
        self.GainSld.setValue(self.camera.get(14))
        self.SetGain()  # 增益
        self.BrightSld.setValue(self.camera.get(10))
        self.SetBrightness()  # 亮度
        self.ContrastSld.setValue(self.camera.get(11))
        self.SetContrast()  # 对比度
        '''
        self.MsgTE.clear()

    def CallBackFunctions(self):
        #  这个是总的回调函数，定义了每一个功能控件的触发条件和相应的回调函数
        self.Bt_path.clicked.connect(self.SetFilePath)
        self.Bt_begin.clicked.connect(self.StartCamera)  # 开始 StartCamera等函数都是自己定义的
        self.Bt_pause.clicked.connect(self.StopCamera)  # 暂停
        self.Bt_save.clicked.connect(self.RecordCamera)
        self.Bt_exit.clicked.connect(self.ExitApp)
        #self.GrayImgCkB.stateChanged.connect(self.SetGray)
    def StartCamera(self):
        self.Bt_begin.setEnabled(False)
        self.Bt_pause.setEnabled(True)
        self.Bt_save.setEnabled(True)
        #self.GrayImgCkB.setEnabled(True)
        self.Bt_save.setText('录像')
        self.Timer.start(1)
        self.timelb = time.process_time()
        # 计时周期为1ms，即每隔1ms程序会自动调用一次TimerOutFun。
        # 我们将图像的具体读取和显示放到TimerOutFun中，这样就可以实现图像的实时读取和显示了

    def StopCamera(self):
        #  意思就是说，放视频的时候显示录像二字，点暂停显示保存二字
        if self.Bt_pause.text()=='暂停':
            self.Bt_pause.setText('继续')
            self.Bt_save.setText('保存')
            self.Timer.stop()
        elif self.Bt_pause.text()=='继续':
            self.Bt_pause.setText('暂停')
            self.Bt_save.setText('录像')
            self.Timer.start(1)

    def TimerOutFun(self):
        #  我们将需要对每一帧图像都重复执行的操作放在函数TimerOutFun中，
        # 该函数的功能为：从摄像头读取图像，调用ColorAdjust函数来调整图片的颜色（这个功能本程序没做），
        # 调用DispImg函数来显示图像，保存视频，获取并显示摄像头帧频和图像尺寸
        success,img=self.camera.read()
        if success:
            self.Image = img  # self.ColorAdjust(img)
            self.DispImg() # 这是自定义的函数 ,显示一帧图像
            #cv2.imshow('Capture',self.Image)
            #self.RecImg()# this function is good in PC but cannot work in PI
            
            self.Image_num+=1
            if self.RecordFlag: # RecordFlag为0时不保存视频，为1才时开始保存视频
                self.video_writer.write(img)  # opencv的存储函数
            if self.Image_num%10==9:
                frame_rate=10/(time.process_time()-self.timelb)
                self.FmRateLCD.display(frame_rate)  # 显示帧率
                self.timelb=time.process_time()
                # 获取并计算帧频则是通过time.clock()函数获取代码执行的时间实现
                #size=img.shape
                self.ImgWidthLCD.display(self.camera.get(3))
                self.ImgHeightLCD.display(self.camera.get(4))
        else:
            self.MsgTE.clear()
            self.MsgTE.setPlainText('Image obtaining failed.')
    def DispImg(self):
        
        #if self.GrayImgCkB.isChecked(): # 本程序没有做灰度
            #img = cv2.cvtColor(self.Image, cv2.COLOR_BGR2GRAY)
        #else:
        # Label可接受的图像类型为QPixmap，且其颜色通道依次为R、G、B；而通过OpenCV的read函数读取
        # 的图像是3维矩阵格式的，且颜色通道依次为B、G、R，因此需要先调用cvtColor函数对矩阵的颜
        # 色进行调整，然后再调用array2qimage函数将其转为QImage格式，再通过QPixmap函数转
        # 为QPixmap格式进行显示。
        img=cv2.resize(self.Image,(640,480))
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        qimg = qimage2ndarray.array2qimage(img)
        
        #height, width, channel = img.shape
        #bytesPerLine = 3 * width
        #qimg = QImage(img, width, height, bytesPerLine, QImage.Format_RGB888)
        self.label_pic.setPixmap(QPixmap(qimg))
        self.label_pic.show()
        '''
        show = cv2.resize(self.Image, (640, 480))
        show = cv2.cvtColor(show, cv2.COLOR_BGR2RGB)
        # print(show.shape[1], show.shape[0])
        showImage = QtGui.QImage(show.data, show.shape[1], show.shape[0], QtGui.QImage.Format_RGB888)
        self.label_pic.setPixmap(QtGui.QPixmap.fromImage(showImage))
        '''
        
    def RecImg(self):# this function is good in PC but cannot work in PI
        # 这个函数在TimerOutFun中被调用，用于识别当前帧中的字母
        # self.Image 是矩阵，例如：img=self.Image
        # B = img[:, :, 0]
        # G = img[:, :, 1]
        # R = img[:, :, 2]
        im_gray = cv2.cvtColor(self.Image, cv2.COLOR_BGR2GRAY)
        L_im_b,L = pic_cut.cut(im_gray)
        '''
        for i in range(L):
            abcstr,_=pic2abc.getabc(L_im_b[i])
            qim2
        '''
        if L>0 :
            abcstr,im_index = pic2abc.getabc(L_im_b[0])
            qim2 = qimage2ndarray.array2qimage(L_im_b[0])

            #height, width= L_im_b[0].shape
            #bytesPerLine = 3 * width
            #qim2 = QImage(L_im_b[0], width, height, bytesPerLine,QImage.Format_Invalid)

            #abcstr=self.Image.shape()
            #print(abcstr)
            self.TE_abcresult.setPlainText(abcstr)
            self.LCD_imindex.display(im_index)
            self.label_abc.setPixmap(QPixmap(qim2))
        #self.label_abc.show()



    def SetFilePath(self):
        dirname = QFileDialog.getExistingDirectory(self, "浏览", '.')
        # 通过调用QFileDialog.getExistingDirectory函数弹出对话框，
        # 让用户自己选择路径，并将选择好的路径显示在FilePathLE中
        if dirname:
            self.FilePathLE.setText(dirname)
            self.save_path=dirname+'/'

    def RecordCamera(self):
        # 本函数实现保存图片和保存视频两种功能。如果按钮显示的是‘保存’，单击后就
        # 调用OpenCV的imwrite函数保存一幅图片，图片的名称会根据时间自动命名。
        # 如果按钮显示的是‘录像’，单击后就调用OpenCV的VideoWriter函数创建一个视频文件，文
        # 件名同样根据时间自动命名，同时将录像标签RecordFlag改为1，这样在TimerOutFun中每次读取
        # 图片后都会将其写入视频文件中。如果按钮显示的是‘停止’，单击后就停止录像，保存视频文件
        tag=self.Bt_save.text()
        if tag=='保存':
            try:
                image_name=self.save_path+'image'+time.strftime('%Y%m%d%H%M%S',time.localtime(time.time()))+'.jpg'
                print(image_name)
                #cv2.imwrite(image_name, self.Image)
                cv2.imencode('.jpg', self.Image)[1].tofile(image_name)
                self.MsgTE.clear()
                self.MsgTE.setPlainText('Image saved.')
            except Exception as e:
                self.MsgTE.clear()
                self.MsgTE.setPlainText(str(e))
        elif tag=='录像':
            self.Bt_save.setText('停止')

            video_name = self.save_path + 'video' + time.strftime('%Y%m%d%H%M%S',time.localtime(time.time())) + '.avi'
            fps = self.FmRateLCD.value()
            size = (self.Image.shape[1],self.Image.shape[0])
            fourcc = cv2.VideoWriter_fourcc('M', 'J', 'P', 'G')
            self.video_writer = cv2.VideoWriter(video_name, fourcc,self.camera.get(5), size)
            self.RecordFlag=1
            self.MsgTE.setPlainText('Video recording...')
            self.Bt_pause.setEnabled(False)
            self.Bt_exit.setEnabled(False)
        elif tag == '停止':
            self.RecordBt.setText('录像')
            self.video_writer.release()
            self.RecordFlag = 0
            self.MsgTE.setPlainText('Video saved.')
            self.Bt_pause.setEnabled(True)
            self.Bt_exit.setEnabled(True)
    def ExitApp(self):
        self.Timer.Stop()
        self.camera.release()
        self.MsgTE.setPlainText('Exiting the application..')
        QCoreApplication.quit()




if __name__ == '__main__':
    app = QApplication(sys.argv)
    ui=CamShow()
    ui.show()# this function maybe in the father class 
    sys.exit(app.exec_())
