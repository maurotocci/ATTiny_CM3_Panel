# CM3 Panel 7 POE - ACME Systems Industrial computer module display

https://www.acmesystems.it/CM3-PANEL-7-POE

### Examples programming ATTiny microcontroller from command line ( Raspbian OS )

Required install avrdude. The programming of the microcontroller can be performed directly from the CM3 Panel POE microprocessor board through the serial port, using a TTL 3V3 serial adapter.

Ex: sudo apt-get install avrdude

### Flash Optiboot bootloader width Atmel ICE
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -catmelice_isp -Pusb -e -Uefuse:w:0xfe:m -Uhfuse:w:0b11010111:m -Ulfuse:w:0xE2:m -Uflash:w:optiboot_attiny88_8000000L.hex:i 

## Programming Bootloader with USBTiny
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -cusbtiny -e -Uefuse:w:0xfe:m -Uhfuse:w:0b11010111:m -Ulfuse:w:0xE2:m -Uflash:w:optiboot_attiny88_8000000L.hex:i 

### Programming PowerSafe with USBTiny
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -cusbtiny -e -Uefuse:w:0xfe:m -Uhfuse:w:0b11010111:m -Ulfuse:w:0xE2:m -Uflash:w:CM3POE7_12.9.hex:i

## Programming PowerSafe with USB serial port adapter
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -carduino -P/dev/ttyUSB0 -b19200 -D -Uflash:w:CM3POE7_12.9.hex:i

