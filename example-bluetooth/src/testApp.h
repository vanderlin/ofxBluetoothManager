#pragma once

#include "ofMain.h"
#include "ofxBluetoothManager.h"
#include "ofxGui.h"


class RectButton : public ofRectangle {
public:
    string name;
    string address;
    bool isConnected;
    int RSSI; int rawRSSI;
    RectButton() {
        isConnected = false;
        RSSI = rawRSSI = 0;
    }
    void draw() {
    
        isConnected ? ofSetColor(10, 200, 100) : ofSetColor(255);
        ofFill();
        ofRect(*this);
        if(inside(ofGetMouseX(), ofGetMouseY())) {
            
            isConnected?ofSetColor(255, 0, 0, 200):ofSetColor(255, 255, 0, 100);
            ofFill();
            ofRect(*this);
            
            ofSetColor(90);
            ofDrawBitmapString(isConnected?"Disconnect":"Connect", width+30, (y+height/2) + 3);
            
        }
        
        ofSetColor(90);
        ofNoFill();
        ofRect(*this);
        ofDrawBitmapString(name+" : "+address+" RSSI: "+ofToString(RSSI)+":"+ofToString(rawRSSI), x+5, (y+height/2) + 3);
        
    }
};


//--------------------------------------------------------------
class testApp : public ofBaseApp {

	public:
    
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    void newBluetoothEvent(ofxBluetoothEvent &e);
    vector <RectButton> buttons;
   
    float            value, lastTime;
    int              lastRSSI;
    int              lastConnectedDevice;
    ofxPanel         gui;
    ofxIntSlider     minSlider;
    ofxIntSlider     maxSlider;
    ofxToggle        useRawRSSI;
};
