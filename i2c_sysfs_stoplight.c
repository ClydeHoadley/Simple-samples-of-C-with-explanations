#define Version "{i2c_sysfs_stoplight (v1.0a)}"
/**************************************************************************************************
i2c_sysfs_stoplight.c                                                                21 August 2021

    This program runs on the Raspberry Pi 3B+.  It uses i2c to control an Arduino Uno.
    This program uses the linux file system to access i2c.  The Uno is switching 3 LEDs on and
    off as commanded by the Pi.  The companion program running on the Uno is i2c_stoplight.ino

    gcc i2c_sysfs_stoplight.c
    ./a.out


    Wire as follows: Connect the Pi GPIO ground to the Uno ground.  Connect the Pi SDA to the
    Uno's SDA (A4), connect the Pi's SCL to the Uno's SCL (A5).  Connect Uno's digital pin 13
    to the anode (+ (short)) of the red LED,  connect the Uno's digital pin 12 to the anode of
    the yellow LED, connect pin 11 to the anode of the green LED.  There should be 330 ohm
    resisters between the ground rail and each of the LED's cathodes (long).  Connect the
    Uno's ground to the breadboard ground.  Upload i2c_stoplight.ino to the Arduino then
    run a.out from the command line of the Raspberry pi.

    Notes:
    To find the address of the 12c device you can run this command.
    pi@breakout:~ $ i2cdetect -y 1
         0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
    00:          -- -- -- -- -- -- 09 -- -- -- -- -- --
    10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    70: -- -- -- -- -- -- -- --
    The output shows there is an I2C device at address 0x09.  That is the Arduino.

    You can control the device from the command line with this command.
    i2cset 0x01 0x09 0x00 3   //tells the Uno to switch on green and turn the other 2 off
    i2cset 0x01 0x09 0x00 2   //switch to yellow and turn the other off
    i2cset 0x01 0x09 0x00 1   //switch to red and turn the others off
    i2cset 0x01 0x09 0x00 0   //no affect the Uno program doesn't act on a zero
                              //The 0x00 is a memory address that I just guessed at.

    by Clyde Hoadely with help from https://www.waveshare.com/wiki/Raspberry_Pi_Tutorial_Series:_I2C
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
**************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <errno.h>

#define I2C_ADDR 0x09    // shell command   i2cdetect -y -1   revials an i2c device at address 0x09
#define LED_RED  0x01
#define LED_YELLOW 0x02
#define LED_GREEN 0x03

int main (void) {
    int value=0;
    int fd;

    fd = open("/dev/i2c-1", O_RDWR);     // i2c bus 1 is the only one we have.  Bus 0 is used
    if (fd < 0) {                        // internally by raspbian.
        perror("Error opening file:\n");
        exit(EXIT_FAILURE);
    }
    if (ioctl(fd, I2C_SLAVE, I2C_ADDR) < 0) {   // I2C_SLAVE must be predefined, but I2C_ADDR is
        perror("ioctl error:\n");               // defined above
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        if(value == LED_RED)
        value = LED_YELLOW;
        else
        if(value == LED_YELLOW)
            value=LED_GREEN;
        else
            value=LED_RED;

        if( write( fd , &value, 1 ) != 1) {
            perror("Error writing file: \n");
        }
        usleep(1000000);
    }
    exit(EXIT_SUCCESS);  //Can't get here from there.
}
