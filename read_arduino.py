#!/usr/bin/env python2.7

#import Gnuplot
import serial
import sys

ser = serial.Serial('/dev/tty.usbmodem1d11',57600)
while True:
	print ser.readline()

readings = []
#g = Gnuplot.Gnuplot()
#g.title("Thermistor readings")
#g('set data style lines')
#g('set yrange [-5:105]')

while 1:
   reading = ser.readline().split()
   print reading

   readings.append(reading)

   if len(readings) > 100:
       readings = readings[-10:]
   print readings
   #g.plot(readings)

