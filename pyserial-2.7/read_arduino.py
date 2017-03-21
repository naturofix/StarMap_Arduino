#!/usr/bin/env python2.7

#import Gnuplot
import serial
import sys

ser = serial.Serial('/dev/tty.usbmodem1d11',57600)

from starmap_arduino import serial_dic 
print serial_dic.keys()

#serial_dic = {}
print_list = ['Current time','Battery Voltage','LST']

static_list = ['Sunrise','Sunset']
n = 0
while True:
	n += 1
	read_line = ser.readline()
	#print read_line.strip()
	read_line_list = read_line.replace('\r\n','').split(' : ')
	if len(read_line_list) > 1:
		#print read_line_list
		if read_line_list[0] not in serial_dic.keys():
			serial_dic[read_line_list[0]] = [read_line_list[1]]
		else:
			serial_dic[read_line_list[0]].append(read_line_list[1].replace('\n',''))
		if read_line_list[0] in print_list:
			if read_line_list[0] == print_list[0]:
				print '\n'
				print n
				#print serial_dic.keys()
				#print '\n'
			print '%s : %s' %(read_line_list[0],read_line_list[1])
	#print n
	#else:
	#	print read_line.strip()

	if n % 100 == 0:
		print '\n_____________________________'
		for key in serial_dic.keys():
			print '%15s\t:\t%18s\t:\t%s' %(key,serial_dic[key][-1],len(serial_dic[key]))
		
		#for key in static_list:
		#	print '%s : %s' %(entry,serial_dic[key][-1])
		#print len(serial_dic[key])
		print '________________________________\n'



		write_line = 'serial_dic = %s' %serial_dic
		write_file = open('starmap_arduino.py','w')
		write_file.write(write_line)
		write_file.close()





