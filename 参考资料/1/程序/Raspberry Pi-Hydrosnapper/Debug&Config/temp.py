#!/usr/bin/python
# -*- coding: UTF-8 -*-
 
# 获取列表的第二个元素
def takeSecond(elem):
    return elem[-1]
 
# 列表
random = [((2, 2),5), (3, 4), (4, 1), (1, 3)]
cnt=[]
for i in range(4):
	temp = random[i]
	cnt.append([temp,4-i])

# 指定第二个元素排序
cnt.sort(key=lambda elem: elem[-1])
 

for i in range(4):
	temp = cnt[i]
	temp.pop()
	cnt[i] = temp[0]
print('排序列表：', cnt)