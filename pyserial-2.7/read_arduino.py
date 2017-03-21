#!/usr/bin/env python2.7

#import Gnuplot
import serial
import sys

ser = serial.Serial('/dev/tty.usbmodem1d11',57600)

from starmap_arduino import serial_dic 

#serial_dic = {}

n = 0
while True:
	n += 1
	read_line = ser.readline()
	print read_line
	read_line_list = read_line.replace('\r\n','').split(' : ')
	if len(read_line_list) > 1:
		print read_line_list
		if read_line_list[0] not in serial_dic.keys():
			serial_dic[read_line_list[0]] = [read_line_list[1]]
		else:
			serial_dic[read_line_list[0]].append(read_line_list[1].replace('\n',''))
	print n
	if n % 10 == 0:
		print serial_dic.keys()


		write_line = 'serial_dic = %s' %serial_dic
		write_file = open('starmap_arduino.py','w')
		write_file.write(write_line)
		write_file.close()





