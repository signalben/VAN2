#https://www.reddit.com/r/learnpython/comments/8kfys3/running_code_while_waiting_for_user_input/ ##corey shraffer
#https://pythonspot.com/matplotlib-update-plot/
import serial
import time
import threading
import tkinter as tk # Python 3.x Version
import matplotlib.pyplot as plt
import numpy as np

from userInput import inputLoop
from serialComs import serialLoop, connect
from display import displayLoop

#This file uses three threads to allow user input, serial communications and a live display simultaneously
#The list 'threadShare' is used to pass information between these functions
class threadShare: 
	running = True
	connected = False
	send = False
	keyCapture = False
	key = "0"
	sent = "None"
	received = []
	error = "None"
	sendBytes = bytearray()

TS = threadShare()

def setup(TS):
	connect(TS) #blocks until connected
	displayThread = threading.Thread(target=displayLoop, args=(TS,))
	displayThread.start()
	serialLoop(TS)

serialThread = threading.Thread(target=setup, args=(TS,))
serialThread.start()

inputThread = threading.Thread(target=inputLoop, args=(TS,))
inputThread.start()





