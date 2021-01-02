#Third party modules:
import serial
import time
import threading
import tkinter as tk # Python 3.x Version
import matplotlib.pyplot as plt
import numpy as np

#From .py files which are own owrk
from userInput import inputLoop
from serialComs import serialLoop, connect
from display import displayLoop

#This file uses three threads to allow user input, serial communications and a live display simultaneously
#The list 'threadShare' is used to pass information between these functions
class threadShare: 
	running = True #flag turns false when UI is quit using q+enter, promting all other threads to close
	connected = False #flag indicates serial port connection state
	send = False #flag true when message on sendBytes to be sent 
	key = "0" #String is single char holding key press obtained by getch() function 
	received = [] #List of message values read from serial port 
	sendBytes = bytearray() #bytes to write to serial port

TS = threadShare()#TS is only instance of threadshare class used

def setup(TS):
	connect(TS) #blocks until connected
	displayThread = threading.Thread(target=displayLoop, args=(TS,))#starts function displayLoop, from display.py in new thread
	displayThread.start()
	serialLoop(TS)#does not return until UI is quit

serialThread = threading.Thread(target=setup, args=(TS,))#spawn thread primarily for serialcoms.py, but runs setup function first
serialThread.start()

inputThread = threading.Thread(target=inputLoop, args=(TS,))#spawn thread for terminal I/O, target in userInput.py
inputThread.start()





