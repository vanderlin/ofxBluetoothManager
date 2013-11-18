//
//  File.cpp
//  SoundDistance
//
//  Created by Todd Vanderlin on 11/15/13.
//
//

#include "ofxBluetoothEvent.h"

string ofxBluetoothEvent::eventToString(int e) {
    switch (e) {
        case DEVICE_CONNECTED:
            return "DEVICE_CONNECTED";
        case DEVICE_DISCONNECTED:
            return "DEVICE_DISCONNECTED";
    };
    return "UNKNOWN";
}