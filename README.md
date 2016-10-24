# LED Smart Lamp

![alt text](./Photos/Off.jpg)![alt text](./Photos/Red.jpg)![alt text](./Photos/Green.jpg)![alt text](./Photos/Blue.jpg)



## Design

The lamp has a wooden base made of hardboard and pine, stained dark and finished with a matte varnish. The the rest of the lamp is made of frosted acrylic squares, glued together to make a cube, which sits on top of the base. 

The electronics sit in the base of the lamp, with the exception of the LEDs which sit on a black piece of card that lies on top of the wooden base.

## Electronics

The circuit diagram for the lamp is below. A NodeMCU is used because 
a) I had one available
b) The pins are 5V tolerant, needed for the NeoPixels

![alt text](./Diagrams/Neopixel_esp8266.jpg)

## Lamp modes
1. Rainbow mode
   This mode cycles through the available spectrum of colours continuously.
2. Static mode
   This mode sets a single static colour for the lamp.
3. Notification mode
   A single static colour is set for the lamp. If certain notifications (sms, whatspp, gmail) are recieved on my phone, then the lamp will flash the corresponding colour. At the moment, the lamp will not stop flashing when the notification is clicked. 
4. Warm mode
   Same as rainbow mode, but only yellows, red and purples.
5. Cold mode
   Same as rainbow mode, but only greens, blues and purples.
6. Off
   Sets the LED colour to rgb(0, 0, 0), which turns the LEDs off.

## Web Interface

The ESP8266 is controlled by a JSON file hosted on a webserver. In this case a Raspberry Pi 3 is used, because I have one up and running for another task. There is a web interface that can be accessed from any device on the local network and can be used to set basic modes for the lamp:
* Rainbow
* Warm
* Cold
* Off
  Note that when using the web interface, the JSON file is overwritten, rather than having key values modified. One day I'll fix this.

## Control GUI

A more advanced control GUI was written in python and Qt 5.7. This allow setting the following modes for the lamp:
* Rainbow
* Static colour
* Notification
* Warm
* Cold
* Off

## Tasker

For notification mode, the [Tasker](https://play.google.com/store/apps/details?id=net.dinglisch.android.taskerm) and [Autotools](https://play.google.com/store/apps/details?id=com.joaomgcd.autotools) (paid) apps are required.
The Tasker folder contains the profiles and tasks needed for the mode to work.