#ifndef Pattern_h
#define Pattern_h

#include "Arduino.h"

#define DEFAULT_STEPS 16
#define DEFAULT_ROWS 2
#define DEFAULT_COLUMNS 8
#define TRIGGER_INTERVAL 1

class Pattern{
  public:
    // VARIABLES
    byte sequence_bytes[2];
    bool sequence[16];
    // METHODS
    Pattern();
    byte outputStepByte();
    void init();
    void loadPattern();
    void savePattern();
    void updateSequenceBytes();
    void updateStepPositionBytes();
    void incrementPosition( byte position );
 
  private:
    // VARIABLES   
    byte _position;
    byte _triggered_step = 99;
    long _previous_trigger_millis = 0;
    bool _random;
    // METHODS
    void _resetSequence();
    void _resetSequenceBytes();
    void _randomStep();
    void _init();
};

#endif