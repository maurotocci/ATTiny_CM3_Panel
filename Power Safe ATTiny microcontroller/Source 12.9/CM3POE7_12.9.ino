/*
================================================================================================

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

================================================================================================
*/

// ---------------------------------------------------------------------------------------------
// General Settings
// ---------------------------------------------------------------------------------------------
#include <ATTinyCore.h>
#include <EEPROM.h>

#define DEBUG         true              // Enable debug event to serial port
const boolean sen   = true;             // Send serial enable
const char*   fw    = "12.9";           // Firmware version
unsigned int  mode;                     // EEPROM Operation Mode: 0 = Manual / 255 = Autostart

// ---------------------------------------------------------------------------------------------
// Pin Output
//                  PRG   ATtiny Pin and description                  CM3 Pin       Active
// ---------------------------------------------------------------------------------------------
#define GS          0     // PD0 30 - Shutdown command to CM3         25            LOW
#define LG          4     // PD4 2  - Led Green                                     LOW
#define LR          5     // PD5 9  - Led Red                                       LOW
#define DON         14    // PB6 7  - Display ON                      27            HIGH
#define LB          15    // PB7 8  - Led Blue                                      LOW
#define ITD         16    // PC7 20 - Interrupt touch display                       HIGH
#define EN5V        25    // PA2 3  - Enable power CM3 start                        HIGH

// ---------------------------------------------------------------------------------------------
// Pin Input
//                  PRG   ATtiny Pin and description                  CM3 Pin       Active
// ---------------------------------------------------------------------------------------------
#define SHDNDONE    1     // PD1 31 - Shutdown done                   26            LOW
#define INT1        2     // PD2 32 - Interrupt PCF8523 0x68 &
                          //          Proximity APDS9960 0x39
                          //          Active is LOW (open drain)
#define P2          3     // PD3 1  - P2 Button                                     LOW

// ---------------------------------------------------------------------------------------------
// Value Measurement Analog Input
// ---------------------------------------------------------------------------------------------
unsigned int    dati[7];                  // Analog inputs measurement
  
// ---------------------------------------------------------------------------------------------
// Limits Measurement Analog Input (mV)
// ---------------------------------------------------------------------------------------------
const unsigned int VL5        = 5000;     // 5V EXT
const unsigned int AllVSC     = 1300;     // Minimum voltage alarm threshold. Forced power off 
const unsigned int GmVSC      = 2400;     // Supercap 30F minimum ignition voltage threshold
const unsigned int BmVSC      = 2200;     // Led Blue supercap in charge
const unsigned int RmVSC      = 1500;     // Led Red supercap minimum voltage threshold

// ---------------------------------------------------------------------------------------------
// Serial port settings
// ---------------------------------------------------------------------------------------------
# if DEBUG
  #define vb  115200          // Serial baud (Tested up to 115200)
# endif

// ---------------------------------------------------------------------------------------------
// Timer settings
// ---------------------------------------------------------------------------------------------
unsigned long int VTIME;

unsigned long int T0;
unsigned int TM           = 1000;   // Loop millisecond power safe control

unsigned long int T1;
unsigned int TD           = 2000;   // Loop millisecond interrupt display

unsigned long int D1;
unsigned int TD1          = 1000;   // Loop millisecond display connected check

// ---------------------------------------------------------------------------------------------
// Set Timer Parameters
// ---------------------------------------------------------------------------------------------
unsigned int nl                 = 0;          // Print line counter Serial
const unsigned int nr           = 10;         // Number lines Serial print

byte gos                        = 0;          // Go shutdown
byte pgs                        = 0;          // Press P2 to shutdown

byte en5                        = 0;
char* sts                       = "";         // Serial messages
byte sint                       = 0;          // Interrupt Display

byte cmp                        = 0;          // 0 = CM3 started - 1 = Boot or problem start
byte bte                        = 0;          // 1 = Boot ok

byte dys                        = 0;          // Check if display is connect to board
char* dsts                      = "";         // Status message display connected

char* opm                       = "";         // Message operating mode

