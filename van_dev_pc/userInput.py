#Third party modules:
import serial, time, threading, sys, termios, tty, os
import tkinter as tk # Python 3.x Version
import matplotlib.pyplot as plt
import numpy as np
#From file which is own work:
from definitions import *

#function takes a string as input by the using, producing either a valid message or returns an error detailing why a valid message could not be formed
#valid messages are put in TS.sendBytes to be sent by the serial thread, function returns if an error is encountered
def parseUserMessage(TS, userInput): 

	userWordList = userInput.upper().split() #convert to all caps, split by whitespace, stored as a list of strings
        
	try:    #first, verify that an address has been defined for this PC in definitions.py
		src = name2Address["PC"]
	except:
		print('Error: "PC" address byte must be defined')
		return	
	try:    
		dest = name2Address[userWordList[0]]    #find destination device address for first user word
	except:
		print("Error: Destination word is not a device name")
		return		
	try:	
		cmd = name2Command[userWordList[1]]     #find command code for second user word
	except:
		print("Error: Command word not recognized ")
		return	

	if cmd == name2Command["SETDEST"]:      #for setting the destinations a different format is used: [receiving device address][command setdest][setdest to address]
		try:
			value = name2Address[userWordList[2]] #third user word should be a device address
		except:
			print("Error: Destination specified is not a device name")
			return

		data = value.to_bytes(2, 'big', signed=True) #use this address as data byte
		TS.sendBytes = bytearray([STD,src,dest,cmd]) #add all other message components first
		TS.sendBytes.extend(data)                    #add the data byte
		TS.send = True                               #set flag to notify serial thread that the message is complete
		return;
			
	else:

		if (len(userWordList)==3):      #format entered is DEST CMD (signed int16)DATA
			
			try:	
				value = int(userWordList[2])    #veryify that DATA word is indeed an integer
			except:
				print("Error: Data word is not an integer ")
				return	

			if value < -32768:      #also verify that this integer is within int16 range
				print("Error: Minimum possible value is -32,768")
				return

			if value > 32767:
				print("Error: Maximum possible value is 32,767")
				return	
	
			data = value.to_bytes(2, 'big', signed=True) #same process as before
			TS.sendBytes = bytearray([STD,src,dest,cmd])
			TS.sendBytes.extend(data)
			TS.send = True
			return;

		if (len(userWordList)==4):      #format entered is DEST CMD (uint8)DAT0 (uint8)DAT1
		
			try:	
				value = int(userWordList[2])    #veryify that DAT0 word is indeed an integer
			except:
				print("Error: DAT0 is not an integer ")
				return	

			if value < 0:   #also verify that this integer is within uint8 range
				print("Error: Minimum possible DAT0 value is 0")
				return

			if value > 255:
				print("Error: Maximum possible DAT0 value is 255")
				return	

			dat0 = value.to_bytes(1, 'big', signed=False)   #convert dat0 to single byte 

			try:	
				value = int(userWordList[3])    #veryify that DAT1 word is indeed an integer
			except:
				print("Error: DAT1 is not an integer ")
				return	

			if value < 0:   #also verify that this integer is within uint8 range
				print("Error: Minimum possible DAT1 value is 0")
				return

			if value > 255:
				print("Error: Maximum possible DAT1 value is 255")
				return	

			dat1 = value.to_bytes(1, 'big', signed=False)  #convert dat1 to single byte 
			TS.sendBytes = bytearray([STD,src,dest,cmd])   #add all other message components first
			TS.sendBytes.extend(dat0)                      #add data bytes 
			TS.sendBytes.extend(dat1)
			TS.send = True                                 #set flag to notify serial thread that the message is complete
			return;
		
#function captures keypresses continously
#is provided with its own thread to allow sending of messages based on keypresses simultaneously
def getch(TS):
	while (TS.key != "c"):  #provide method of escaping function
		fd = sys.stdin.fileno()
		old_settings = termios.tcgetattr(fd)
		try:
			tty.setraw(sys.stdin.fileno())
			TS.key = sys.stdin.read(1)

		finally:
			termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)	
		time.sleep(0.05)  #limit update cycle to 20hz				

#function impliments user control of buggy via keycapture
def keyControl(TS):

	TS.key = "0"    #start with no actionable kepyress
	keyThread = threading.Thread(target=getch, args=(TS,)) #start thread for getch(), providing the keycapture
	keyThread.start()
	delay = 0.3             #large delay required to compensate for intitial 'pause' given by stdin upon start of keypress
	userInput = "PID PERIOD 0" #key control directly targets the motors, this message switches the PID device off to prevent it interfering
	parseUserMessage(TS, userInput) #send this message
	time.sleep(0.1) #pause to ensure it is sent before being overwritten

	while(TS.running):

		if (TS.key == "c"): #user has ending keycapture mode 
			userInput = "PID PERIOD 20" #switch PID device back on
			parseUserMessage(TS, userInput) 
			break #exit keycapture mode

		elif (TS.key == "w"): #forward motion, medium speed
			userInput = "MOTORS SET 220 220"
			delay = 0.3

		elif (TS.key == "s"): #backward motion, medium speed
			userInput = "MOTORS SET 30 30"
			delay = 0.3

		elif (TS.key == "a"): #left, fullspeed
			userInput = "MOTORS SET 0 255"
			delay = 0.3

		elif (TS.key == "d"): #right, fullspeed
			userInput = "MOTORS SET 255 0"
			delay = 0.3

		else:   #any other key, no key: stop
			userInput = "MOTORS SET 128 128"
			delay = 0.05  #shorter delay improves response time

			
		TS.key = "0" #set back to non-actionable keypress, preventing keypresses from 'locking'
		parseUserMessage(TS, userInput) #send message to motors
		time.sleep(delay) #limits message frequency, must not be faster than getch() delay or its keypresses will be used quicker than they are provided


#takes input for control of UI
def inputLoop(TS):		
	while(TS.running):

		if TS.connected:        #only if a serial device is connected, give the user a list of options, then references to device addresses & commands
			print(" ")
			print("Press 'q' + enter to quit")
			print("Press 'c' + enter for PC control")
			print("Addressable devices:" )
			deviceList = str(name2Address.keys()) #create string of device address dictionary (from definitions.py)
			print(deviceList[11:-2]) #print without some superflous packing characters 
			print("Commands:" )
			commandsList = str(name2Command.keys()) #same process for command dictionary
			print(commandsList[11:-2]) 
			print("Send message in formats: DEST CMD DATA, DEST CMD DAT0 DAT1, or DEST SETDEST ADDRESS")

		userInput = input();	#get string from stdin, is blocking

		if userInput == 'q': #if string is single 'q' char, notify all threads to close, then exit
			TS.running = False;
			break

		elif userInput == 'c': #enter keyControl function, which returns when keycontrol is exited
			print("Key control mode- use W,S,D,A keys")
			print("Press C again to return")
			keyControl(TS)	

		elif TS.connected:  #user has entered a message to be sent, pass for interpretation
			parseUserMessage(TS, userInput)

		else:               #user has entered message despite no device being connected
			print("Serial device not found")




