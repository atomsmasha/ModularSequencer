#ifndef Channel_h
#define Channel_h

#include <Pattern.h>
#include "Arduino.h"

#define DEFAULT_PATTERNS 8

class Channel
{
  public:
    // VARIABLES
    Pattern* pattern;
    byte pattern_bytes[1];
    // METHODS
    Channel();
    void init();
    void loadChannel();
    void saveChannel();
    void setActivePattern( byte pattern_selection );
    byte getActivePattern();
    void updatePatternBytes();
  
  private:
    // VARIABLES
    byte _active_pattern;
    // METHODS
    void _resetPatternBytes();
    void _init();
};

#endif
