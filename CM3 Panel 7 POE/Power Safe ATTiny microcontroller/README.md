# Industrial computer module display

https://www.acmesystems.it/CM3-PANEL-7-POE

In this area are published the sources of the ATTiny microcontroller which manage, through a supercapacitor, the correct shutdown of the ACME Systems CM3 Panel POE industrial card in case of external power failure. 
Compared to the previous version of CM3 Panel, the microcontroller also allows the board to operate even without the touch display, for example to be installed on DIN rail supports inside electrical panels.

All information on the operating status of the microprocessor board is sent through the microcontroller on the serial port on the EXP2 connector.

An RGB LED on the board, connected to the pins of the microcontroller, indicates the state of charge of the supecapacitor.

## Examples programming ATTiny microcontroller from command line ( Raspbian OS )

Required install avrdude

Ex: sudo apt-get install avrdude

# Flash Optiboot bootloader width Atmel ICE
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -catmelice_isp -Pusb -e -Uefuse:w:0xfe:m -Uhfuse:w:0b11010111:m -Ulfuse:w:0xE2:m -Uflash:w:optiboot_attiny88_8000000L.hex:i 

# Programming Bootloader with USBTiny
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -cusbtiny -e -Uefuse:w:0xfe:m -Uhfuse:w:0b11010111:m -Ulfuse:w:0xE2:m -Uflash:w:optiboot_attiny88_8000000L.hex:i 

# Programming PowerSafe with USBTiny
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -cusbtiny -e -Uefuse:w:0xfe:m -Uhfuse:w:0b11010111:m -Ulfuse:w:0xE2:m -Uflash:w:CM3POE7_12.06.hex:i

# Programming PowerSafe with USB0 serial port adapter
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -carduino -P/dev/ttyUSB0 -b19200 -D -Uflash:w:CM3POE7_12.06.hex:i


