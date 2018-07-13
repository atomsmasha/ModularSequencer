#include <DisplayPanel.h>
#include "Wire.h"
#include "LedControl.h"

DisplayPanel::DisplayPanel()
{

}

void DisplayPanel::init()
{
    _init();
}

void DisplayPanel::updateSequence( byte sequence_byte, byte index )
{
    _writeDisplayBuffer(sequence_byte, index+2);
}

void DisplayPanel::updatePattern( byte pattern_number )
{
    _writeDisplayBuffer(pattern_number, 1);
}

void DisplayPanel::updateChannel( byte channel_number )
{
    _writeDisplayBuffer(channel_number, 0);
}

void DisplayPanel::refreshDisplay()
{
    _writeDisplayPanel();
}

///
/// PRIVATE METHODS
///

void DisplayPanel::_init()
{
    lc.shutdown(0,false);  // Wake up displays
    lc.shutdown(1,false);
    lc.setIntensity(0,5);  // Set intensity levels
    lc.setIntensity(1,5);
    lc.clearDisplay(0);  // Clear Displays
    lc.clearDisplay(1);

    for(byte i = 0; i < 8; i++)
    {
        lc.setRow(0,i,_display_buffer[i]);
    }
}

void DisplayPanel::_writeDisplayBuffer( byte buffer_line, byte index )
{
    _display_buffer[index] = buffer_line;
}

void DisplayPanel::_writeDisplayPanel()
{
    for(byte i = 0; i < 8; i++)
    {
        lc.setRow(0,i,_display_buffer[i]);
    }
}