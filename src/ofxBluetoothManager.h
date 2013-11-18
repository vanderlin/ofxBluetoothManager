//
//  ofxBlueToothDevice.h
//  SoundDistance
//
//  Created by Todd Vanderlin on 11/15/13.
//
//

#pragma once
#include "ofMain.h"
#include "ofxbluetoothEvent.h"

class ofxBluetoothDevice {
public:
    string name;
    string address;
    bool isConnected;
    ofxBluetoothDevice();
    ofxBluetoothDevice(string n, string addr);
    int getRSSI();
    int getRawRSSI();
};



class ofxBluetoothManager {
    
private:

public:
    
    ofxBluetoothManager();
    ~ofxBluetoothManager();
    static vector <ofxBluetoothDevice> listPairedDevices();
    static vector <ofxBluetoothDevice> getConnectedDevices();
    static void connecToDevice(string deviceAddress);
    static void disconnectFromDevice(string deviceAddress);
    
    void bluetoothEvent();
    void newBluetoothEvent();
    
    static ofEvent <ofxBluetoothEvent> events;

};