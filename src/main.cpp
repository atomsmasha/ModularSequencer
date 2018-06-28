#include <Wire.h>
#include <Arduino.h>
#include "Adafruit_MCP23017.h"
#include "LedControl.h"

// Connect pin #12 of the expander to Analog 5 (i2c clock)
// Connect pin #13 of the expander to Analog 4 (i2c data)
// Connect pins #15, 16 and 17 of the expander to ground (address selection = 0)
// Connect pin #9 of the expander to 5V (power)
// Connect pin #10 of the expander to ground (common ground)
// Connect pin #18 through a ~10kohm resistor to 5V (reset pin, active low)

LedControl lc=LedControl(12,10,11,1);  // Pins: DIN,CLK,CS, # of Display connected
long previousLEDMillis = 0;        // will store last time LED was updated

long clock = 100;
short step = 0;

const uint16_t max16 = 65535;
long debounceInterval = 20;

// setting up the grid/sequencer buttons
Adafruit_MCP23017 mcp_grid;
short mcp_grid_setter[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
uint16_t lastGridKeyState = max16;
long previousGridDebounceMillis =  0;
bool gridUpdated = false;

// setting up the channel and pattern selector buttons
Adafruit_MCP23017 mcp_selector;
short mcp_selector_setter[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
uint16_t lastSelectorKeyState = max16;
long previousSelectorDebounceMillis = 0;
bool selectorUpdated = false;

short currentPattern = 1;
short currentChannel = 1;
short lastPattern = 1;
short lastChannel = 1;

bool switchPattern = false;
bool switchChannel = false;
// bool patternCopied = false;

short channel1CurrentRunningPattern = 1;
short channel2CurrentRunningPattern = 1;
short channel3CurrentRunningPattern = 1;
short channel4CurrentRunningPattern = 1;
short channel1LastRunningPattern = 0;
short channel2LastRunningPattern = 0;
short channel3LastRunningPattern = 0;
short channel4LastRunningPattern = 0;

byte ledPanel[] =
{
   B00000000,
   B00000000,
   B00000000,
   B00000000,
   B00000000,
   B00000000,
   B00000000,
   B00000000,
};

byte selector[] =
{
    B00001000,
    B00001000,
    B00000000,
};

byte steps[] =
{
    B00001000,
    B00000100,
    B00000010,
    B00000001,
    B00001000,
    B00000100,
    B00000010,
    B00000001,
    B00001000,
    B00000100,
    B00000010,
    B00000001,
    B00001000,
    B00000100,
    B00000010,
    B00000001,
};

byte pattern[] =
{
    B00001001,
    B00001000,
    B00001000,
    B00001011,
};

byte pattern1[4];
byte pattern2[4];
byte pattern3[4];
byte pattern4[4];
byte pattern5[4];
byte pattern6[4];
byte pattern7[4];
byte pattern8[4];
byte pattern9[4];
byte pattern10[4];
byte pattern11[4];
byte pattern12[4];
byte pattern13[4];
byte pattern14[4];
byte pattern15[4];
byte pattern16[4];
byte pattern17[4];
byte pattern18[4];
byte pattern19[4];
byte pattern20[4];
byte pattern21[4];
byte pattern22[4];
byte pattern23[4];
byte pattern24[4];
byte pattern25[4];
byte pattern26[4];
byte pattern27[4];
byte pattern28[4];
byte pattern29[4];
byte pattern30[4];
byte pattern31[4];
byte pattern32[4];

void initializePatterns(){
    for(short i=0;i<4;i++){
        pattern1[i] = B00000000;
        pattern2[i] = B00000000;
        pattern3[i] = B00000000;
        pattern4[i] = B00000000;
        pattern5[i] = B00000000;
        pattern6[i] = B00000000;
        pattern7[i] = B00000000;
        pattern8[i] = B00000000;
        pattern9[i] = B00000000;
        pattern10[i] = B00000000;
        pattern11[i] = B00000000;
        pattern12[i] = B00000000;
        pattern13[i] = B00000000;
        pattern14[i] = B00000000;
        pattern15[i] = B00000000;
        pattern16[i] = B00000000;
        pattern17[i] = B00000000;
        pattern18[i] = B00000000;
        pattern19[i] = B00000000;
        pattern20[i] = B00000000;
        pattern21[i] = B00000000;
        pattern22[i] = B00000000;
        pattern23[i] = B00000000;
        pattern24[i] = B00000000;
        pattern25[i] = B00000000;
        pattern26[i] = B00000000;
        pattern27[i] = B00000000;
        pattern28[i] = B00000000;
        pattern29[i] = B00000000;
        pattern30[i] = B00000000;
        pattern31[i] = B00000000;
        pattern32[i] = B00000000;
    }
}

void setup() {  
    mcp_selector.begin(0);
    for(int i = 0; i <= 15; i++){
        mcp_selector.pinMode(mcp_selector_setter[i], INPUT);
        mcp_selector.pullUp(mcp_selector_setter[i], HIGH);
    }

    mcp_grid.begin(1);
    for(int i = 0; i <= 15; i++){
        mcp_grid.pinMode(mcp_grid_setter[i], INPUT);
        mcp_grid.pullUp(mcp_grid_setter[i], HIGH);
    }

    lc.shutdown(0,false);  // Wake up displays
    lc.shutdown(1,false);
    lc.setIntensity(0,5);  // Set intensity levels
    lc.setIntensity(1,5);
    lc.clearDisplay(0);  // Clear Displays
    lc.clearDisplay(1);

    initializePatterns();
}

void writeScreen()
{
  for (int i = 0; i < 8; i++){
    lc.setRow(0,i,ledPanel[i]);
  }
}

void updatePatternBuffer(){
    for(int i = 4; i < 8; i++){
        ledPanel[i] = (pattern[i-4] << 2);
    }
}

void updateSelectorBuffer(){
    ledPanel[0] = (selector[0] << 2);
    ledPanel[2] = (selector[1] << 2);
    ledPanel[3] = (selector[2] << 2);
}

void cursorGrid(){
    if (step >= 0 && step <= 3){
        ledPanel[4] ^= (steps[step] << 2);
        step++;
        return;
    }

    if (step >= 4 && step <= 7){
        ledPanel[5] ^= (steps[step] << 2);
        step++;
        return;
    }

    if (step >= 8 && step <= 11){
        ledPanel[6] ^= (steps[step] << 2);
        step++;
        return;
    }

    if (step >= 12 && step <= 15){
        ledPanel[7] ^= (steps[step] << 2);
        if(step == 15){
            step = 0;
            return;
        } else {
            step++;
        }
    }    
}

byte reverseBits(byte data){
    byte returnByte = 0;
    for(int i = 0; i <= 3; i++){
        bitWrite(returnByte, 7-i, bitRead(data, 0+i));
    }
    return(returnByte >> 4);
}

void updateGrid(uint16_t gridKeyState){
    gridKeyState = ~gridKeyState;
    pattern[2] ^= reverseBits(gridKeyState << 12 >> 12);
    pattern[3] ^= reverseBits(gridKeyState << 8 >> 12);
    pattern[0] ^= reverseBits(gridKeyState << 4 >> 12);
    pattern[1] ^= reverseBits(gridKeyState >> 12);
    gridUpdated = true;
}

void updateSelector(uint16_t selectorKeyState){
    /*selectorKeyState = ~selectorKeyState;
    selector[0] ^= reverseBits(selectorKeyState << 12 >> 12);
    selector[1] ^= reverseBits(selectorKeyState << 4 >> 12);
    selector[2] ^= reverseBits(selectorKeyState >> 12);
    selectorUpdated = true;*/
    if(selectorKeyState == 65279){
        selector[1] = B00001000;
        selector[2] = B00000000;
        if(switchPattern == false){
            lastPattern = currentPattern;
        }    
        currentPattern = 1;
        switchPattern = true;
    }

    if(selectorKeyState == 65023){
        selector[1] = B00000100;
        selector[2] = B00000000;
        if(switchPattern == false){
            lastPattern = currentPattern;
        }   
        currentPattern = 2;
        switchPattern = true;
    }

    if(selectorKeyState == 64511){
        selector[1] = B00000010;
        selector[2] = B00000000;
        if(switchPattern == false){
            lastPattern = currentPattern;
        }   
        currentPattern = 3;
        switchPattern = true;
    }

    if(selectorKeyState == 63487){
        selector[1] = B00000001;
        selector[2] = B00000000;
        if(switchPattern == false){
            lastPattern = currentPattern;
        }   
        currentPattern = 4;
        switchPattern = true;
    }

    if(selectorKeyState == 61439){
        selector[1] = B00000000;
        selector[2] = B00001000;
        if(switchPattern == false){
            lastPattern = currentPattern;
        }   
        currentPattern = 5;
        switchPattern = true;
    }

    if(selectorKeyState == 57343){
        selector[1] = B00000000;
        selector[2] = B00000100;
        if(switchPattern == false){
            lastPattern = currentPattern;
        }   
        currentPattern = 6;
        switchPattern = true;
    }

    if(selectorKeyState == 49151){
        selector[1] = B00000000;
        selector[2] = B00000010;
        if(switchPattern == false){
            lastPattern = currentPattern;
        }   
        currentPattern = 7;
        switchPattern = true;
    }

    if(selectorKeyState == 32767){
        selector[1] = B00000000;
        selector[2] = B00000001;
        if(switchPattern == false){
            lastPattern = currentPattern;
        }   
        currentPattern = 8;
        switchPattern = true;
    }

    if(selectorKeyState == 65534){
        selector[0] = B00001000;
        if(switchChannel == false){
            lastChannel = currentChannel;
        }   
        currentChannel = 1;
        //switchChannel = true;
    }

    if(selectorKeyState == 65533){
        selector[0] = B00000100;
        if(switchChannel == false){
            lastChannel = currentChannel;
        }  
        currentChannel = 2;
        //switchChannel = true;
    }

    if(selectorKeyState == 65531){
        selector[0] = B00000010;
        if(switchChannel == false){
            lastChannel = currentChannel;
        }  
        currentChannel = 3;
        //switchChannel = true;
    }

    if(selectorKeyState == 65527){
        selector[0] = B00000001;
        if(switchChannel == false){
            lastChannel = currentChannel;
        }  
        currentChannel = 4;
        //switchChannel = true;
    }

    selectorUpdated = true;
}

void patternCopy(byte *pattern2copy){
    for(short i=0;i<4;i++){
        Serial.println(pattern[i]);
        pattern2copy[i] = pattern[i];
        Serial.println(pattern2copy[i]);
    }
}

void storeLastPattern(){
    if(switchChannel == false && currentChannel == 1){
        if(lastPattern == 1){
            patternCopy(pattern1);
        }
        if(lastPattern == 2){
            patternCopy(pattern2);
        }
        if(lastPattern == 3){
            patternCopy(pattern3);
        }
        if(lastPattern == 4){
            patternCopy(pattern4);
        }
        if(lastPattern == 5){
            patternCopy(pattern5);
        }
        if(lastPattern == 6){
            patternCopy(pattern6);
        }
        if(lastPattern == 7){
            patternCopy(pattern7);
        }
        if(lastPattern == 8){
            patternCopy(pattern8);
        }
    }
}

void patternRetrieve(byte *pattern2retrieve){
    for(short i=0;i<4;i++){
        Serial.println(pattern2retrieve[i]);
        pattern[i] = pattern2retrieve[i];
        Serial.println(pattern[i]);
    }
}

void switchDaPattern(){
    if(switchChannel == false && currentChannel == 1){
        if(currentPattern == 1){
            patternRetrieve(pattern1);
            channel1CurrentRunningPattern = 1;
        }
        if(currentPattern == 2){
            patternRetrieve(pattern2);
            channel1CurrentRunningPattern = 2;
        }
        if(currentPattern == 3){
            patternRetrieve(pattern3);
            channel1CurrentRunningPattern = 3;
        }
        if(currentPattern == 4){
            patternRetrieve(pattern4);
            channel1CurrentRunningPattern = 4;
        }
        if(currentPattern == 5){
            patternRetrieve(pattern5);
            channel1CurrentRunningPattern = 5;
        }
        if(currentPattern == 6){
            patternRetrieve(pattern6);
            channel1CurrentRunningPattern = 6;
        }
        if(currentPattern == 7){
            patternRetrieve(pattern7);
            channel1CurrentRunningPattern = 7;
        }
        if(currentPattern == 8){
            patternRetrieve(pattern8);
            channel1CurrentRunningPattern = 8;
        }
    }
}

void loop() {

    unsigned long currentLEDMillis = millis();

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

    uint16_t selectorKeyState = mcp_selector.readGPIOAB();
    //Serial.println(selectorKeyState);
    if(selectorKeyState != max16){
        unsigned long currentSelectorDebounceMillis = millis();
        if(selectorKeyState == lastSelectorKeyState && selectorUpdated == false){
            if(currentSelectorDebounceMillis - previousSelectorDebounceMillis > debounceInterval){
                previousSelectorDebounceMillis = currentSelectorDebounceMillis;
                updateSelector(selectorKeyState);
            }
        } else {
            lastSelectorKeyState = selectorKeyState;
        }
    } else {
        selectorUpdated = false;
    }

    if(step == 15 && switchPattern == true){
        storeLastPattern();
    }

    if(step == 0 && switchPattern == true){
        switchDaPattern();
        switchPattern = false;
    }

    if(currentLEDMillis - previousLEDMillis > clock) {
        previousLEDMillis = currentLEDMillis;
        updateSelectorBuffer();
        updatePatternBuffer();
        cursorGrid();
    }

    writeScreen();
}