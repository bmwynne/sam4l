Sam4l Cellular Printer Project
======================

A cellular printer project utliziing the Atmel Sam4l and the Zebra ZT230
-------------

Dependencies
-------------
1. gcc-arm-none-eabi to cross compile x86 architechture to the arm cortex-m4 used by the Sam4l.
..* I reccomend using Homebrew on OS X. 
..* $> brew cask install gcc-arm-embedded

2. To flash the target built binary onto the device, in this case I will list a program called OpenOCD which uses the debug port of the Sam4l.
..* Like arm-none-eabi, I reccomend using brew...
..* $>brew install openocd


Programming
-------------
1. Download / Clone this project into your desired workspace.
2. $> cd sam4l
3. $> make
4. $> openocd -f atmel_sam4l8_xplained_pro.cfg -c "program sam4l-printer-flash.elf verify reset exit"

Visualizing
-------------
1. Open your favorite serial port to see text information
..* Baud Rate: 115200 (On OS X)Device: tty.usbmodem141432

Documentation
-------------
More Documentation to come...

