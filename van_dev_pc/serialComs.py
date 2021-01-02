#Third party modules:
import serial
import time
import threading
#From file which is own work:
from definitions import *

#function blocks until serial connection detection or user quits		
def connect(TS):
	global ser #pyserial serial port object
	alreadyExcepted = False #when flag is true prevents repeating of "No connection" message
	serialPort = '/dev/ttyUSB0'
	while(TS.running): #failure to connect prevents spawning of other threads
		try:
			ser = serial.Serial(serialPort, 115200, timeout = 0) #will fail if no serial device plugged in
			TS.connected = True
			print("Connected to device at " + ser.name)
			print("Press 'q' + enter to quit")
			print("Press 'c' + enter for PC control")
			print("Addressable devices:" )
			deviceList = str(name2Address.keys())   #valid addresses and commands printed for user convenience
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
				alreadyExcepted = True  #Print "no connection" message only once
			time.sleep(0.05)  #but continue trying to establish a connection

#calculates XOR checksum of all other message bytes (same process as VAN nodes)
def calcChecksum(inData):
	checksum = 0
	for b in inData:
		checksum = checksum ^ b
	return checksum      

#runs indefinetly unitl UI quit		
def serialLoop(TS):
	global ser
	buff = [] #list used to construct message until complete
	while(TS.running):
		try:	
			if ser.in_waiting >= 7: #fails if serial device unplugged, allowing reconnection        
				start = False #message start byte not yet detected

				while((start == False) and (ser.in_waiting >= 7)): #enough bytes for a complete message, but start not yet detected
					b = ser.read() #read single byte from serial port
					i = int.from_bytes( b, byteorder='big', signed=False) #convert that byte to integer

					if (i == STD) or (i == ACK) or (i == RESP): #any of these are valid start bytes, although this UI does not use ACK/RESP protocols
						buff.append(i)
						start = True  #escape while loop

				for x in range(0, 6): #now read src, cmd, dat0, dat1, checksum values into buff
					b = ser.read()
					i = int.from_bytes( b, byteorder='big', signed=False)
					buff.append(i)

				checksum = calcChecksum(buff[:-1]) #calculate XOR of all bytes in buff other than last (which is the received checksum)

				if checksum == buff[-1]: #if calculated and recieved checksums equal
					TS.received = buff[:-1]#valid message recieved, add to threadshare to allow use by display thread
					
				buff.clear() #reset buffer for next message

		except: #caused by serial port failure, attempt reconnection
			ser.close()
			print("Error: Connection lost")
			connect(TS)

		if(TS.send):#indicates userInput thread has produces message to send
			TS.send = False #only send it once

			checksum = calcChecksum(TS.sendBytes)                   
			TS.sendBytes.extend(checksum.to_bytes(1, 'big')) #add a calculated checksum to bytes to send
			bytesSentNum = ser.write(TS.sendBytes) #send them, returns number bytes sent, not displayed

	time.sleep(0.001) #limit frequency of checking for received bytes				





