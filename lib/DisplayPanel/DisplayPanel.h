#ifndef DisplayPanel_h
#define DisplayPanel_h

#include <Arduino.h>
#include "LedControl.h"

#define DATAPIN 7
#define CLKPIN 5
#define CSPIN 6
#define NUMDEVICES 1

class DisplayPanel{
  public:
	// METHODS
	DisplayPanel();
	void init();
	void refreshDisplay();
	void updateSequence( byte sequence_byte, byte index );
	void updatePattern( byte pattern_number );
	void updateChannel( byte channel_number );

  private:
	// VARIABLES
	LedControl lc = LedControl(DATAPIN,CLKPIN,CSPIN,NUMDEVICES);
	byte _refresh_time = 50;
	long _previousLEDMillis = 0;
	bool _pattern_updated = false;
	byte _display_buffer[8] ={
    	B00000000,
    	B00000000,
    	B00000000,
    	B00000000,
		B00000000,
		B00000000,
		B00000000,
		B00000000,
	};
	byte _sequence_buffer[2] = {
		B00000000,
		B00000000,
	};
	byte _pattern_buffer[1] = {
		B00000000,
	};
	byte _channel_buffer[1] = {
		B00000000,
	};
	// METHODS
	void _init();
	void _writeDisplayPanel();
	void _writeDisplayBuffer( byte buffer_line, byte index);
};

#endif