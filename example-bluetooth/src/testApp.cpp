#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

    ofDisableAntiAliasing();
    ofSetWindowTitle("OF Bluetooth Manager");
    
    lastConnectedDevice = -1;
    vector <ofxBluetoothDevice> devices = ofxBluetoothManager::listPairedDevices();
    for (int i=0; i<devices.size(); i++) {
        RectButton btn;
        btn.set(20, 20+(i*32), 400, 30);
        btn.name = devices[i].name;
        btn.address = devices[i].address;
        buttons.push_back(btn);
    }
    
    ofAddListener(ofxBluetoothManager::events, this, &testApp::newBluetoothEvent);
  
    
    
    // a gui
    gui.setup();
    gui.setPosition(430, 20);
    gui.add(minSlider.setup("Min", -20, -127, 1));
    gui.add(maxSlider.setup("Max", 3, -127, 1));
    gui.add(useRawRSSI.setup("Use Raw RSSI", false));
    
    vector <ofxBluetoothDevice> connectedDevices = ofxBluetoothManager::getConnectedDevices();
    for(int i=0; i<connectedDevices.size(); i++) {
        if(connectedDevices[i].isConnected) {
            lastConnectedDevice = i;
            break;
        }
    }
    
    lastTime = 0;
    lastRSSI = 0;
    
}

//--------------------------------------------------------------
void testApp::newBluetoothEvent(ofxBluetoothEvent &e) {
    printf("BT Event: %s : %i\n", ofxBluetoothEvent::eventToString(e.type).c_str(), e.type);
}

//--------------------------------------------------------------
void testApp::update(){

    if(ofGetElapsedTimef() - lastTime > 0.2) {
        lastTime = ofGetElapsedTimef();
        
        vector <ofxBluetoothDevice> devices = ofxBluetoothManager::getConnectedDevices();
        
        // update the rec buttons with a new RSSI
        for(int i=0; i<devices.size(); i++) {
            
            for(int j=0; j<buttons.size(); j++) {
                if(devices[i].name == buttons[j].name && devices[i].isConnected) {
                    buttons[j].isConnected = devices[i].isConnected;
                    buttons[j].RSSI = devices[i].getRSSI();
                    buttons[j].rawRSSI = devices[i].getRawRSSI();
                }
            }
            
        }
        
    }
}

//--------------------------------------------------------------
void testApp::draw(){

    for (int i=0; i<buttons.size(); i++) {
        buttons[i].draw();
    }
    
    gui.draw();
    
    
    if(lastConnectedDevice != -1) {
        float rssi = useRawRSSI ? buttons[lastConnectedDevice].rawRSSI : buttons[lastConnectedDevice].RSSI;
        float rad = ofMap(rssi, minSlider, maxSlider, 10, 100);
        value += (rad - value) * 0.2;
        
        ofFill();
        ofSetColor(255, 0, 255);
        ofCircle(ofGetWidth()/2, 300, value);
        
        ofSetColor(0);
        ofDrawBitmapString(buttons[lastConnectedDevice].name, (ofGetWidth()-buttons[lastConnectedDevice].name.length()*8)/2, 300 );
    }
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    int idx = -1;
    for (int i=0; i<buttons.size(); i++) {
        if(buttons[i].inside(x, y)) {
            idx = i;
        }
    }
    if(idx!=-1) {
        if(!buttons[idx].isConnected) {
            ofxBluetoothManager::connecToDevice(buttons[idx].address);
            buttons[idx].isConnected = true;
            lastConnectedDevice = idx;
        }
        else {
            ofxBluetoothManager::disconnectFromDevice(buttons[idx].address);
            buttons[idx].isConnected = false;
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
