To compile Arduino sketches espBuggy.ino, espStatic.ino, mega.ino, uno.ino, uno_enc.ino (located within vanNodes folder)
Add folder libaries to C:\Users\"username"\Documents\Arduino
Add folder vanNodes to C:\Users\"username"\Documents\Arduino
Do not remove netDef.cpp, netDef.h from sketch folders (they are unique for each node) 
Open .ino's with Arduino IDE. 

espBuggy.ino, espStatic.ino are designed for ESP32, with board set to ESP32 dev module.
This requires "ESP32 by Espressif Systems" to be installed via the Arduino boards manager, link to guide below:   
https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/

Mega.ino requires board to be set to mega. 
Uno.ino, uno_enc.ino have been tested using generic nanos, should work with any 328p board

The python user interface resides within van_dev_pc.
Requires Python 3, Ubuntu 18.04 
Modules must be installed:
serial, time, threading, tkinter, numpy
Leave all .py files in the same folder, start by running run.py from the terminal