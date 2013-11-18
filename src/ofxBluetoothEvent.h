//
//  File.h
//  SoundDistance
//
//  Created by Todd Vanderlin on 11/15/13.
//
//

#pragma once
#include "ofEvents.h"

class ofxBluetoothEvent : public ofEventArgs {
    
public:
    
    enum {
        DEVICE_CONNECTED,
        DEVICE_DISCONNECTED,
    };
    static string eventToString(int e);
    int type;
    
};
