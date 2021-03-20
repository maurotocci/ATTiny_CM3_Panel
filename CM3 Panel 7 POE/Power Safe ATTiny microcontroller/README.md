# CM3 Panel 7 POE - Industrial computer module display

https://www.acmesystems.it/CM3-PANEL-7-POE

In this area are published the sources of the ATTiny microcontroller which manage, through a supercapacitor, the correct shutdown of the ACME Systems CM3 Panel POE industrial card in case of external power failure. 
Compared to the previous version of CM3 Panel, the microcontroller also allows the board to operate even without the touch display, for example to be installed on DIN rail supports inside electrical panels.

All information on the operating status of the microprocessor board is sent through the microcontroller on the serial port on the EXP2 connector.

An RGB LED on the board, connected to the pins of the microcontroller, indicates the state of charge of the supecapacitor.

## Examples programming ATTiny microcontroller from command line ( Raspbian OS )

Required install avrdude

Ex: sudo apt-get install avrdude

## Flash Optiboot bootloader width Atmel ICE
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -catmelice_isp -Pusb -e -Uefuse:w:0xfe:m -Uhfuse:w:0b11010111:m -Ulfuse:w:0xE2:m -Uflash:w:optiboot_attiny88_8000000L.hex:i 

## Programming Bootloader with USBTiny
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -cusbtiny -e -Uefuse:w:0xfe:m -Uhfuse:w:0b11010111:m -Ulfuse:w:0xE2:m -Uflash:w:optiboot_attiny88_8000000L.hex:i 

## Programming PowerSafe with USBTiny
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -cusbtiny -e -Uefuse:w:0xfe:m -Uhfuse:w:0b11010111:m -Ulfuse:w:0xE2:m -Uflash:w:CM3POE7_12.9.hex:i

## Programming PowerSafe with USB0 serial port adapter
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -carduino -P/dev/ttyUSB0 -b19200 -D -Uflash:w:CM3POE7_12.9.hex:i

### Description of the functions contained in the firmware 12.9

The Power Safe function is managed through an ATTiny microcontroller. The latter communicates with the microprocessor via GPIO lines CM3. The micro takes measurements of all power stages of the CME Panel 7 at 1 second intervals to ensure through the supercap the correct shutdown of the board in the event of a power failure.

In addition to these measurements, it communicates with the microprocessor through other lines to monitor the following situations and more precisely:

Operating system started regularly.
Shutdown performed regularly.

The microcontroller also manages the stage of the display power supply circuit. This will allow you to use the card as well without connecting the touch display.

Other 3 lines of the microcontroller are used in the following way:
Real Time Clock interrupt, for switching on the board in MANUAL at a predetermined time (Power Safe Active).
Touch screen interrupt for MANUAL ignition (Power Safe Active).
P2 button, for manual switching on / off of the card (Power Safe Active).

An RGB LED on the board, connected to the microcontroller, indicates the following operating modes through colors:
Supercap charge level.
MANUAL / AUTOMATIC operating mode.
Do not press the reset button of the microcontroller while the board is running.

The firmware on the microcontroller was set as follows:

## Changing the AUTOMATIC / MANUAL operating mode:

JUMPER JPROG BYPASS = OPEN

JUMPER Supercap in CHG position

## The board must be off and the RGB LED must NOT flash.
Press and hold the P2 button and power up the card. The LED will emit a RED light for a few seconds, release the button P2. This situation will indicate that the card operating mode will be in MANUAL. The RGB led will start blinking indicating the state of charge of the Supercap, once charged, the board must be switched on manually as follows indicated.

JUMPER JPROG BYPASS = OPEN

JUMPER Supercap in CHG position

To switch to AUTOMATIC mode instead, the board must always be off and the RGB LED must NOT flash.
Press and hold the P2 button and power up the card. The LED will emit a GREEN light for a few seconds, release the button
P2. This situation will indicate that the card operating mode will be in AUTOMATIC. The RGB led will start blinking
indicating the state of charge of the Supercap, once charged, the card will turn on automatically.




