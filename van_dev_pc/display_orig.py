#https://www.reddit.com/r/learnpython/comments/8kfys3/running_code_while_waiting_for_user_input/ ##corey shraffer
#https://pythonspot.com/matplotlib-update-plot/
#https://gist.github.com/athiyadeviyani/b18afdc8136f003956b1a71d94a6c696
#https://stackoverflow.com/questions/13510882/how-to-change-ttk-progressbar-color-in-python

import time
from tkinter import *
import tkinter as tk # Python 3.x Version
from tkinter.ttk import Progressbar
from definitions import *


def displayLoop(TS):
	root = tk.Tk()
	root.title('VAN Monitor')
	root.geometry("700x350")
	label = tk.Label(root, text="") 
	label.config(font=("Courier", 15))
	label.pack(padx=20, pady=20) 

	label.config(text = "Last recieved: ")

	uslabel = tk.Label(root, text="Ultrasound distance:") 
	uslabel.config(font=("Courier", 15))
	uslabel.pack(padx=20, pady=2) 
	bar = Progressbar(root, length = 650)
	bar['value'] = 0
	bar.pack(padx=20, pady=2)

	label2 = tk.Label(root, text="") 
	label2.config(font=("Courier", 15))
	label2.pack(padx=20, pady=20) 

	acclabel = tk.Label(root, text="Inclination: ") 
	acclabel.config(font=("Courier", 15))
	acclabel.pack(padx=20, pady=2) 
	accbar = Progressbar(root, length = 650)
	accbar['value'] = 0
	accbar.pack(padx=20, pady=2)

	turnlabel = tk.Label(root, text="Turnspeed: ") 
	turnlabel.config(font=("Courier", 15))
	turnlabel.pack(padx=20, pady=2) 
	turnbar = Progressbar(root, length = 650)
	turnbar['value'] = 0
	turnbar.pack(padx=20, pady=2)

	while(TS.running):
		time.sleep(0.001)

		if TS.received:	
			try:
				message = TS.received
				src = int(message[1])
				cmd = int(message[3])
				dat0 = int(message[4])
				dat1 = int(message[5])
				TS.received.clear()

			except:	
				TS.received.clear()

			else:		
				TS.received.clear()
			
				try:
					srcName = address2Name[src]
				except:
					srcName = src	

				try:
					cmdName = command2Name[cmd]
				except:
					cmdName = cmd	

				if srcName == "ULTRASOUND":
					distance = int((256*dat0) + dat1)
					if distance <= 1500:
						label2.config(text = "Obsticle detected")

					else:
						label2.config(text = "")

					bar['value'] = distance/100
					bar.update()
					uslabel.config(text="Ultrasound distance: " + str(int(distance/60)) +"cm")  

					"""elif srcName == "IMU":
						if cmdName == "P1":


					"""
				elif(srcName == "IMU"):

					if (cmdName == "P0"):
						data = bytearray()
						data.extend(dat0.to_bytes(1, 'big', signed=False))
						data.extend(dat1.to_bytes(1, 'big', signed=False))                                                     
						acceleration = int.from_bytes(data, 'big', signed=True)
						data.clear()
						accbar['value'] = 50 - acceleration/20
						accbar.update()

					elif (cmdName == "P1"):
						data = bytearray()
						data.extend(dat0.to_bytes(1, 'big', signed=False))
						data.extend(dat1.to_bytes(1, 'big', signed=False))                                                     
						turnspeed = int.from_bytes(data, 'big', signed=True)
						data.clear()
						turnbar['value'] = 50 - turnspeed/8
						turnbar.update()

				else:
					lastMessage = str(srcName) + " " + str(cmdName) + " " + str(dat0) + " " + str(dat1)
					label.config(text = "Last recieved: "+lastMessage)
					label.update()



		

