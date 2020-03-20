import cv2
# 腐蚀膨胀所用到的核
kernelX = cv2.getStructuringElement(cv2.MORPH_RECT, (20, 5))
kernelx = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 1))
kernelY = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 20))
kernely = cv2.getStructuringElement(cv2.MORPH_RECT, (1, 5))
kernelL = cv2.getStructuringElement(cv2.MORPH_RECT, (50, 5))
kernels = cv2.getStructuringElement(cv2.MORPH_RECT, (6, 6))
kernelc = cv2.getStructuringElement(cv2.MORPH_RECT, (2, 2))

def cut(img_gray):
    GBlur = cv2.GaussianBlur(img_gray, (3, 3), 0)
    canny = cv2.Canny(GBlur, 50, 150)
    img_de = cv2.dilate(canny, kernelX, iterations=2)
    img_de = cv2.erode(img_de, kernelX, anchor=(-1, -1), iterations=2)
    img_de = cv2.erode(img_de, kernelx, anchor=(-1, -1), iterations=1)
    img_de = cv2.erode(img_de, kernely, anchor=(-1, -1), iterations=1)
    img_de = cv2.erode(img_de, kernelL, anchor=(-1, -1), iterations=2)
    img_de = cv2.dilate(img_de, kernelL, anchor=(-1, -1), iterations=2)
    img_de = cv2.dilate(img_de, kernelX, anchor=(-1, -1), iterations=2)
    img_de = cv2.dilate(img_de, kernels, anchor=(-1, -1), iterations=2)
    img_de = cv2.medianBlur(img_de, 15)
    contours,_= cv2.findContours(img_de.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    # the above function has 3 return values in opencv 3,but has 2 return values in my PC
    # the verson of opencv in PI is opencv3 , my PC's opencv is opencv4 
    lpImage = []
    #print(len(contours))
    for c in contours:
        # 边界框
        x, y, w, h = cv2.boundingRect(c)
        im_binary=img_gray[y:y + h, x:x + w]
        im_binary[im_binary > 60] = 255
        im_binary[im_binary <= 60] = 0
            # 腐蚀膨胀的函数是对白色前景黑色背景的图像而言的
            # 我们的图中，字母是黑的而背景是白的，因此要膨胀字母，等价于腐蚀白色背景
            # 因此这里对im_binary进行腐蚀操作
        im_binary = cv2.erode(im_binary, kernelc, iterations=1)
        lpImage.append(im_binary)
    return lpImage,len(lpImage)




