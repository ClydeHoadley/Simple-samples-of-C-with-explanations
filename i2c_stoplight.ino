/***************************************************************************************
i2c_stoplight.ino                                                         21 August 2021

This is the I2C Slave program that recieves commands from the Master I2C program
(i2c_sysfs_stoplight.c,) that is running on a Raspberry Pi 3B+   This program
switches LED's on and off as instructed by the master control program.

Wire as follows: Connect the Pi GPIO ground to the Uno ground.  Connect the Pi SDA to the
Uno's SDA (A4), connect the Pi's SCL to the Uno's SCL (A5).  Connect Uno's digital pin 13
to the anode (+ (short)) of the red LED,  connect the Uno's digital pin 12 to the anode of
the yellow LED, connect pin 11 to the anode of the green LED.  There should be 330 ohm
resisters between the ground rail and each of the LED's cathodes (long).  Connect the
Uno's ground to the breadboard ground.  Upload i2c_stoplight.ino to the Arduino then
run a.out from the command line of the Raspberry pi.

by Clyde Hoadley with help from Adafruit.com
****************************************************************************************************
                                GNU General Public License v3.0
    This program is free software: you can redistribute it and/or modify it under the terms of the
    GNU General Public License as published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
    the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program.
    If not, see <https://www.gnu.org/licenses/>.
****************************************************************************************/

#include <Wire.h>

#define SLAVE_ADDR 9
#define RED 13
#define YELLOW 12
#define GREEN 11

void setup() {
  // put your setup code here, to run once:
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  Wire.begin(SLAVE_ADDR);
  Serial.begin(9600);
  Wire.onReceive(receiveEvent);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
}

void receiveEvent() {
 byte x;
  // Read while data received
  
  while (0 < Wire.available()) {
    x = Wire.read();
  }
  
  if (x==1) {
   digitalWrite(RED, HIGH);
   digitalWrite(YELLOW, LOW);
   digitalWrite(GREEN, LOW);
  }
  if (x==2) {
   digitalWrite(RED, LOW);
   digitalWrite(YELLOW, HIGH);
   digitalWrite(GREEN, LOW);
  }
  if (x==3) {
   digitalWrite(RED, LOW);
   digitalWrite(YELLOW, LOW);
   digitalWrite(GREEN, HIGH);
  }
  // Print to Serial Monitor
  Serial.println("Receive event");
}
