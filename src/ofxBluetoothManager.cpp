//
//  ofxBluetoothDevice.cpp
//  SoundDistance
//
//  Created by Todd Vanderlin on 11/15/13.
//
//

#include "ofxBluetoothManager.h"

#ifdef TARGET_OSX
#import <Foundation/Foundation.h>
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <IOBluetooth/objc/IOBluetoothDeviceInquiry.h>
#import <IOBluetooth/objc/IOBluetoothDevice.h>
#import <IOBluetooth/objc/IOBluetoothHostController.h>
#import <IOBluetooth/IOBluetoothUtilities.h>

@interface BTController : NSObject {
}
-(void)connecToDevice:(NSString*)addrStr;
-(void)disconnectFromDevice:(NSString*)addrStr;
-(void)disconnectFromAllDevices;

@end

@implementation BTController

//--------------------------------------------------------------
-(id)init {
    self = [super init];
    if(self) {
        [self disconnectFromAllDevices];
        [IOBluetoothDevice registerForConnectNotifications:self selector:@selector(deviceIsConnected:fromDevice:)];
    }
    return self;
}

//--------------------------------------------------------------
-(void)dealloc {
    [super dealloc];
    [self disconnectFromAllDevices];
}

//--------------------------------------------------------------
-(void)disconnectFromAllDevices {
    NSArray * devices = [IOBluetoothDevice pairedDevices];
    IOBluetoothDevice * device;
    NSEnumerator * e = [devices objectEnumerator];
    while (device = [e nextObject]) {
        if([device isConnected]) {
            [device closeConnection];
        }
    }
}

//--------------------------------------------------------------
-(void)connecToDevice:(NSString*)addrStr {
    
    BluetoothDeviceAddress addr;
    IOBluetoothNSStringToDeviceAddress(addrStr, &addr);
    
    IOBluetoothDevice * device = [[IOBluetoothDevice alloc] init];
    device = [IOBluetoothDevice deviceWithAddress:&addr];
    [device openConnection];
    
    
}

//--------------------------------------------------------------
-(void)disconnectFromDevice:(NSString*)addrStr {
    
    NSArray * devices = [IOBluetoothDevice pairedDevices];
    IOBluetoothDevice * device;
    NSEnumerator * e = [devices objectEnumerator];
    while (device = [e nextObject]) {
        if([device isConnected] && [addrStr isEqualToString:[device addressString]]) {
            [device closeConnection];
        }
    }

}

//--------------------------------------------------------------
-(void)deviceDidDisconnected:(IOBluetoothUserNotification*)notification fromDevice:(IOBluetoothDevice*)device {
    NSLog(@"deviceIsConnected %@ %i", notification, [device isConnected]);
    NSLog(@"RSSI %i", [device RSSI]);
    
    static ofxBluetoothEvent evt;
    evt.type = ofxBluetoothEvent::DEVICE_DISCONNECTED;
    ofNotifyEvent(ofxBluetoothManager::events, evt);
}

//--------------------------------------------------------------
-(void)deviceIsConnected:(IOBluetoothUserNotification*)notification fromDevice:(IOBluetoothDevice*)device {
    NSLog(@"deviceIsConnected %@ %i", notification, [device isConnected]);
    NSLog(@"RSSI %i", [device RSSI]);
    
    [device registerForDisconnectNotification:self selector:@selector(deviceDidDisconnected:fromDevice:)];

    
    static ofxBluetoothEvent evt;
    evt.type = ofxBluetoothEvent::DEVICE_CONNECTED;
    ofNotifyEvent(ofxBluetoothManager::events, evt);
}
@end
#endif









//--------------------------------------------------------------

BTController * btController = NULL;
ofEvent <ofxBluetoothEvent>  ofxBluetoothManager::events;

//--------------------------------------------------------------
BTController * getBTInstance() {
    if(btController == NULL) {
        btController = [[BTController alloc] init];
    }
    return btController;
}

//--------------------------------------------------------------
ofxBluetoothManager::ofxBluetoothManager() {
    
}
ofxBluetoothManager::~ofxBluetoothManager() {
    
}

