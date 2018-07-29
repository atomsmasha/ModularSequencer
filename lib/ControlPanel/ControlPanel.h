#ifndef ControlPanel_h
#define ControlPanel_h

#include "Adafruit_MCP23017.h"
#include <Arduino.h>

#define DEFAULT_PINS 16
#define DEFAULT_DEBOUNCE_INTERVAL 20
#define MAX_16 65535
#define BUTTON_1 65279
#define BUTTON_2 65023
#define BUTTON_3 64511
#define BUTTON_4 63487
#define BUTTON_5 61439
#define BUTTON_6 57343
#define BUTTON_7 49151
#define BUTTON_8 32767
#define BUTTON_9 65534
#define BUTTON_10 65533
#define BUTTON_11 65531
#define BUTTON_12 65527
#define BUTTON_13 65519
#define BUTTON_14 65503
#define BUTTON_15 65471
#define BUTTON_16 65407

class ControlPanel{
	public:
		// VARIABLES
		bool button_registered = false;
		byte button_returned;
		// METHODS
		ControlPanel();
		void init ( byte address );
		void readControlPanel();
		
	private:
		// VARIABLES
		Adafruit_MCP23017 _iochip;
		long _debounce_interval = DEFAULT_DEBOUNCE_INTERVAL;
		long _prev_debounce_millis = 0;
		uint16_t _last_button_state = MAX_16;
		int _button_array[16] = {
			BUTTON_1,
			BUTTON_2,
			BUTTON_3,
			BUTTON_4,
			BUTTON_5,
			BUTTON_6,
			BUTTON_7,
			BUTTON_8,
			BUTTON_9,
			BUTTON_10,
			BUTTON_11,
			BUTTON_12,
			BUTTON_13,
			BUTTON_14,
			BUTTON_15,
			BUTTON_16,
		};
		// METHODS
		void _init( byte address );
		byte _translateControlPanel( uint16_t button_state );
};

#endif