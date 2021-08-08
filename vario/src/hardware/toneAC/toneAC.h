/* ---------------------------------------------------------------------------
toneAC Library - v1.5.0 - 04/27/2019
AUTHOR/LICENSE:
Slightly modified ToneAC library created by Tim Eckel - tim@leethost.com
Copyright 2019 License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
LINKS:
Project home: https://github.com/teckel12/arduino-toneac
Blog: http://forum.arduino.cc/index.php?topic=142097.0
DISCLAIMER:
This software is furnished "as is", without technical support, and with no 
warranty, express or implied, as to its usefulness for any purpose.
PURPOSE:
Replacement to the standard tone library with the advantage of nearly twice
the volume, higher frequencies (even if running at a lower clock speed),
higher quality (less clicking), nearly 1.5k smaller compiled code and less
stress on the speaker. Disadvantages are that it must use certain pins and
it uses two pins instead of one. But, if you're flexible with your pin
choices, this is a great upgrade. It also uses timer 1 instead of timer 2,
which may free up a conflict you have with the tone library. It exclusively 
uses port registers for the fastest and smallest code possible.
USAGE:
Connection is very similar to a piezo or standard speaker. Except, instead
of connecting one speaker wire to ground you connect both speaker wires to
Arduino pins. The pins you connect to are specific, as toneAC lets the
ATmega microcontroller do all the pin timing and switching. This is
important due to the high switching speed possible with toneAC and to make
sure the pins are alyways perfectly out of phase with each other
(push/pull). See the below CONNECTION section for which pins to use for
different Arduinos. Just as usual when connecting a speaker, make sure you
add an inline 100 ohm resistor between one of the pins and the speaker wire.
CONNECTION:
  Pins  9 & 10 - ATmega328, ATmega128, ATmega640, ATmega8, Uno, Leonardo, etc.
  Pins 11 & 12 - ATmega2560/2561, ATmega1280/1281, Mega
  Pins 12 & 13 - ATmega1284P, ATmega644
  Pins 14 & 15 - Teensy 2.0
  Pins 25 & 26 - Teensy++ 2.0
SYNTAX:
  toneAC( frequency [, volume ] ) - Play a note.
    Parameters:
      * frequency  - Play the specified frequency indefinitely, turn off with noToneAC().
      * volume     - [optional] Set a volume level. (default: 10, range: 0 to 10 [0 = off])
  noToneAC()  - Stop playing.
--------------------------------------------------------------------------- */

#ifndef toneAC_h
  #define toneAC_h

  #if defined(ARDUINO) && ARDUINO >= 100
    #include <Arduino.h>
  #elif defined(ARDUINO)
    #include <WProgram.h>
  #else
    #include <stdint.h>
    #include <avr/io.h>
  #endif

  #if defined (__AVR_ATmega32U4__) || defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
    #define PWMT1AMASK DDB5
    #define PWMT1BMASK DDB6
    #define PWMT1DREG DDRB
    #define PWMT1PORT PORTB
  #elif defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__)
    #define PWMT1AMASK DDD4
    #define PWMT1BMASK DDD5
    #define PWMT1DREG DDRD
    #define PWMT1PORT PORTD
  #else
    #define PWMT1AMASK DDB1
    #define PWMT1BMASK DDB2
    #define PWMT1DREG DDRB
    #define PWMT1PORT PORTB
  #endif

  #if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__)
    #define TIMSK1 TIMSK
  #endif

  #define NOTONEAC 0
  #define PLAY_FOREVER 0

  #define TONEAC_VOL_0     0
  #define TONEAC_VOL_1     200
  #define TONEAC_VOL_2     100
  #define TONEAC_VOL_3     67
  #define TONEAC_VOL_4     50
  #define TONEAC_VOL_5     40
  #define TONEAC_VOL_6     33
  #define TONEAC_VOL_7     29
  #define TONEAC_VOL_8     22
  #define TONEAC_VOL_9     11
  #define TONEAC_VOL_10    2

  void toneAC(const uint32_t frequency = NOTONEAC, const uint8_t volume = TONEAC_VOL_10);
  void toneACopt(const uint32_t frequency);
  void noToneAC();

  void pulseToneInit();
  void pulseToneStart();
  void pulseToneStop();
  void pulseToneSet(const uint32_t &frequency, const uint16_t beep_len = 10, const uint16_t mute_len = 4);


  struct PulseToneState {
    uint16_t beep = 0;
    uint16_t mute = 1;
    uint16_t beep_len = 10;
    uint16_t mute_len = 4;
    uint32_t tone_frequency = 200;
  };

#endif