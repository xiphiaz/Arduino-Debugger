#include "arduinoGrapher.h"

bool recievingData = false;
int baudRate = 9600;
bool ledOn = false;

//--------------------------------------------------------------
void arduinoGrapher::setup(){
    serial.setupDevices();
    
    serial.println("{pin_on:a0}");
    serial.println("{print:begin}");
    
    ofBackgroundHex(0x191919);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
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
void arduinoGrapher::update(){
//    cout << "new message: "<<serial.readLine()<<endl;
    serial.update();
    
    if (serial.messages.size()>0){
        ofSetWindowTitle("Buffer size: "+ofToString(serial.messages.size())+" first timestamp: "+ofToString(serial.messages[0].timestamp));
    }
    
}

//--------------------------------------------------------------
void arduinoGrapher::draw(){
    

    drawGraph(100, 100, 1000, 800, 30);
    
    
    
}

void arduinoGrapher::exit(){
    
    serial.println("{print:end}");
//    serial.serialConnection.close();
    
}

//--------------------------------------------------------------
void arduinoGrapher::keyPressed(int key){
    
    switch (key) {
        case ' ':
        {
            if(recievingData){
                serial.println("{print:end}");
                recievingData = false;
            }else{
                serial.println("{print:begin}");
                recievingData = true;
            }
//            recievingData = !recievingData;
        }
        break;
            
        case '0':
        {
            serial.println("{pin_on:a0}");
        }
        break;
            
        case '1':
        {
            serial.println("{pin_on:a1}");
        }
        break;
            
        case 'b':
        {
            cout << serial.baudRate<<endl;
            if(recievingData) serial.println("{print:end}"); //stop the flood while changing speeds
            if (serial.baudRate==9600){
                serial.setBaud(115200);
//                serial.setBaud(9600);
            }else{
                serial.setBaud(9600);
            }
            serial.serialConnection.flush(true, true);
            cout << serial.baudRate<<endl;
            if(recievingData) serial.println("{print:begin}");
            
        }
        break;
            
        case 'l':
        {
            if(ledOn){
                serial.println("{led:off}");
                ledOn = false;
            }else{
                serial.println("{led:on}");
                ledOn = true;
            }
        }
        break;
            
        default:
            break;
    }

}

//--------------------------------------------------------------
void arduinoGrapher::keyReleased(int key){

}

//--------------------------------------------------------------
void arduinoGrapher::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void arduinoGrapher::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void arduinoGrapher::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void arduinoGrapher::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void arduinoGrapher::windowResized(int w, int h){

}

//--------------------------------------------------------------
void arduinoGrapher::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void arduinoGrapher::dragEvent(ofDragInfo dragInfo){ 

}

void arduinoGrapher::drawGraph(int posX, int posY, int graphWidth, int height, float graphScale){ //graph scale is the number of seconds represented
    
    float xScaling = ((float)graphWidth/graphScale)/1000000;
    
    int pinId = 0;
    
    if(serial.messages.size()>0){
        
        
        
        vector<ofPoint> graphPoints;
        
//        graphPoints.push_back(ofPoint(0, height));
        
        
        for(int i=0; i<serial.messages.size(); i++){
            
            if (serial.messages[i].pinValues.size()>0){
                //                cout<<"message pin is "<<serial.messages[i].pinValues[0].pin<<endl;
                int timestamp = serial.messages[i].timestamp;

                int timestampOffset = serial.messages[serial.messages.size()-1].timestamp-timestamp;
                
                //                cout << "timestamp: "<<timestamp<<", first: "<<serial.messages[1].timestamp<<", offset: "<<timestampOffset<<endl;
                
                
                
                ofPoint vertex;
                vertex.x = graphWidth-timestampOffset*xScaling; // 1 pixel per ms
                vertex.y = height-((float)serial.messages[i].pinValues[pinId].value/1023*height);
                
                graphPoints.push_back(vertex);
            }
            
        }
        
        //        cout << xScaling<<endl;
        
        //        cout << "rightmost vertex is "<<graphPoints[graphPoints.size()-1].x<<endl;
        
        
        
        
//        graphPoints.push_back(ofPoint(graphWidth, height));
        
        
        
        
        ofTranslate(100, 100);
        
//        ofSetHexColor(0x00ff00);
//        ofRect(0, 0, graphWidth, (float)height/10);
        
        ofSetHexColor(0x0098D4);
//        ofBeginShape();
//            ofVertexes(graphPoints);
//        ofEndShape();
        
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(1.5);
        glBegin(GL_LINE_STRIP);
        
        for (int i=0; i<graphPoints.size(); i++) {
            glVertex2f(graphPoints[i].x, graphPoints[i].y);
        }
        glEnd();
    }
    
}
