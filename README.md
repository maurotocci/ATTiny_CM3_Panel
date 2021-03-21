<h3 align="center">CM3 Panel 7 POE - ACME Systems Industrial computer module display</h3>

<p align="center">
  <a href="https://www.acmesystems.it/CM3-PANEL-7-POE/">
    <img src="https://www.acmesystems.it/www/CM3-PANEL-7-POE/CM3-PANEL-7-POE_2048.jpg" alt="CM3 Panel 7 POE" width="600px">
  </a>
</p>


In this area are published the sources of the ATTiny microcontroller which manage, through a supercapacitor, the correct shutdown of the ACME Systems CM3 Panel POE industrial board in case of external power failure. 
Compared to the previous version of CM3 Panel, the microcontroller also allows the board to operate even without the touch display, for example to be installed on DIN rail supports inside electrical panels.

All information on the operating status of the microprocessor board is sent through the microcontroller on the serial port on the EXP2 connector.

An RGB LED on the board, connected to the pins of the microcontroller, indicates the state of charge of the supecapacitor.

## Description of the functions contained in the firmware 12.9

The Power Safe function is managed through an ATTiny microcontroller. The latter communicates with the microprocessor via GPIO lines CM3. The micro takes measurements of all power stages of the CME Panel 7 at 1 second intervals to ensure through the supercap the correct shutdown of the board in the event of a power failure.

In addition to these measurements, it communicates with the microprocessor through other lines to monitor the following situations and more precisely:

Operating system started regularly.

Shutdown performed regularly.

The microcontroller also manages the stage of the display power supply circuit. This will allow you to use the card as well without connecting the touch display.

Other 3 lines of the microcontroller are used in the following way:
Real Time Clock interrupt, for switching on the board in MANUAL at a predetermined time (Power Safe Active).
Touch screen interrupt for MANUAL ignition (Power Safe Active).
P2 button, for manual switching on / off of the board (Power Safe Active).

An RGB LED on the board, connected to the microcontroller, indicates the following operating modes through colors:
Supercap charge level.
MANUAL / AUTOMATIC operating mode.
Do not press the reset button of the microcontroller while the board is running.

The firmware on the microcontroller was set as follows:

### Changing the AUTOMATIC / MANUAL operating mode:

JUMPER JPROG BYPASS = OPEN

JUMPER Supercap in CHG position

### The board must be off and the RGB LED must NOT flash.
Press and hold the P2 button and power up the card. The LED will emit a RED light for a few seconds, release the button P2. This situation will indicate that the board operating mode will be in MANUAL. The RGB led will start blinking indicating the state of charge of the Supercap, once charged, the board must be switched on manually as follows indicated.

JUMPER JPROG BYPASS = OPEN

JUMPER Supercap in CHG position

To switch to AUTOMATIC mode instead, the board must always be off and the RGB LED must NOT flash.
Press and hold the P2 button and power up the board. The LED will emit a GREEN light for a few seconds, release the button
P2. This situation will indicate that the board operating mode will be in AUTOMATIC. The RGB led will start blinking
indicating the state of charge of the Supercap, once charged, the board will turn on automatically.


## Development and test environment mode.


The board has 2 JUMPERS, JPROG BYPASS and one near the supercap with 2 positions, CHG (supercap connected) and DCHG (discharge supercap).

In the development, test environment, the Power Safe control can be disabled as follows:

Remove the supercap JUMPER, insert the jumper on JPROG BYPASS and power the board.

The same, it will leave immediately without carrying out automatic shutdown in the event of a power failure. However, the microcontroller continues to manage the circuit power supply of the display and if it is not present, turn off the relative power supply.

## Power Safe mode ACTIVE.

JUMPER JPROG BYPASS = OPEN

JUMPER Supercap in CHG position

In this mode, the board when the RGB LED is powered emits the GREEN color for one second which indicates the fully automatic operation, 

Then flashes indicating the state of charge of the Supercap and more precisely:

Supercap voltage <1.5V = RED, Supercap voltage between 1.5V and <2.2V = MAGENTA, Supercap voltage between 2.2V and <2.4V = BLUE, Supercap voltage> = 2.4 V = GREEN.

When the voltage> = 2.4 V on supercap is reached, the power supply circuit CM3 is activated:

GREEN blinking continues until boot is complete and then blinks faster in the same color. 
Lacking power supply, the board switches off automatically by carrying out automatic shutdown. When the power returns, it switches on again
automatically.

### Power Safe mode ACTIVE but board switching on and off in MANUAL.

JUMPER JPROG BYPASS = OPEN

JUMPER Supercap in CHG position

In this mode, when the board is powered, the RGB LED emits the RED color for one second which indicates the MANUAL operation, then flashes indicating the state of charge of the Supercap and more precisely:

Tab Off:

Supercap voltage <1.5V = RED, Supercap voltage between 1.5V and <2.2V = MAGENTA, Supercap voltage between 2.2V and <2.4V = BLUE, Supercap voltage> = 2.4 = GREEN.

The board can be turned on only and exclusively through the following events:

By touching the touch screen of the display;

Real Time Clock interrupt signal;

By pressing button P2;

When the board is turned on and the P2 button is pressed, it automatically turns off. This also happens in the event that it comes to lack of external power supply.

On the expansion connector of the board, EXP2, through a TTL 3V3 serial USB adapter, the microcontroller transmits all
information of all monitored lines.

### Notes

The tests performed in the ACME Systems laboratories were performed on the 2020-12-02-raspios-buster-armhf-lite operating system version with Node Red, XORG and Google Chrome installed. In the Desktop version, the board demonstrated faster shutdown times than in the Lite version.

From the project calculations, and from the stress tests performed, the use of a 30 F supercap guarantees that even when there is a power failure during boot, the mocrocontroller is able to perform the automatic shutdown of the board regularly.

### The serial communication parameters are as follows:
Speed           : 115200 baudrate

Parity          : none

Bits            : 8

Stopbits        : 1

Flow control    : none


