#https://www.reddit.com/r/learnpython/comments/8kfys3/running_code_while_waiting_for_user_input/ ##corey shraffer
#https://pythonspot.com/matplotlib-update-plot/
import serial
import time
import threading

from definitions import *
		
def connect(TS):
	global ser
	alreadyExcepted = False
	serialPort = '/dev/ttyUSB0'
	#serialPort = '/dev/ttyACM0'
	while(TS.running):
		try:
			ser = serial.Serial(serialPort, 115200, timeout = 0)
			TS.connected = True
			print("Connected to device at " + ser.name)
			print("Press 'q' + enter to quit")
			print("Press 'c' + enter for PC control")
			print("Addressable devices:" )
			deviceList = str(name2Address.keys())
			print(deviceList[11:-2]) 
			print("Commands:" )
			commandsList = str(name2Command.keys())
			print(commandsList[11:-2]) 
			print("Send message in formats: DEST CMD DATA, DEST CMD DAT0 DAT1, or DEST SETDEST ADDRESS")
			break
		except:
			if alreadyExcepted == False:
				TS.connected = False
				print("Error: No serial device found at "+serialPort)
				print("Connect it now, or press 'q' + return to quit")
				alreadyExcepted = True
			time.sleep(0.05)

def calcChecksum(inData):
	checksum = 0
	for b in inData:
		checksum = checksum ^ b
	return checksum      
		
def serialLoop(TS):
	global ser
	buff = []
	while(TS.running):
		#print(ser.in_waiting);
		try:	
			if ser.in_waiting >= 7:
				start = False

				while((start == False) and (ser.in_waiting >= 7)):
					b = ser.read()
					i = int.from_bytes( b, byteorder='big', signed=False)

					if (i == STD) or (i == ACK) or (i == RESP):
						buff.append(i)
						start = True

				for x in range(0, 6):
					b = ser.read()
					i = int.from_bytes( b, byteorder='big', signed=False)
					buff.append(i)

				checksum = calcChecksum(buff[:-1])

				if checksum == buff[-1]:
					TS.received = buff[:-1]
					
				buff.clear()

		except:
			ser.close()
			print("Error: Connection lost")
			connect(TS)

		if(TS.send):
			TS.send = False

			checksum = calcChecksum(TS.sendBytes)
			TS.sendBytes.extend(checksum.to_bytes(1, 'big'))
			#print(TS.sendBytes)
			bytesSentNum = ser.write(TS.sendBytes)
			#print("bytes sent: " + str(bytesSentNum))

	time.sleep(0.001)				





