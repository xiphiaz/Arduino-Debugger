#README


##Intro

This hardware/software grapher will assist you in developing your Arduino projects by giving you easy to use and measure readouts of the pins on your Arduino while still allowing the Arduino to be running the project code without interference.

##Hardware setup

You will need:

+	An Arduino project that has a sketch running on it with some kind of input/output on the pins (this should be the easy step!)
+	A spare Arduino for measuring the other Arduino's pins and relaying the info to the graphing software
+	Jumper cables (simple method) _or_ shield header pins (the ones with double length legs)

The easiest method to get you up and running is to simply put jumper cables between the two Arduino's on the pins you want to be checking

For a more permanent and easily re-usable solution, de-solder all the pins on the monitoring Arduino and replace them with shield header pins. The monitoring Arduino can now be stacked on top of the project Arduino like a shield, and all other wires can still be put through the monitoring Arduino's pins. **Note this has not been tested yet, try this at your own risk**

##Software Installation

Next up, we will get the arduino flashed with the software.

1.	Open up the _arduino\_debugger.pde_ sketch in the Arduino IDE
2. Connect the monitoring arduino board (the arduino that is not running your project software)
3. Upload the sketch to the monitoring Arduino

Next you can pop the main application wherever you want it (/Applications/ or otherwise)

##Usage

1. Always have the Arduino plugged in before launching the application
2.	When opened, hit the start button [or space] and click on the pins that you wish to be checking
3.	If you want to measure a value click on the graph and a measurement will show up.
4.	Clicking in two parts of the graph will show both measurement of the active pins, and the time interval between the two measurements
5. If you want to compare the individual values of a pin between two measurements, hover the cursor over that pin's button and a measurement will show up to the right of the graph
6. If there is lots of data coming in and the graph starts to appear jumpy, you can bump up the baud rate by clicking the _Change Baud_ button. Note that increasing the baud rate will increase the number of messages in the buffer, and depending on your computer, the ram may struggle to keep up
7. Sometimes changing the baud rate may reset the timestamp, causing the graph to overflow. This is easily fixed by pressing the reset button on the app (not the Arduino reset)

###License

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.
See file LICENSE for further informations on licensing terms. 
                                                              
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.                   

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA