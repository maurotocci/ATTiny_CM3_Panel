# Industrial computer module display

https://www.acmesystems.it/CM3-PANEL-7-POE

In this area are published the sources of the ATTiny microcontroller which manage, through a supercapacitor, the correct shutdown of the ACME Systems CM3 Panel POE industrial card in case of external power failure. 
Compared to the previous version of CM3 Panel, the microcontroller also allows the board to operate even without the touch display, for example to be installed on DIN rail supports inside electrical panels.

All information on the operating status of the microprocessor board is sent through the microcontroller on the serial port on the EXP2 connector.

An RGB LED on the board, connected to the pins of the microcontroller, indicates the state of charge of the supecapacitor.
