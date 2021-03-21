<h3 align="center">CM3 Panel 7 POE - ACME Systems Industrial computer module display</h3>

<p align="center">
  <a href="https://www.acmesystems.it/CM3-PANEL-7-POE/">
    <img src="https://www.acmesystems.it/www/CM3-PANEL-7-POE/CM3-PANEL-7-POE_attiny.svg" alt="CM3 PAnel 7 POE" width="80%">
  </a>
</p>


### Examples programming ATTiny microcontroller from command line ( Raspbian OS )

Required install avrdude. The programming of the microcontroller can be performed directly from the CM3 Panel POE microprocessor board through the serial port, using a TTL 3V3 serial adapter.

Ex: <h4>sudo apt-get install avrdude</h4>

### Flash Optiboot bootloader width Atmel ICE
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -catmelice_isp -Pusb -e -Uefuse:w:0xfe:m -Uhfuse:w:0b11010111:m -Ulfuse:w:0xE2:m -Uflash:w:optiboot_attiny88_8000000L.hex:i 

### Programming Bootloader with USBTiny
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -cusbtiny -e -Uefuse:w:0xfe:m -Uhfuse:w:0b11010111:m -Ulfuse:w:0xE2:m -Uflash:w:optiboot_attiny88_8000000L.hex:i 

### Programming PowerSafe with USBTiny
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -cusbtiny -e -Uefuse:w:0xfe:m -Uhfuse:w:0b11010111:m -Ulfuse:w:0xE2:m -Uflash:w:CM3POE7_12.9.hex:i

### Programming PowerSafe with USB serial port adapter
$ sudo avrdude -Cavrdude.conf -v -pattiny88 -carduino -P/dev/ttyUSB0 -b19200 -D -Uflash:w:CM3POE7_12.9.hex:i

### Steps ATTiny programming ( Source 12.9 directory )

1. Remove jumper SJVCC on USBTiny

<p align="center">
    <img src="img/usbtiny.jpg" width="200px">
</p>

2. Connect the USBTiny adapter to PC and connect on ICSP port board
3. Power on Board
4. Lanch the command ./flash_bootloader.sh
5. Now the boot loader is programmed
6. Connect USB serial adapter to PC or USB port CM3 Panel ( Serial port. Ex: /tty/USBO or COM1 )
7. Connect serial USB adapter on EXP2 pin connector
8. Press and hold the button Reset MicroP P3
9. Lanch the command ./flash_powersafe.sh and release immediately the button Reset MicroP P3

At the end of the programming, the RGB LED on the board will indicate the operating mode and the state of charge of the supercapacitor.

## Notes

The communication on the correct functioning of the Power Safe, between the CM3 Panel and the ATTiny microcontroller takes place through 2 GPIOs, GPIO 25 is used by the ATTiny to notify the microprocessor to carry out the shutdown (LOW), while the GPIO26 (status HIGH) communicates to the ATTiny that it shutdown occurred regularly, consequently ATTiny deactivates the board power supply.

`================================================================================================

  Description :   CM3 Panel 7.3 POE - Power Safe Controller
  Rev.        :   12.9
  MCU         :   ATTiny88 - With optiboot bootloader - 8Mhz internal clock
  Author      :   Mauro Tocci - ACME Systems
  Last Update :   10 Mar 2021

  ----------------------------------------------------------------------------------------------
  Var         PRG   ATTiny Pin and description                        CM3 Pin       Active
  ----------------------------------------------------------------------------------------------
  GS          0     PD0 30 - Shutdown command to CM3                  25            LOW
  SHDNDONE    1     PD1 31 - Shutdown done from CM3                   26            HIGH
  INT1        2     PD2 32 - Interrupt PCF8523 0x68 & APDS9960 0x39                 LOW
  P2          3     PD3 1  - P2 Button
  LG          4     PD4 2  - Led Green                                              LOW
  LR          5     PD5 9  - Led Red                                                LOW
              6     PD6 10 - AIN0=TX
              7     PD7 11 - AIN1=RX
              8     PB0 12 - Free
              9     PB1 13 - Free
              10    PB2 14 - Free
              11    PB3 15 - MOSI
              12    PB4 16 - MISO
              13    PB5 17 - SCK
                        18 - AVCC VREF
  DON         14    PB6 7  - Display ON                               27            HIGH
  LB          15    PB7 8  - Led Blue                                               LOW
  ITD         16    PC7 20 - Interrupt touch display
              17    PC0 23 - ADC Power 1.8 V
              18    PC1 24 - ADC Power 3.3 V
              19    PC2 25 - ADC External 5V
              20    PC3 26 - ADC Internal 5V
              21    PC4 27 - SDA
              22    PC5 28 - SCL
              23    PA0 19 - ADC Measure current Display
              24    PA1 22 - ADC Volt Supercap
  EN5V        25    PA2 3  - Enable internal 5V
              26    PA3 6  - Free
              27    PC6 29 - Reset micro

================================================================================================`




