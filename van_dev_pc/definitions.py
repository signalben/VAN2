#Third party modules:
import serial
import time
import threading
import tkinter as tk # Python 3.x Version
import matplotlib.pyplot as plt
import numpy as np

#Definitions for start codes of messages
STD = 7
ACK = 90
RESP = 110

#disctionary allows references to devices in plaintext to be converted to their uint8 addresses
name2Address = {
"MEGA" : 1,
"ESP32BUGGY" : 2,
"PC" : 3,
"NANO" : 4,
"ESP32STATIC" : 5,
"ULTRASOUND" : 16,
"JOYSTICK" : 17,
"MOTORS" : 20,
"SKIDSTEER" : 30,
"PID" : 31,
"IMU" : 18,
"REMOTE" : 19,
"NANO_ENC" : 6
}

#create another dictionary with keys and values swapped, for displaying received device addresses in plaintext 
address2Name = {v:k for k, v in name2Address.items()}

#disctionary allows commands in plaintext to be converted to their uint8 codes
name2Command = {
"REPORT" : 1,
"PERIOD" : 2, 
"ECHO"   : 3,
"RESPONSE" : 4,
"SETDEST" : 5, 
"REPORTNOW" : 6,
"SET": 7,
"P0" : 8,
"P1" : 9,
"P2" : 10,
"P3" : 11
}

#create another dictionary with keys and values swapped, for displaying received device commands in plaintext 
command2Name = {v:k for k, v in name2Command.items()}



