#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    serial.setupDevices();
    
//    cout << "testing string delimit function"<<endl;
//    
//    string example = "ewf[3489923]fkjewfw{12:34}{56:78}";
//    vector<string>result;
//    result = serial.unwrapDelimited(example, '{', '}');
//    
//    message message = serial.parseMessage(example);
//    
//    cout << "pin 0 is id: "<<message.pinValues[0].pin<<", value: "<<message.pinValues[0].value<<endl;
    
}

//--------------------------------------------------------------
void testApp::update(){
//    cout << "new message: "<<serial.readLine()<<endl;
    serial.update();
}

//--------------------------------------------------------------
void testApp::draw(){

    if(serial.messages.size()>0){
        vector<ofPoint> graphPoints;
        
        for(int i=0; i<serial.messages.size(); i++){
            
            if (serial.messages[i].pinValues.size()>0){
                cout<<"message pin is "<<serial.messages[i].pinValues[0].pin<<endl;
                
                ofPoint vertex;
                vertex.x = i;
                vertex.y = serial.messages[i].pinValues[0].value;
                
                graphPoints.push_back(vertex);
            }
            
            
        }
        
        
        ofSetHexColor(0xff0000);
        ofTranslate(100, 100);
        
        ofBeginShape();
        ofVertexes(graphPoints);
        ofEndShape();
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