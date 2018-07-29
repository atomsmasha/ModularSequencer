#include <ControlPanel.h>
#include "Arduino.h"
#include "Adafruit_MCP23017.h"

ControlPanel::ControlPanel(){
}

void ControlPanel::init( byte address ){
    _init(address);
}

void ControlPanel::readControlPanel(){
    uint16_t button_state = _iochip.readGPIOAB();
    if(button_state != MAX_16){
        unsigned long debounce_millis = millis();
        if(button_state == _last_button_state){
            if(debounce_millis - _prev_debounce_millis >= DEFAULT_DEBOUNCE_INTERVAL){
                _prev_debounce_millis = debounce_millis;
                button_returned = _translateControlPanel( button_state );
                button_registered = (button_returned < 16) ? true : false;
            }
        }
        else{_last_button_state = button_state;}
    }
    else{button_registered = false;}
}

//
// PRIVATE METHODS
//

void ControlPanel::_init( byte address ){
    _iochip.begin(address);
    for(byte i = 0; i < DEFAULT_PINS; i++){
        _iochip.pinMode(i, INPUT);
        _iochip.pullUp(i, HIGH);
    }
}

byte ControlPanel::_translateControlPanel( uint16_t button_state ){
    for(byte i = 0; i < 16; i++){
        if(_button_array[i] == button_state){
            return i;
        }
    }
    return 99;
}