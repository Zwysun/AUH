import serial
import struct
serial = serial.Serial('/dev/ttyAMA0',115200,timeout=2)
if serial.isOpen():
    pass
else:
    print('serial not open!')
    

def control_send(angle,distance):
# we choose < becaues both x86 (PC) and ARM (stm32) is little endian
    ang_byte_4 = struct.pack('<f',angle)
    dis_byte_4 = struct.pack('<f',distance)
    data=b'\xfe\x0f\x01\x23'+ang_byte_4+dis_byte_4+b'\xaa\xaa\x0a'
    serial.write(data)
    
def abc_send(abc_str):
    abc_bytes=abc_str.encode('utf-8')
    data=b'\xfe\x0f\x01\x25'+abc_bytes+b'\xaa\xaa\x0a'
    serial.write(data)
    


#连接串口

if serial.isOpen():
	print ('串口已打开')

	#data = b'Beking\r\n'    #发送的数据
	#data = b'Bkautyhdwing'    #发送的数据
	#angle=str(1.57)#do not tansfer angle as string, transfer is as float or Bytes
	#angle_b=angle.encode('ascii')# 
	data=b'\xfe\x0b\x01\x23'
	data=data+ang_byte_4+b'\xaa\xaa\x0a'
	#print(angle.encode('utf-8'))
	print(data)
	serial.write(data) # encode the string as utf-8
	#print ('You Send Data:',data)      for english letters and number, utf-8 is the same as ASCII
 
	while True:
		data = serial.read(5)    #串口读20位数据
		# if stm32 send data very slow, (e.g. 2s send once),then read(4) can only get 3 ones
		print ('receive data is :',data)
		print(i)
		i=i+1
		#if data != b'':
		#	break
	#print ('receive data is :',data)
	
else:
	print ('串口未打开')
 
 #data=b'\xfe\x11\x01\x231.57\xaa\xaa\x0a'
	#data_b=data.encode('utf-8')
 
#关闭串口
serial.close()
 
if serial.isOpen():
	print ('串口未关闭')
else:
	print ('串口已关闭')
