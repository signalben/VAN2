#Third party modules:
import serial, time, threading, sys, termios, tty, os
import tkinter as tk # Python 3.x Version
import matplotlib.pyplot as plt
import numpy as np
#From file which is own work:
from definitions import *

def parseUserMessage(TS, userInput):

	userWordList = userInput.upper().split()

	try:
		src = name2Address["PC"]
	except:
		print('Error: "PC" address byte must be defined')
		return	
	try:
		dest = name2Address[userWordList[0]]
	except:
		print("Error: Destination word is not a device name")
		return		
	try:	
		cmd = name2Command[userWordList[1]]
	except:
		print("Error: Command word not recognized ")
		return	

	if cmd == name2Command["SETDEST"]:
		try:
			value = name2Address[userWordList[2]]
		except:
			print("Error: Destination specified is not a device name")
			return

		data = value.to_bytes(2, 'big', signed=True)
		TS.sendBytes = bytearray([STD,src,dest,cmd])
		TS.sendBytes.extend(data)
		TS.send = True
		return;
			
	else:

		if (len(userWordList)==3):
			
			try:	
				value = int(userWordList[2])
			except:
				print("Error: Data word is not an integer ")
				return	

			if value < -32768:
				print("Error: Minimum possible value is -32,768")
				return

			if value > 32767:
				print("Error: Maximum possible value is 32,767")
				return	
	
			data = value.to_bytes(2, 'big', signed=True)
			TS.sendBytes = bytearray([STD,src,dest,cmd])
			TS.sendBytes.extend(data)
			#print(TS.sendBytes)
			TS.send = True
			return;

		if (len(userWordList)==4):
		
			try:	
				value = int(userWordList[2])
			except:
				print("Error: DAT0 is not an integer ")
				return	

			if value < 0:
				print("Error: Minimum possible DAT0 value is 0")
				return

			if value > 255:
				print("Error: Maximum possible DAT0 value is 255")
				return	

			dat0 = value.to_bytes(1, 'big', signed=False)

			try:	
				value = int(userWordList[3])
			except:
				print("Error: DAT1 is not an integer ")
				return	

			if value < 0:
				print("Error: Minimum possible DAT1 value is 0")
				return

			if value > 255:
				print("Error: Maximum possible DAT1 value is 255")
				return	

			dat1 = value.to_bytes(1, 'big', signed=False)
			TS.sendBytes = bytearray([STD,src,dest,cmd])
			TS.sendBytes.extend(dat0)
			TS.sendBytes.extend(dat1)
			TS.send = True
			return;

def getch(TS):
	while (TS.key != "c"):
		fd = sys.stdin.fileno()
		old_settings = termios.tcgetattr(fd)
		try:
			tty.setraw(sys.stdin.fileno())
			TS.key = sys.stdin.read(1)

		finally:
			termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)	
		time.sleep(0.05)				

def keyControl(TS):

	TS.key = "0"
	keyThread = threading.Thread(target=getch, args=(TS,))
	keyThread.start()
	delay = 0.3
	userInput = "PID PERIOD 0"
	parseUserMessage(TS, userInput)
	time.sleep(0.1)

	while(TS.running):
		#print(userInput)

		if (TS.key == "c"):
			userInput = "PID PERIOD 20"
			parseUserMessage(TS, userInput)
			break

		elif (TS.key == "w"):
			userInput = "MOTORS SET 220 220"
			delay = 0.3

		elif (TS.key == "s"):
			userInput = "MOTORS SET 30 30"
			delay = 0.3

		elif (TS.key == "a"):
			userInput = "MOTORS SET 0 255"
			delay = 0.3

		elif (TS.key == "d"):
			userInput = "MOTORS SET 255 0"
			delay = 0.3

		else:
			userInput = "MOTORS SET 128 128"
			delay = 0.05

			
		TS.key = "0"
		parseUserMessage(TS, userInput)
		time.sleep(delay)	



def inputLoop(TS):		
	while(TS.running):

		if TS.connected:
			print(" ")
			print("Press 'q' + enter to quit")
			print("Press 'c' + enter for PC control")
			print("Addressable devices:" )
			deviceList = str(name2Address.keys())
			print(deviceList[11:-2]) 
			print("Commands:" )
			commandsList = str(name2Command.keys())
			print(commandsList[11:-2]) 
			print("Send message in formats: DEST CMD DATA, DEST CMD DAT0 DAT1, or DEST SETDEST ADDRESS")

		userInput = input();	

		if userInput == 'q':
			TS.running = False;
			break

		elif userInput == 'c':
			print("Key control mode- use W,S,D,A keys")
			print("Press C again to return")
			keyControl(TS)	

		elif TS.connected:
			#===========threadShare[2] = userInput
			parseUserMessage(TS, userInput)

		else:
			print("Serial device not found")




