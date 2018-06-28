/*#include "Adafruit_MCP23017.h"
#include <Arduino.h>
#include <Wire.h>

class Matrix {
    const uint16_t max16 = 65535;
    Adafruit_MCP23017 mcp_matrix;
    uint16_t lastSelectorKeyState = max16;
    const short mcp_matrix_pins_to_set[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    long previousSelectorDebounceMillis = 0;
    bool selectorUpdated = false;

    const byte pin;
    int state;
    unsigned long buttonDownMs;

  public:
    Matrix(short mcp_pins_to_set) :
        //for(short i=0;i<mcp_pins_to_set.len;i++){
        //    mcp_matrix_pins_to_set[i] = mcp_pins_to_set[i];
        //}
        mcp_matrix_pins_to_set(mcp_pins_to_set);
    {
    }

    void setup() {
      pinMode(pin, INPUT_PULLUP);
      state = HIGH;
    }

    void loop() {
      int prevState = state;
      state = digitalRead(pin);
      if (prevState == HIGH && state == LOW) {
        buttonDownMs = millis();
      }
      else if (prevState == LOW && state == HIGH) {
        if (millis() - buttonDownMs < 50) {
          // ignore this for debounce
        }
        else if (millis() - buttonDownMs < 250) {
          // short click
          // TEST BY TOGGLING PIN 8
          digitalWrite(8, !digitalRead(8));
        }
        else  {
          // long click
          // TEST BY TOGGLING PIN 9
          digitalWrite(9, !digitalRead(9));
        }
      }
    }
};

Matrix button(7);



        mcp_selector.begin(0);
    for(int i = 0; i <= 15; i++){
        mcp_selector.pinMode(mcp_selector_setter[i], INPUT);
        mcp_selector.pullUp(mcp_selector_setter[i], HIGH);
    }

    uint16_t gridKeyState = mcp_grid.readGPIOAB();
    if(gridKeyState != max16){
        unsigned long currentGridDebounceMillis = millis();
        if(gridKeyState == lastGridKeyState && gridUpdated == false){
            if(currentGridDebounceMillis - previousGridDebounceMillis > debounceInterval){
                previousGridDebounceMillis = currentGridDebounceMillis;
                updateGrid(gridKeyState);
            }
        } else {
            lastGridKeyState = gridKeyState;
        }
    } else {
        gridUpdated = false;
    }

*/