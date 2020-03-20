import tensorflow as tf
from PIL import Image     
import numpy as np
from tensorflow.examples.tutorials.mnist import input_data
data = ['A','B','C','D','E','F','G','H','I','J','K','L','M','N']

with tf.Session() as sess:
    graph = tf.get_default_graph()
    tf.saved_model.loader.load(sess,[tf.saved_model.tag_constants.TRAINING],r'/home/pi/Desktop/Python_zjz/Model')
    input_x = sess.graph.get_tensor_by_name('input/input_x:0')#传入的参数会在后文讲到的
    keep_prob = sess.graph.get_tensor_by_name('dropout/input_prob:0')
    y_conv = sess.graph.get_tensor_by_name('y_conv:0')

    try:
      f =open('zjz.txt')
      f.close()
      status = 0
    except IOError:
      print ('no')
      status = 1

    while(status):
        print('wait')
    imE = Image.open('p1.jpg')
    imF = Image.open('p2.jpg')
    imG = Image.open('p3.jpg')
    imH = Image.open('p4.jpg')

    mE = np.asarray(imE)/255
    mF = np.asarray(imF)/255
    mG = np.asarray(imG)/255
    mH = np.asarray(imH)/255

    mE = np.array(mE).reshape(1,784)
    mF = np.array(mF).reshape(1,784)
    mG = np.array(mG).reshape(1,784)
    mH = np.array(mH).reshape(1,784)
    
    test_images = np.zeros((4, 784), dtype=np.float)
    test_images[0] = mE
    test_images[1] = mF
    test_images[2] = mG
    test_images[3] = mH
    result = sess.run(y_conv,feed_dict = {input_x:test_images,keep_prob:1.0})
with open("zjz.txt","w") as f:
    L =  np.argmax(result[0])
    f.writelines(data[L])
    L =  np.argmax(result[1])
    f.writelines(data[L])
    L =  np.argmax(result[2])
    f.writelines(data[L])
    L =  np.argmax(result[3])
    f.writelines(data[L])
f.close()

    

