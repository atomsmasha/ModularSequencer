#include <Sequencer.h>
#include <ControlPanel.h>
#include <DisplayPanel.h>
#include "Arduino.h"

#define MATRIX_ADDRESS 1
#define SELECTOR_ADDRESS 0
bool sequence_updated = false;
bool selector_updated = false;
long clock = 100;
long previousMillis = 0;

Sequencer seq = Sequencer();
ControlPanel matrix_cpanel = ControlPanel();
ControlPanel selector_cpanel = ControlPanel();
DisplayPanel dpanel = DisplayPanel();

void setup()
{
    matrix_cpanel.init(MATRIX_ADDRESS);
    selector_cpanel.init(SELECTOR_ADDRESS);
    dpanel.init();
    seq.init();
    seq.setFlashInterval(clock/2);
    dpanel.updateChannel(seq.selector_bytes[0]);
    dpanel.updatePattern(seq.selector_bytes[1]);
}

void flashChannels()
{
    byte active_channel = seq.getActiveChannel();
    for(byte i = 0; i < 4; i++)
    {
        if(seq.channel[i].pattern[seq.getActivePattern(i)].sequence[seq.position] == true && i != active_channel)
        {
            seq.setFlashChannel(i, true);
        }
        else
        {
            seq.setFlashChannel(i, false);
        }
    }
}

void updateDisplay()
{   
    for(byte i = 0; i < 2; i++)
    {
        dpanel.updateSequence(seq.getSequenceByte(i), i);
    }
    flashChannels();
    seq.flashChannels();
    dpanel.updateChannel(seq.selector_bytes[0]);
    dpanel.updatePattern(seq.selector_bytes[1]);
    dpanel.refreshDisplay();
}

void readMatrixAndUpdateSequence()
{
    matrix_cpanel.readControlPanel();
    if(matrix_cpanel.button_registered == true && sequence_updated == false)
    {
        seq.updateActivePattern(matrix_cpanel.button_returned);
        sequence_updated = true;
        return;
    }
    if(matrix_cpanel.button_registered == false)
    {
        sequence_updated = false;
        return;
    }
}

void incrementCursorAndUpdateSequence()
{
    long currentMillis = millis();
    if(currentMillis - previousMillis >= clock)
    {
        seq.incrementPosition();
        previousMillis = currentMillis;
    }
}

void readSelectorAndUpdate()
{
    selector_cpanel.readControlPanel();
    if(selector_cpanel.button_registered == true && selector_updated == false)
    {
        if(selector_cpanel.button_returned < 8)
        {
            seq.setActivePattern(selector_cpanel.button_returned);
            selector_updated = true;
            return;
        }      
        if(selector_cpanel.button_returned < 12)
        {
            byte selected_channel = selector_cpanel.button_returned - 8;
            seq.setActiveChannel(selected_channel);
            selector_updated = true;
            return;
        }
    }
    if(selector_cpanel.button_registered == false)
    {
        selector_updated = false;
    }
}

void loop()
{
    readSelectorAndUpdate();
    readMatrixAndUpdateSequence();
    incrementCursorAndUpdateSequence();
    updateDisplay();
}
