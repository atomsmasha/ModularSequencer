#ifndef Sequencer_h
#define Sequencer_h

#include <Channel.h>
#include "Arduino.h"

#define DEFAULT_CHANNELS 4
#define DEFAULT_FLASH_INTERVAL 50

class Sequencer{
  public:
    // VARIABLES
    Channel* channel;
    byte selector_bytes[2];
    byte position = 0;
    // METHODS
    Sequencer();
    void init();   
    void loadSequencer();
    void saveSequencer();
    byte getActivePattern( byte channel_selection );
    void setActivePattern( byte pattern_selection );
    void updateActivePattern( byte step );
    void setActiveChannel( byte channel_selection );
    byte getActiveChannel();
    byte getSequenceByte( byte index );
    void incrementPosition();
    void setFlashInterval( byte interval );
    void setFlashChannel( byte index, bool state );
    void flashChannels();

  private:
    // VARIABLES
    byte _active_channel;
    byte _active_pattern;
    bool _position_incremented = false;
    bool _flash_channel[4];
    long _flash_previous_millis = 0;
    byte _flash_interval;
    // METHODS
    void _resetSelectorBytes( byte index );
    void _updateChannelByte();
    void _updatePatternByte();
    void _init();
    
};

#endif