<h3 align="center">CM3 Panel 7 POE - ACME Systems Industrial computer module display</h3>

<p align="center">
  <a href="https://www.acmesystems.it/CM3-PANEL-7-POE/">
    <img src="https://www.acmesystems.it/www/CM3-PANEL-7-POE/CM3-PANEL-7-POE_attiny.svg" alt="CM3 PAnel 7 POE" width="80%">
  </a>
</p>


### Examples programming ATTiny microcontroller from command line ( Raspbian OS )

Required install avrdude. The programming of the microcontroller can be performed directly from the CM3 Panel POE microprocessor board through the serial port, using a TTL 3V3 serial adapter.

Ex: sudo apt-get install avrdude

### Flash Optiboot bootloader width Atmel ICE
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -catmelice_isp -Pusb -e -Uefuse:w:0xfe:m -Uhfuse:w:0b11010111:m -Ulfuse:w:0xE2:m -Uflash:w:optiboot_attiny88_8000000L.hex:i 

### Programming Bootloader with USBTiny
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -cusbtiny -e -Uefuse:w:0xfe:m -Uhfuse:w:0b11010111:m -Ulfuse:w:0xE2:m -Uflash:w:optiboot_attiny88_8000000L.hex:i 

### Programming PowerSafe with USBTiny
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -cusbtiny -e -Uefuse:w:0xfe:m -Uhfuse:w:0b11010111:m -Ulfuse:w:0xE2:m -Uflash:w:CM3POE7_12.9.hex:i

### Programming PowerSafe with USB serial port adapter
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -carduino -P/dev/ttyUSB0 -b19200 -D -Uflash:w:CM3POE7_12.9.hex:i

### Examples USBTiny adapter

<p align="center">
    <img src="img/usbtiny.jpg" width="50%">
</p>