void setup() {

  // ---------------------------------------------------------------------------------------------
  // Pin set and configure
  // ---------------------------------------------------------------------------------------------
  pinMode(GS,           OUTPUT);
  pinMode(LR,           OUTPUT);
  pinMode(LG,           OUTPUT);
  pinMode(LB,           OUTPUT);
  pinMode(ITD,          INPUT);
  pinMode(EN5V,         OUTPUT);

  // Set all pin to Off
  digitalWrite(GS,      LOW);           // Go Shutdown - H = Attiny send shutdown to CM3
  digitalWrite(LR,      HIGH);
  digitalWrite(LG,      HIGH);
  digitalWrite(LB,      HIGH);

  digitalWrite(EN5V,    LOW);           // LOW = Disable internal 5V

  pinMode(SHDNDONE,     INPUT_PULLUP);  // Set pullup - Confirm that the CM3 has been turned off
  pinMode(INT1,         INPUT);
  pinMode(P2,           INPUT);

  pinMode(DON,          OUTPUT);
  digitalWrite(DON,     LOW);

  // ---------------------------------------------------------------------------------------------
  // Check change Operative Mode - read/write EEPROM value from address 250 ( Button P2 )
  // ---------------------------------------------------------------------------------------------

    if ( digitalRead(P2) == 0 && EEPROM.read(250) == 0 ) {
      EEPROM.write(250, 255);
      digitalWrite(LG, LOW);
      delay(TM);    
    }

    if ( digitalRead(P2) == 0 && EEPROM.read(250) == 255 ) {
      EEPROM.write(250, 0);
      digitalWrite(LB, LOW);
      delay(TM);    
    }

    mode = EEPROM.read(250);

    if ( mode == 0 ) {
      opm = "MAN";
      digitalWrite(LB, LOW);
      delay(TM);    
    }    

    if ( mode == 255 ) {
      opm = "AUTO";
      digitalWrite(LG, LOW);
      delay(TM);    
    }    
    
  // ------------------------------------------------------------------------------- 
  # if DEBUG
  // ------------------------------------------------------------------------------- 
    Serial.begin(vb);
    delay(250);
    Serial.println();
    Serial.print("ACME Systems - Firmware:");
    Serial.print(fw);

    Serial.print(" ChipID:");
  
    if ( EEPROM.read(100) <= 16 ) {
      Serial.print(0);
    }    

    Serial.print(EEPROM.read(100),HEX);

    if ( EEPROM.read(200) <= 16 ) {
      Serial.print(0);
    }    

    Serial.print(EEPROM.read(200),HEX);

    Serial.print(" Operating Mode:");
    Serial.println(opm);
    
    Serial.flush();
    delay(1000);
    
    title();
  # endif

  // Start ADC
  ADCSRA |= _BV(ADEN);

}

