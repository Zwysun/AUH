
from PIL import Image
import pickle
import numpy as np
import cv2
# 下面这几行全局的代码，在别的.py文件中imprt 本文件时就会执行
# 其他py文件中如果使用的是from pic_cut_fun import getabc，也会执行本文件中的全局部分的程序
table = "ABCDEFGHIJKLM"
WHITE = 255
network_w=pickle.load(open('w_of_trainingdata_3_9.dat', 'rb'))
network_b=pickle.load(open('b_of_trainingdata_3_9.dat', 'rb'))
kernelY = cv2.getStructuringElement(cv2.MORPH_RECT, (10, 10))

def feedforward(weights, biases,a):
    """Return the output of the network if ``a`` is input.
    输入参数a的shape：(dim_x,1)
    """
    for b, w in zip(biases, weights):  # 计算每一层的输出，直到最后一层
        a = sigmoid(np.dot(w, a) + b)  # 对于二维矩阵，np.dot(w, a)是作矩阵乘法
    # print(np.shape(a))
    return a


def sigmoid(z):
    """The sigmoid function."""
    return 1.0/(1.0+np.exp(-z))




def add_white(im):
    w,h = im.size
    if w>h:
        big_l = int(w*1.3)
    else:
        big_l = int(h*1.3)
    big_im = Image.new('L', (big_l, big_l), WHITE)
    big_im.paste(im, (int((big_l-w)/2), int((big_l-h)/2)))
    return big_im




def get_single_abc(im_binary):
    #  im_binary is a matrix
    im = Image.fromarray(im_binary)
    '''
    im = add_white(im)
    im=im.resize((28,28))
    '''
    test_data = np.matrix(im.getdata(), dtype='float') / 255.0
    test_data = np.transpose(test_data)
    print(test_data.shape)
    im_y = feedforward(network_w, network_b, test_data)
    abcstr = table[np.argmax(im_y)]
    return abcstr

def get_single_abc_cv2(im_binary):
    #  im_binary is a matrix
    ''''''
    w=im_binary.shape[0]
    h=im_binary.shape[1]
    if w>h:
        edge = int(w*0.3)
    else:
        edge = int(h*0.3)

    #im = add_white(im_binary)
    im = cv2.copyMakeBorder(im_binary, edge, edge, edge, edge, cv2.BORDER_CONSTANT, value=[255, 255, 255])
    #print(im.shape)
    im = cv2.resize(im,(28, 28))
    #print(im)
    test_data = np.reshape(im,(28*28,1),order='C') / 255.0 # 注意order='C',这样是对的
    #print(test_data.shape)
    im_y = feedforward(network_w, network_b, test_data)
    abc_index = np.argmax(im_y)
    abcstr = table[abc_index]
    return abcstr, abc_index

def getabc_v2(img3):
    img = cv2.cvtColor(img3, cv2.COLOR_BGR2GRAY)
    canny = cv2.Canny(img, 10, 500)
    img_close = cv2.morphologyEx(canny, cv2.MORPH_CLOSE, kernelY)
    _, _, stats, centroids = cv2.connectedComponentsWithStats(img_close.copy())
    img_eqhist = cv2.equalizeHist(img)
    abcstr = []
    abcindex = []
    index_arg = np.argsort(stats[:, 0])
    for i in index_arg:
        x_i = stats[i, 0]
        y_i = stats[i, 1]
        width_i = stats[i, 2]
        heigth_i = stats[i, 3]
        area_i = stats[i, 4]
        #wRh = width_i / heigth_i
        #print(i, 'th,area=', area, 'width/height=', wRh)
        if area_i > 200 and area_i < 10000:  # and (wRh <0.5 or wRh > 0.8):
            sub_pic_i = img_eqhist[y_i:y_i + heigth_i, x_i:x_i + width_i]
            #cv2.imshow('sub' + str(i), sub_pic_i)
            th_i, sub_pic_i = cv2.threshold(sub_pic_i, 0, 255, cv2.THRESH_OTSU)
            #cv2.imshow('sub_th' + str(i), sub_pic_i)
            abcstr_i, abc_index_i = get_single_abc_cv2(sub_pic_i)
            abcstr.append(abcstr_i)
            abcindex.append(abc_index_i)
            #print(i, abcstr)
    return abcstr, abcindex

'''
img3 = cv2.imread('../abcpic/620ab.jpg')
abc_count = np.zeros((13,4))
for i in range(40):
    abcstr, abcindex = getabc_v2(img3)
    abc_count[abcindex[0],0]+=1
    abc_count[abcindex[1],1]+=1
    abc_count[abcindex[2], 2] += 1
    abc_count[abcindex[3], 3] += 1

abcstr = []
for i in range(4):
    abcstr.append(table[np.argmax(abc_count[:,i])])
print(abcstr)
#abc_count=abc_count.reshape(13,order='F')
#max_index =  np.argsort(abc_count[:, 0])
#max_index=np.argpartition(abc_count, -4)[-4:]
print(abc_count)
#print(max_index)
print(np.shape(abcindex))
'''
#w=pickle.load(open('weights.dat', 'rb'))
#b=pickle.load(open('biases.dat', 'rb'))
# w_of_trainingdata_3_2.dat 错2个,
# #事实证明，3_2在mutiabc3全对，在true_abc3错一个E
# 3_3错四个
# 3_5错三个
#w=pickle.load(open('good_w_v1.dat', 'rb'))
#b=pickle.load(open('good_b_v1.dat', 'rb'))

#print("in pic cut fun")


#print("C B A M M A B I C K M A F")
'''
im_test = Image.open("11-34.png")
im_test = im_test.convert('L')
im_test=im_test.resize((28,28))
test_data = np.matrix(im_test.getdata(), dtype='float') / 255.0
test_data = np.transpose(test_data)
#print(np.shape(test_data))
im_y = feedforward(w, b, test_data)
#print(np.argmax(im_y))
print(table[np.argmax(im_y)], end=" ")
'''
