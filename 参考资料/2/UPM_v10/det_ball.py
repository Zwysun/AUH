import cv2
import numpy as np
from matplotlib import pyplot as plt

def rec_ball_v1(img3):
    # get im_binary by rgb
    img_b = img3[:, :, 0]
    img_g = img3[:, :, 1]
    img_r = img3[:, :, 2]
    img3_b = img_b * 0
    img3_b_index = np.logical_and(img_b < 80, img_r > 100)
    img3_b_index = np.logical_and(img_g > 100, img3_b_index)
    img3_b[img3_b_index] = 255
    _, _, stats, centroids = cv2.connectedComponentsWithStats(img3_b.copy())
    if stats.shape[0] ==1:
        distance =600

    try:
        area = stats[1:-1, 4]
        index = np.argmax(area) + 1
        #print(centroids[index, :])
        if area[index-1] < 500 :
            distance =700
        else:
            xx = centroids[index, 0]
            yy = centroids[index, 1]
            distance = ((xx - 320) ** 2 + (yy - 240) ** 2) ** 0.5
    except:
        distance = 800

    if distance <=200:
        stop_flag =1
    else:
        stop_flag = 0

    '''
    print('distance=', distance)
    plt.figure(1)
    plt.imshow(img3_b, cmap='gray')
    plt.plot(xx, yy, 'bo')
    '''

    return stop_flag,distance,img3_b
    #print('distance=', distance)

'''
img3 = cv2.imread('../pic_v5/aa640rot_v3.jpg')
print(rec_ball_v1(img3))

plt.show()
cv2.waitKey(0)
cv2.destroyAllWindows()
'''