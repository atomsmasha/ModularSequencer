#include <Pattern.h>
#include "Arduino.h"

Pattern::Pattern()
{

}

void Pattern::init()
{
    _init();
}

void Pattern::loadPattern()
{
    // code goes here
}

void Pattern::savePattern()
{
    // code goes here
}

void Pattern::incrementPosition( byte position )
{
    if(_random == true)
    {
        _randomStep();
        return;
    }

    _position = position;
}

void Pattern::updateSequenceBytes()
{
    // zero the buffer
    _resetSequenceBytes();
    byte step_bit = 0;
    for(byte row = 0; row < 2; row++)
    {
        for(byte column = 0; column < 8; column++)
        {
            if(sequence[step_bit] == true)
            {
                bitWrite(sequence_bytes[row], 7 - column, 1);
            }
            step_bit++;
        }
    }
}

void Pattern::updateStepPositionBytes()
{
    if(_position < 8)
    {
        sequence_bytes[0] ^= bit(7 - _position);
        return;
    }

    if(_position < 16)
    {
        sequence_bytes[1] ^= bit(7-(_position-8));
        return;
    }
}

byte Pattern::outputStepByte()
{
    // code to output to DAC will go here after reading up on the spec
}

//
// PRIVATE METHODS
//

void Pattern::_resetSequence()
{
    for(byte i = 0; i < DEFAULT_STEPS; i++)
    {
        sequence[i] = false;
    }
    return;
}

void Pattern::_resetSequenceBytes()
{
    for(byte i = 0; i < DEFAULT_ROWS; i++)
    {
        sequence_bytes[i] = B00000000;
    }
    return;
}

void Pattern::_randomStep()
{
    _position = random(15);
    return;
}

void Pattern::_init()
{
    _position = 0;
    _random = false;

  // set up default steps
  _resetSequence();
}