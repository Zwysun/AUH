import serial
ser = serial.Serial("/dev/ttyAMA0",115200)
str='fe00002502020203000a0a'
ser.write(bytes.fromhex(str))