void loop() {

  // ---------------------------------------------------------------------------------------------
  // Set millis() microcontroller
  // ---------------------------------------------------------------------------------------------
  VTIME = millis();

  // ---------------------------------------------------------------------------------------------
  // Reset or overflow millis()
  // ---------------------------------------------------------------------------------------------
  if (VTIME == TM) {
    T0 = millis();
    T1 = millis();
    D1 = millis();
  }

  // ---------------------------------------------------------------------------------------------
  // Loop values reading and Power control
  // ---------------------------------------------------------------------------------------------
  if ( VTIME >= T0 )
  {
    cmp  = digitalRead(SHDNDONE); // 0 = CM3 Power off or wait boot

    // ---------------------------------------------------------------------------------------------
    // Read digital input and ADC data
    // ---------------------------------------------------------------------------------------------
    dati[0] = digitalRead(P2);    // P2 Button
    dati[1] = digitalRead(INT1);  // INT# - Interrupt RTC / Proximity
    
    dati[2] = readVcc(7,3305);    // Volt Supercap       ADC7
    dati[3] = readVcc(0,3305);    // Power 1.8 V         ADC0
    dati[4] = readVcc(1,4300);    // Power 3.3 V         ADC1
    dati[5] = readVcc(2,6610);    // External 5V         ADC2
    dati[6] = readVcc(3,6610);    // Internal 5V         ACD3
    dati[7] = readVcc(6,441);     // mA Display          ADC6
    
    // ---------------------------------------------------------------------------------------------
    // RGB led color status
    // ---------------------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------------------
    // > 2400mV - CM3 started
    // ---------------------------------------------------------------------------------------------
    if ( dati[2] > GmVSC && cmp == 0 ) {
        digitalWrite(LG, LOW);
        delay(20);
    }

    // ---------------------------------------------------------------------------------------------
    // > 2400mV - CM3 problem
    // ---------------------------------------------------------------------------------------------
    if ( dati[2] > GmVSC && cmp == 1 ) {
        digitalWrite(LB, LOW);
        delay(20);
    }

    // ---------------------------------------------------------------------------------------------
    // > 2200mV and < 2400mV
    // ---------------------------------------------------------------------------------------------
    if ( dati[2] > BmVSC && dati[2] < GmVSC ) {
        digitalWrite(LB, LOW);
        delay(20);
    }

    // ---------------------------------------------------------------------------------------------
    // > 1500mV and < 2200mV
    // ---------------------------------------------------------------------------------------------
    if ( dati[2] > RmVSC && dati[2] < BmVSC ) {
        digitalWrite(LB, LOW);
        digitalWrite(LR, LOW);
        delay(20);
    }

    // ---------------------------------------------------------------------------------------------
    // < 1500mV
    // ---------------------------------------------------------------------------------------------
    if ( dati[2] < RmVSC ) {
        digitalWrite(LR, LOW);
        delay(20);
    }

    // ---------------------------------------------------------------------------------------------
    // Power Functions - Minimum voltage alarm threshold. Forced power off
    // ---------------------------------------------------------------------------------------------
    if ( dati[2] < AllVSC ) {
      digitalWrite(EN5V,  LOW);
      gos = 0;
      en5 = 0;
      sts = "Low Supercap";
    }

    // ---------------------------------------------------------------------------------------------
    // Loop check display interrupt
    // ---------------------------------------------------------------------------------------------
    if ( VTIME >= T1 )
    {
      if ( digitalRead(ITD) == 1 ) {
        sint = 1;    
        T1 = VTIME + TD;
      } else {
        sint = 0;    
      }
    }

    // -----------------------------------------------------------------
    // Section Autostart CM3 Panel
    // -----------------------------------------------------------------
    if ( dati[2] > GmVSC && dati[5] > VL5 && cmp == 1 && mode == 255 ) {
      digitalWrite(LR, HIGH);
      digitalWrite(LG, LOW);
      digitalWrite(LB, HIGH);
      delay(250);                                               

      digitalWrite(EN5V,  HIGH);
      digitalWrite(GS,    LOW);
      en5 = 1;
      pgs = 0;
      sts = "Boot";
    }

    // -----------------------------------------------------------------------
    // Section Manual start CM3 Panel ( RTC or Proximity, P2 or INT Display )
    // -----------------------------------------------------------------------
    if ( dati[2] > GmVSC && dati[5] > VL5 && cmp == 1 && mode == 0 && ( dati[0] == 0 || dati[1] == 0 || sint == 1 ) ) {
      digitalWrite(LR, HIGH);
      digitalWrite(LG, LOW);
      digitalWrite(LB, HIGH);
      delay(250);                                               

      digitalWrite(EN5V,  HIGH);
      digitalWrite(GS,    LOW);
      en5 = 1;
      pgs = 0;
      sts = "Boot";
    }

    // -----------------------------------------------------------------
    // Status CM3 Panel Active
    // -----------------------------------------------------------------
    if ( cmp == 0 && pgs == 0 ) {
      digitalWrite(EN5V,  HIGH);
      digitalWrite(DON,   HIGH);
      gos = 0;
      pgs = 0;
      en5 = 1;
      bte = 1;
      sts = "OK";
    }

    // -----------------------------------------------------------------
    // Status CM3 Panel OFF
    // -----------------------------------------------------------------
    if ( cmp == 1 && bte == 1 && gos == 0 ) {
      digitalWrite(EN5V,   LOW);
      digitalWrite(DON,   LOW);
      bte = 0;
      en5 = 0;
      gos = 0;
      sts = "Off";
    }

    // -----------------------------------------------------------------
    // Manual shutdown ( Button P2 )
    // -----------------------------------------------------------------
    if ( dati[0] == 0 && pgs == 0 && cmp == 0 && mode == 0 ) {
      digitalWrite(LR, LOW);
      digitalWrite(LG, HIGH);
      digitalWrite(LB, HIGH);
      delay(500);                                               

      digitalWrite(GS, HIGH);
      digitalWrite(DON, LOW);
      gos = 1;
      pgs = 1;
      sts = "Shutdown";
    }
    
    // -----------------------------------------------------------------
    // Low voltage alarm - send shutdown
    // -----------------------------------------------------------------
    if ( dati[5] < VL5 && cmp == 0 ) {
      digitalWrite(GS, HIGH);
      digitalWrite(DON, LOW);
      gos = 1;
      sts = "Shutdown";
    }

    // -----------------------------------------------------------------
    // Shutdown completed and poweroff
    // -----------------------------------------------------------------
    if ( cmp == 1 && gos == 1 ) {
      digitalWrite(EN5V,   LOW);
      digitalWrite(GS,      LOW);
      gos = 0;
      en5 = 0;
      sts = "Off";
    }

    // ---------------------------------------------------------------------------------------------
    // Disable power safe ( but control display connection active )
    // ---------------------------------------------------------------------------------------------
    if ( bte == 0 && dati[6] > VL5 && dati[5] > VL5 && dys == 0 ) {
      digitalWrite(DON, HIGH);
      sts = "Boot";
      D1 = VTIME + TD1;
      dys = 1;
    }

    // ---------------------------------------------------------------------------------------------
    // Display ON message
    // ---------------------------------------------------------------------------------------------
    if ( dys == 1 ) {
      dsts = "Disp. ON";
      D1 = VTIME + TD1;
      dys = 2;
    }

    // ---------------------------------------------------------------------------------------------
    // Loop check if display is connect 
    // ---------------------------------------------------------------------------------------------
    if ( VTIME >= D1 && dys == 2 && dati[7] < 10 )
    {
        digitalWrite(DON, LOW);
        dsts = "Disp. OFF";
        dys = 3;
    }

    // -----------------------------------------------------------------
    // Variable reset check display connected
    // -----------------------------------------------------------------
    if ( dati[5] < VL5 ) {
        dys = 0;
    }

    // ------------------------------------------------------------------------------- 
    # if DEBUG
    // ------------------------------------------------------------------------------- 
      if (sen) {
        Serial.begin(vb);
        nl++;
        
        if ( nl > nr ) {
          title();
          nl = 1;
        } 
  
        // ---------------------------------------------------------------------------------------------
        // Print to serial data
        // ---------------------------------------------------------------------------------------------
        for (int i = 0; i <= 7; i++) {

          // Full-scale current display
          if ( i == 7 && dati[7] > 2048 ) {
            Serial.print(0);
          } else {
            Serial.print(dati[i]);
          }
          
          Serial.print('\t');
        }
  
        Serial.print(gos);
        Serial.print('\t');
        Serial.print(en5);
        Serial.print('\t');
        Serial.print(digitalRead(SHDNDONE));
        Serial.print('\t');
        Serial.print(sint);
        Serial.print('\t');
        Serial.print(sts);
        Serial.print(' ');
        Serial.println(dsts);
        Serial.flush();

        dsts = "";
      } 
    # endif

    // Turn off leds
    digitalWrite(LR, HIGH);
    digitalWrite(LG, HIGH);
    digitalWrite(LB, HIGH);

    T0 = VTIME + TM;
  }
}

// ------------------------------------------------------------------------------- 
# if DEBUG
// ------------------------------------------------------------------------------- 
  void title() {
      Serial.begin(vb);
      Serial.println();
      Serial.println("P2\tINT\tSC\t1V8\t3V3\t5VE\t5VI\tBkl\tGS\tE5V\tCMP\tDI\tCM3");
      Serial.flush();
  }
# endif

// ------------------------------------------------------------------------------- 
// ADC converter
// ------------------------------------------------------------------------------- 
unsigned long readVcc(unsigned int d, unsigned long Factor) {
  uint8_t low;
  uint8_t high;
  unsigned long result;
  ADMUX = d + 64;                       // ADMUX Set Register
  delay(2);                             // Wait for Vref to settle 2ms
  ADCSRA |= _BV(ADSC);                  // Start conversion
  while (bit_is_set(ADCSRA, ADSC));     // Measuring
  low  = ADCL;                          // Must read ADCL first - it then locks ADCH
  high = ADCH;                          // Unlocks both
  result = (high << 8) | low;           // Numeric value
  return (result*Factor) >>10;          // mV / mA value
}
