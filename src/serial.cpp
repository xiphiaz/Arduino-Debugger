//
//  serial.cpp
//  Arduino Grapher
//
//  Created by Zak Henry on 12/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "serial.h"


Serial::Serial(){ //constructor
    
	cout << "Stepper control class instantiated \n";
    
    stepper0Ready = stepper1Ready = stepper2Ready = true;
    arduinoConnected = false;
    cout <<"Serial constructor complete\n";
    
}

void Serial::setupDevices(){
    
    serialConnection.enumerateDevices();
    printf("enumerated serialConnection devices\n");
    
    if(!serialConnection.setup(/*"tty.usbserial-A9007Mbm"*/1, 9600)){
        printf("Serial setup failed!\n");
    }else{
        arduinoConnected = true;
    }
    
    cout <<"Serial setup complete\n";
}

bool Serial::println(string line){
    
    unsigned char * charLine = new unsigned char[line.size()+1];
    charLine[line.size()]=0;
    memcpy(charLine,line.c_str(),line.size());

    if (arduinoConnected){
        serialConnection.writeBytes(charLine,line.size());
        cout <<"output to serialConnection: "<<charLine<<"\n";
    }

}

void Serial::setStepper(float angle, float speed){
    
    char buffer [50];
    sprintf (buffer, "d%f-s%de", angle+90, (int)speed); //note speed is cast to int for now, will be float eventually. angle has 90 added to it so it can be in the range 0-180
    //    sprintf (buffer, "$"); //note speed is cast to int for now, will be float eventually
    println (buffer);
    
}

void Serial::update(){
    
    read(serialConnection, bytesReturned0, messageBuffer0, message0);
    
}

string Serial::readLine(){
    
    string message = "herp";
    
    read(serialConnection, bytesReturned0, messageBuffer0, message0);
    
    return message0;
}

void Serial::read(ofSerial& serialPort, unsigned char * bytesReturned, string& messageBuffer, string& message){
    
    // if we've got new bytes
    if(serialPort.available() > 0){
        // we wil keep reading until nothing is left
        while (serialPort.available() > 0){
            
            // we'll put the incoming bytes into bytesReturned
            serialPort.readBytes(bytesReturned, NUM_BYTES);
            
            // if we find the splitter we put all the buffered messages 
            // in the final message, stop listening for more data and 
            // notify a possible listener
            // else we just keep filling the buffer with incoming bytes. 
            if(*bytesReturned == '\n'){
                
                message = messageBuffer;
                messageBuffer = "";
                
                messages.push_back(parseMessage(message));
                if (messages.size()>1000){
                    messages.erase(messages.begin());
                }
                
                cout << "messages in memory "<<messages.size()<<endl;
            
            }else{
                if(*bytesReturned != '\r') messageBuffer += *bytesReturned;
            }
//            cout << "  messageBuffer: (" << messageBuffer << ")\n";
        }
        
        // clear the message buffer
        memset(bytesReturned,0,NUM_BYTES);
    }
}

vector<string> Serial::unwrapDelimited(string input, char leftDelimit, char rightDelimit){
    
//    cout << "unwrapDelimited function begins"<<endl;
    
    int beginPos = 0;
    int endPos = 0;
    vector <string> resultVector;
    
    while(1){
//        cout << "about to search the string for left delimit at position ("<<endPos<<")"<<endl;
        beginPos = input.find(leftDelimit, endPos);
        endPos = input.find(rightDelimit, endPos+1);
        
        if (beginPos == string::npos||endPos == string::npos){
            break;
        }
//        cout << "beginPos = "<<beginPos<<" endPos = "<<endPos<<endl;
        int length = (endPos-beginPos)+1;
//        cout << "length: "<<length<<endl;
        string result = input.substr(beginPos+1, length-2);
        
//        cout << "result = "<<result<<endl;
        resultVector.push_back(result);
    }
    
    
    return resultVector;

}


message Serial::parseMessage(string rawMessage){
    
//    cout << "raw message: "<<rawMessage<<endl;
    
    message message;
    vector<string>timestampVector = unwrapDelimited(rawMessage, '[', ']');
    if (timestampVector.size()>0){
        message.timestamp = atoi(timestampVector[0].c_str());
//        cout << "timestamp is: "<<message.timestamp<<endl;
    }
        
    
    vector<string>pinValueVector = unwrapDelimited(rawMessage, '{', '}');
    if (pinValueVector.size()>0){
        for (int i=0; i<pinValueVector.size(); i++){
            pinValue pinValue;
            pinValue.pin = atoi(pinValueVector[i].substr(0, pinValueVector[i].find(':')).c_str());
            pinValue.value = atoi(pinValueVector[i].substr(pinValueVector[i].find(':')+1, pinValueVector[i].size()).c_str());
//            cout << "pin is: "<<pinValue.pin<<", value is: "<<pinValue.value<<endl;
            message.pinValues.push_back(pinValue);
        }
    }
    
    
    return message;
    
}


