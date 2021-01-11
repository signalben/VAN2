#Third party modules:
import time
from tkinter import *
import tkinter as tk # Python 3.x Version
from tkinter.ttk import Progressbar
#From file which is own work:
from definitions import *

#Function displays received messages from a connected network in its own window
def displayLoop(TS):
        #use to tkinter to produce a window, title it, set its size 
	root = tk.Tk()  
	root.title('VAN Monitor')
	root.geometry("700x350")

	#produce first label, used to show recieved messages
	label = tk.Label(root, text="") 
	label.config(font=("Courier", 15))
	label.pack(padx=20, pady=20) 
        label.config(text = "Last recieved: ")

        #produce additional label for listing ultrasound distances in cm 
	uslabel = tk.Label(root, text="Ultrasound distance:") 
	uslabel.config(font=("Courier", 15))
	uslabel.pack(padx=20, pady=2)
	#also show this using a progressbar
	bar = Progressbar(root, length = 650)
	bar['value'] = 0
	bar.pack(padx=20, pady=2)

        #labelled, progressbar showing IMU acceleration of buggy, stringly affected by buggy angle due to gravity 
	acclabel = tk.Label(root, text="Inclination: ") 
	acclabel.config(font=("Courier", 15))
	acclabel.pack(padx=20, pady=2) 
	accbar = Progressbar(root, length = 650)
	accbar['value'] = 0
	accbar.pack(padx=20, pady=2)

        #labelled progressbar showing velocity of buggy rotation from above (IMU Z axis)
	turnlabel = tk.Label(root, text="Turnspeed: ") 
	turnlabel.config(font=("Courier", 15))
	turnlabel.pack(padx=20, pady=2) 
	turnbar = Progressbar(root, length = 650)
	turnbar['value'] = 0
	turnbar.pack(padx=20, pady=2)

        #text changed depending on whether buggy is blocked or not
	obsticleLabel = tk.Label(root, text=" ") 
	obsticleLabel.config(font=("Courier", 15))
	obsticleLabel.pack(padx=20, pady=0) 

        #text changed depending on whether buggy is rolled over or not
	rolloverLabel = tk.Label(root, text=" ") 
	rolloverLabel.config(font=("Courier", 15))
	rolloverLabel.pack(padx=0, pady=0) 

        #start these labels without assuming the state of the buggy, messages updating these only sent when states change
	obsticleLabel.config(text="No report")  
	rolloverLabel.config(text="No report") 

	while(TS.running):
		time.sleep(0.001)

		if TS.received:	#TS.received set by serial thread, list of received message bytes
			try:    #corrupt messages should not have been added, but use try as an additional precaution
				message = TS.received   #copy message content to integer variables, destination not used
				src = int(message[1])
				cmd = int(message[3])
				dat0 = int(message[4])
				dat1 = int(message[5])
				TS.received.clear()     #clear to prevent acting on same message twice

			except:	
				TS.received.clear()     #clear corrupt message

			else:		
				TS.received.clear()     #clear anyway, in case of possibility of some value being evaulated as false

                                #attempt to get source and command names, if not in definitions.py, proceed using integer value
				try:
					srcName = address2Name[src]
				except:
					srcName = src	

				try:
					cmdName = command2Name[cmd]
				except:
					cmdName = cmd	

                                #messages from ultrasound are a 'special case'- Ans use a unique method of display
				if srcName == "ULTRASOUND":
					distance = int((256*dat0) + dat1) #convert both databytes to single integer (sign not required)

					bar['value'] = distance/100     #scale for progressbar
					bar.update()
					uslabel.config(text="Ultrasound distance: " + str(int(distance/60)) +"cm")  #   /60 scale factor calulated from reading at 50cm

                        
                                #another special case- reports from motors use dat0 as boolean clearahead(ultrasound), dat1 as boolean upright(IMU)  
				elif ((srcName == "MOTORS") and (cmdName == "REPORT")):
					if (dat0 == 1):
						obsticleLabel.config(text="Clear Ahead")  
					else:
						obsticleLabel.config(text="Obstructed") 	
					if (dat1 == 1):
						rolloverLabel.config(text="Upright")  
					else:
						rolloverLabel.config(text="Rolled Over") 		

                                #special cases where IMU sends X axis acceleration with cmd=P0, or Z axis rotation velocity with cmd=P1 
				elif(srcName == "IMU"):

					if (cmdName == "P0"):
						data = bytearray()
						data.extend(dat0.to_bytes(1, 'big', signed=False))      #requires conversion from 2 uint8's to one signed int16
						data.extend(dat1.to_bytes(1, 'big', signed=False))                                                     
						acceleration = int.from_bytes(data, 'big', signed=True)
						data.clear()
						accbar['value'] = 50 - acceleration/20 #scale to give +/-90deg incline uses entire progressbar 
						accbar.update()

					elif (cmdName == "P1"):
						data = bytearray()
						data.extend(dat0.to_bytes(1, 'big', signed=False))
						data.extend(dat1.to_bytes(1, 'big', signed=False))                                                     
						turnspeed = int.from_bytes(data, 'big', signed=True)
						data.clear()
						turnbar['value'] = 50 - turnspeed/8 #not calibrated, scaled for maximum turning speed
						turnbar.update()
						
                                #default for any other messages, where its components are simply listed
				else:
					lastMessage = str(srcName) + " " + str(cmdName) + " " + str(dat0) + " " + str(dat1)
					label.config(text = "Last recieved: "+lastMessage)
					label.update()



		

