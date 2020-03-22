import cv2
import pytesseract

# 图片读取
img_path = 'C:/Users/zwysu/Desktop/1.jpg'
original_img = cv2.imread(img_path)
# 灰度处理
gray_img = cv2.cvtColor(original_img, cv2.COLOR_BGR2GRAY)

'''
print("cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)结果如下：")
print('大小:{}'.format(gray_img.shape))
print("类型：%s" % type(gray_img))
print(gray_img)
'''
# 二值化处理， 重点 阈值设置 180
ret, binary_img = cv2.threshold(gray_img, 180, 255, cv2.THRESH_BINARY)

# 寻找所有外围轮廓
cnts, _ = cv2.findContours(binary_img, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

# 将轮廓按面积大小降序排序
cnts_sorted = sorted(cnts, key=cv2.contourArea, reverse=True)

# 取出最大轮廓的矩形坐标
x, y, w, h = cv2.boundingRect(cnts_sorted[0])

# 进行裁剪并反色
target_img = 255-binary_img[y:(y + h), x:(x + w)]

# 进行开运算，除去毛刺
kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (2, 3))
after_opening = cv2.morphologyEx(target_img, cv2.MORPH_OPEN, kernel)

'''
# 字符分割，待定闭运算  填充黑点？
cnts2, _ = cv2.findContours(after_opening, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
characters = sorted(cnts2, key=cv2.contourArea, reverse=True)
x, y, w, h = cv2.boundingRect(characters[5])
one = after_opening[y:(y + h), x:(x + w)]
cv2.imwrite('C:/Users/zwysu/Desktop/c.jpg', one)
cv2.imwrite('C:/Users/zwysu/Desktop/b2.jpg', after_opening)
'''
content = pytesseract.image_to_string(after_opening)  # 解析图片
print(content)

cv2.imwrite('C:/Users/zwysu/Desktop/open.jpg', after_opening)
cv2.imwrite('C:/Users/zwysu/Desktop/target.jpg', target_img)
cv2.imwrite('C:/Users/zwysu/Desktop/gray.jpg', gray_img)
cv2.imwrite('C:/Users/zwysu/Desktop/b.jpg', binary_img)