//--------------------------------------------------------------
ofxBluetoothDevice::ofxBluetoothDevice() {
    name = address = "";
    isConnected=false;
}
ofxBluetoothDevice::ofxBluetoothDevice(string n, string addr) {
    name = n; address = addr;
    isConnected=false;
}
int ofxBluetoothDevice::getRSSI() {
#ifdef TARGET_OSX
    NSArray * devices = [IOBluetoothDevice pairedDevices];
    IOBluetoothDevice * device;
    NSEnumerator * e = [devices objectEnumerator];
    while (device = [e nextObject]) {
        if([device isConnected]) {
            string a = [[device addressString] UTF8String];
            if(a == address) {
                return (int)[device RSSI];
            }
        }
    }
    return 0;
#endif
}
int ofxBluetoothDevice::getRawRSSI() {
#ifdef TARGET_OSX
    NSArray * devices = [IOBluetoothDevice pairedDevices];
    IOBluetoothDevice * device;
    NSEnumerator * e = [devices objectEnumerator];
    while (device = [e nextObject]) {
        if([device isConnected]) {
            string a = [[device addressString] UTF8String];
            if(a == address) {
                return (int)[device rawRSSI];
            }
        }
    }
    return 0;
#endif
}


//--------------------------------------------------------------
vector <ofxBluetoothDevice> ofxBluetoothManager::listPairedDevices() {
    vector<ofxBluetoothDevice>connectedDevices;
#ifdef TARGET_OSX
        NSArray * devices = [IOBluetoothDevice pairedDevices];
        if(devices) {
            IOBluetoothDevice * device;
            NSEnumerator * e = [devices objectEnumerator];
            while (device = [e nextObject]) {
                if(device) {
                    string n = [device name]==nil?"":[[device name] UTF8String];
                    string a = [device addressString]==nil?"":[[device addressString] UTF8String];
                    connectedDevices.push_back(ofxBluetoothDevice(n, a));
                }
            }
        }
#endif
#ifdef TARGET_WIN32
    /* This is a WIP */
    HBLUETOOTH_DEVICE_FIND founded_device;
    
    BLUETOOTH_DEVICE_INFO device_info;
    device_info.dwSize = sizeof(device_info);
    
    BLUETOOTH_DEVICE_SEARCH_PARAMS search_criteria;
    search_criteria.dwSize = sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS);
    search_criteria.fReturnAuthenticated = TRUE;
    search_criteria.fReturnRemembered = FALSE;
    search_criteria.fReturnConnected = FALSE;
    search_criteria.fReturnUnknown = FALSE;
    search_criteria.fIssueInquiry = FALSE;
    search_criteria.cTimeoutMultiplier = 0;
    
    founded_device = BluetoothFindFirstDevice(&search_criteria, &device_info);
    
    if (founded_device == NULL) {
        printf(TEXT("Error: \n%s\n"), getErrorMessage(WSAGetLastError(), error));
        //return -1;
    }
    
    do {
        string n = device_info.szName;
        string a = device_info.Address;
        connectedDevices.push_back(ofxBluetoothDevice(n, a));
        printf("founded device: %s\n", device_info.szName);
        
    } while (BluetoothFindNextDevice(founded_device, &device_info));
#endif

#ifdef TARGET_LINUX
#endif
    return connectedDevices;
}

//--------------------------------------------------------------
vector <ofxBluetoothDevice> ofxBluetoothManager::getConnectedDevices() {
    vector<ofxBluetoothDevice>connectedDevices;
#ifdef TARGET_OSX
    NSArray * devices = [IOBluetoothDevice pairedDevices];
    IOBluetoothDevice * device;
    NSEnumerator * e = [devices objectEnumerator];
    while (device = [e nextObject]) {
        if(device) {
            if([device isConnected]) {
                string n = [device name]==nil?"":[[device name] UTF8String];
                string a = [device addressString]==nil?"":[[device addressString] UTF8String];
                connectedDevices.push_back(ofxBluetoothDevice(n, a));
                connectedDevices.back().isConnected = [device isConnected];
            }
        }
    }
#endif
    return connectedDevices;
}


//--------------------------------------------------------------
void ofxBluetoothManager::connecToDevice(string deviceAddress) {
#ifdef TARGET_OSX
    [getBTInstance() connecToDevice:[NSString stringWithUTF8String:deviceAddress.c_str()]];
#endif
}

//--------------------------------------------------------------
void ofxBluetoothManager::disconnectFromDevice(string deviceAddress) {
#ifdef TARGET_OSX
    [getBTInstance() disconnectFromDevice:[NSString stringWithUTF8String:deviceAddress.c_str()]];
#endif
}






















