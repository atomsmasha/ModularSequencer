#include <Sequencer.h>
#include <Channel.h>
#include "Arduino.h"

Sequencer::Sequencer(){
}

void Sequencer::init(){
    _init();
}

byte Sequencer::getActivePattern( byte channel_selection ){
    byte active_pattern = this->channel[channel_selection].getActivePattern();
    return active_pattern;
}

void Sequencer::setActivePattern( byte pattern_selection ){
    _active_pattern = pattern_selection;
    this ->channel[_active_channel].setActivePattern(_active_pattern);
    _updatePatternByte();
}

void Sequencer::updateActivePattern( byte step ){
    _active_pattern = getActivePattern(_active_channel);
    this->channel[_active_channel].pattern[_active_pattern].sequence[step] = !this->channel[_active_channel].pattern[_active_pattern].sequence[step];
    this->channel[_active_channel].pattern[_active_pattern].updateSequenceBytes();
}

void Sequencer::setActiveChannel( byte channel_selection ){
    _active_channel = channel_selection;
    _active_pattern = getActivePattern(_active_channel);
    _updateChannelByte();
    flashChannels();
    _updatePatternByte();
    return;
}

byte Sequencer::getActiveChannel(){
    return _active_channel;
}

byte Sequencer::getSequenceByte( byte index ){
    _active_pattern = getActivePattern(_active_channel);
    return this->channel[_active_channel].pattern[_active_pattern].sequence_bytes[index];
}

void Sequencer::incrementPosition(){
    if(position < 15){
        position++;
        _position_incremented = true;
    }
    else{
        position = 0;
        _position_incremented = true;
    }
    
    for(byte i = 0; i < DEFAULT_CHANNELS; i++){
        byte active_pattern = getActivePattern(i);
        this->channel[i].pattern[active_pattern].incrementPosition(position);
        this->channel[i].pattern[active_pattern].updateSequenceBytes();
        this->channel[i].pattern[active_pattern].updateStepPositionBytes();
    }
}

void Sequencer::setFlashInterval( byte interval ){
    _flash_interval = interval;
}

void Sequencer::setFlashChannel( byte index, bool state ){
    _flash_channel[index] = state;
}

void Sequencer::flashChannels(){
    long flash_millis = millis();
    if(_position_incremented == true && flash_millis - _flash_previous_millis <= _flash_interval){
        for(byte i = 0; i < 4; i++){
            if(_flash_channel[i] == true){
                bitWrite(selector_bytes[0], 7 - i, 1);
                // I could also put code for triggering the DACs in here
            }
        }
        return;
    }
    else{
        _flash_previous_millis = flash_millis;
        _position_incremented = false;
        _updateChannelByte();
        return;
    }
}

//
// PRIVATE METHODS
//

void Sequencer::_resetSelectorBytes( byte index ){
    selector_bytes[index] = B00000000;
}

void Sequencer::_updateChannelByte(){
    // zero the buffer
    _resetSelectorBytes(0);
    bitWrite(selector_bytes[0], 7 - _active_channel, 1);
}

void Sequencer::_updatePatternByte(){
    // zero the buffer
    _resetSelectorBytes(1);
    bitWrite(selector_bytes[1], 7 - _active_pattern, 1);
}

void Sequencer::_init(){
    channel = new Channel[DEFAULT_CHANNELS];
    for(byte i = 0; i < DEFAULT_CHANNELS; i++){
        channel[i].init();
        _flash_channel[i] = false;
    }
    _active_channel = 0;
    _active_pattern = 0;
    _flash_interval = DEFAULT_FLASH_INTERVAL;
    _updateChannelByte();
    _updatePatternByte();
}