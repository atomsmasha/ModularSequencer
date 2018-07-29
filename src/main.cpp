#include <Sequencer.h>
#include <ControlPanel.h>
#include <DisplayPanel.h>
#include "Arduino.h"
#include "SPI.h"
#include "DAC_MCP49xx.h"

#define MATRIX_ADDRESS 1
#define SELECTOR_ADDRESS 0
bool sequence_updated = false;
bool selector_updated = false;
bool update_display = false;
bool triggered = false;
long previousMillis = 0;
byte clock_pin = 3;

Sequencer seq = Sequencer();
ControlPanel matrix_cpanel = ControlPanel();
ControlPanel selector_cpanel = ControlPanel();
DisplayPanel dpanel = DisplayPanel();
DAC_MCP49xx dac_2(DAC_MCP49xx::MCP4902, 10);
DAC_MCP49xx dac_1(DAC_MCP49xx::MCP4902, 9);

void setup(){
    matrix_cpanel.init(MATRIX_ADDRESS);
    selector_cpanel.init(SELECTOR_ADDRESS);
    dpanel.init();
    seq.init();
    seq.setFlashInterval(25);
    dpanel.updateChannel(seq.selector_bytes[0]);
    dpanel.updatePattern(seq.selector_bytes[1]);
}

void clockPulseIn(){
    int analog_clock = analogRead(clock_pin);

    if(analog_clock >= 512){
        if(triggered == false){
            seq.incrementPosition();
            triggered = true;
            return;
        }
    }
    else{
        triggered = false;
    }
}

void flashChannels(){
    byte active_channel = seq.getActiveChannel();
    for(byte i = 0; i < 4; i++){
        if(seq.channel[i].pattern[seq.getActivePattern(i)].sequence[seq.position] == true && i != active_channel){
            seq.setFlashChannel(i, true);
        }
        else{
            seq.setFlashChannel(i, false);
        }
    }
}

void outputToDACs(){
    dac_1.outputA(seq.channel[0].pattern[seq.getActivePattern(0)].outputStepByte());
    dac_1.outputB(seq.channel[1].pattern[seq.getActivePattern(1)].outputStepByte());
    dac_2.outputA(seq.channel[2].pattern[seq.getActivePattern(2)].outputStepByte());
    dac_2.outputB(seq.channel[3].pattern[seq.getActivePattern(3)].outputStepByte());    
}

void updateDisplay(){   
    // maybe always do this stuff
    for(byte i = 0; i < 2; i++){
        dpanel.updateSequence(seq.getSequenceByte(i), i);
    }
    flashChannels();
    seq.flashChannels();
    dpanel.updateChannel(seq.selector_bytes[0]);
    dpanel.updatePattern(seq.selector_bytes[1]);
    // but only do this 1 out of every 10 iterations, and when it fires, skip the update functions above to balance
    // the load on the processor
    dpanel.refreshDisplay();
}

void readMatrixAndUpdateSequence(){
    matrix_cpanel.readControlPanel();
    if(matrix_cpanel.button_registered == true && sequence_updated == false){
        seq.updateActivePattern(matrix_cpanel.button_returned);
        sequence_updated = true;
        return;
    }
    if(matrix_cpanel.button_registered == false){
        sequence_updated = false;
        return;
    }
}

void readSelectorAndUpdate()
{
    selector_cpanel.readControlPanel();
    if(selector_cpanel.button_registered == true && selector_updated == false){
        if(selector_cpanel.button_returned < 8){
            seq.setActivePattern(selector_cpanel.button_returned);
            selector_updated = true;
            return;
        }      
        if(selector_cpanel.button_returned < 12){
            byte selected_channel = selector_cpanel.button_returned - 8;
            seq.setActiveChannel(selected_channel);
            selector_updated = true;
            return;
        }
    }
    if(selector_cpanel.button_registered == false){
        selector_updated = false;
    }
}

void loop(){
    readSelectorAndUpdate();
    readMatrixAndUpdateSequence();
    clockPulseIn();
    outputToDACs();

    // update display once every 10 iterations?
    // - see my comments in the updateDisplay() function
    update_display = !update_display;
    if(update_display){
        updateDisplay();
    }
}

// I guess I can whack some ideas here?
//
// - Editable pattern lengths
// - Pattern light flashes if unsaved changes have been made. Press it to save
// - Maybe a clock out and a global reset?
// - Ability to choose trigger/pulse length