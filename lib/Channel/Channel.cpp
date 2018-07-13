#include <Channel.h>
#include <Pattern.h>
#include "Arduino.h"

Channel::Channel()
{

}

void Channel::init()
{
    _init();
}

void Channel::loadChannel()
{
    // code goes here
}

void Channel::saveChannel()
{
    // code goes here
}

void Channel::setActivePattern( byte pattern_selection )
{
    _active_pattern = pattern_selection;
    return;
}

byte Channel::getActivePattern()
{
    return _active_pattern;
}

void Channel::updatePatternBytes()
{
    // zero the buffer
    _resetPatternBytes();
    bitWrite(pattern_bytes[0], 7 - this->getActivePattern(), 1);
}

//
// PRIVATE METHODS
//

void Channel::_resetPatternBytes()
{
    pattern_bytes[0] = B00000000;
}

void Channel::_init()
{
    pattern = new Pattern[DEFAULT_PATTERNS];
    for(byte i = 0; i < DEFAULT_PATTERNS; i++)
    {
        pattern[i].init();
    }
    pattern_bytes[0] = B10000000;
    _active_pattern = 0;
}
