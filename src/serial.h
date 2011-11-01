//
//  serial.h
//  Arduino Grapher
//
//  Created by Zak Henry on 12/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef _STEPPER_CONTROL // if this class hasn't been defined, the program can define it
#define _STEPPER_CONTROL // by using this if statement you prevent the class to be called more 
// than once which would confuse the compiler

#include "ofMain.h"

#define NUM_BYTES 1

typedef struct pinValue {
    int pin;
    int value;
}pinValue;

typedef struct message {
    int timestamp;
    vector<pinValue>pinValues;
}message;



class Serial {
    
    vector<char>buffer;
    
    bool stepper0Ready, stepper1Ready, stepper2Ready;
    bool arduinoConnected;
    
public:
    
    Serial(); //constructor
    void setupDevices();
    
    ofSerial serialConnection;
    
    int challengeArduinoForBaud();
    
    int baudRates [11];
    
    int baudRate;
    void setBaud(int rate);
    bool serialLocked;
    
    void print(ofSerial& serialPort, string line);
    void println(string line);
    
    void update();
        
    vector<message>messages;
    
    void emptyBuffer();
    string readln(ofSerial& serialPort, unsigned char * bytesReturned, string& messageBuffer); //wait for a line to come through
    
    string message0, message1, message2, messageBuffer0, messageBuffer1, messageBuffer2;
    unsigned char bytesReturned0[1], bytesReturned1[1], bytesReturned2[1];
    void read(ofSerial& serialPort, unsigned char * bytesReturned, string& messageBuffer, string& message);
    
    message parseMessage(string rawMessage);
    vector<string> unwrapDelimited(string input, char leftDelimit, char rightDelimit);
    
};


#endif 