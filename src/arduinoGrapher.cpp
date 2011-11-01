#include "arduinoGrapher.h"

bool recievingData = false;
int baudRate = 9600;
bool ledOn = false;
int blue = 0x0098D4;
int darkBlue = 0x002f3a;
int green = 0x01BB00;
int lightGrey = 0xDEE8E9;
int orange = 0xFE4F00;
int darkGrey = 0xC3C9C9;
int black = 0x000000;
int white = 0xFFFFFF;
int offBlack = 0x191919;

bool pinStatus [20] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, };


struct button {
    int x, y, w, h;
    bool on, hover;
    string name;
};

vector<button> buttons;

void setupGui(){
    for (int i=0; i<sizeof(pinStatus); i++){
        button newButton;
        
        newButton.x = 30*i+10;
        newButton.y = 500;
        newButton.w = 20;
        newButton.h = 20;
        
        if(i>13){
            newButton.name = "a"+ofToString(i-14);
        }else{
            newButton.name = ofToString(i);
        }
        
        
        newButton.on = pinStatus[i];
        newButton.hover = false;
        
        buttons.push_back(newButton);
        
    }
}

void drawButtons(){
    
    for (int i=0; i<buttons.size(); i++){
        
        if(i>13){
            ofSetHexColor(orange);
        }else{
            ofSetHexColor(green);
        }
        
        ofRect(buttons[i].x, buttons[i].y, buttons[i].w, buttons[i].h);
        
//        cout << buttons[i].x<<endl;
        
        if (!buttons[i].on){
            ofSetHexColor(offBlack);
            ofRect(buttons[i].x+2, buttons[i].y+2, buttons[i].w-4, buttons[i].h-4);
        }
        
        if (buttons[i].hover){
            ofSetHexColor(white);
            ofCircle(buttons[i].x+(buttons[i].w/2), buttons[i].y+buttons[i].h+5, 5);
            
        }
        
    }
    
}

void arduinoGrapher::toggleButton(int i){
    
    buttons[i].on = !buttons[i].on;
    
    
    if(pinStatus[i]){ //if false, turn on the pin
        serial.println("{pin_off:"+buttons[i].name+"}");
    }else{
        serial.println("{pin_on:"+buttons[i].name+"}");
    }
    
    
    pinStatus[i] = !pinStatus[i];
    
}

void drawGui(){
    
//    ofTranslate(100, 500);
    
    drawButtons();
    
    
    
}

void arduinoGrapher::handleGui(){
    
    for (int i=0; i<buttons.size(); i++){
        
        buttons[i].hover = false;
        
        if(mouseX>=buttons[i].x&&mouseX<=buttons[i].x+buttons[i].w){
            if(mouseY>=buttons[i].y&&mouseY<=buttons[i].y+buttons[i].h){
                
                buttons[i].hover = true;
                
//                cout << "mouseover!"<<endl;
                
            }
            
        }
    }
    
    
}



//--------------------------------------------------------------
void arduinoGrapher::setup(){
    
    setupGui();
    setupGraphs();
    
    serial.setupDevices();
    
    serial.println("{pin_on:a0}");
    serial.println("{print:begin}");
    
    ofBackgroundHex(offBlack);
    
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
    
    handleGui();
    
//    cout << "running"<<endl;
    
}

//--------------------------------------------------------------
void arduinoGrapher::draw(){
    

    drawGraph(100, 100, 1000, 800, 30);
    drawGui();
    
    ofSetHexColor(orange);
    ofCircle(mouseX, mouseY, 10);
    
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
    cout << "key released"<<endl;
}

//--------------------------------------------------------------
void arduinoGrapher::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void arduinoGrapher::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void arduinoGrapher::mousePressed(int x, int y, int button){
    
    cout << "pressed at "<<x<<", "<<y<<""<<endl;
    
    for (int i=0; i<buttons.size(); i++){
        
        if(mouseX>=buttons[i].x&&mouseX<=buttons[i].x+buttons[i].w){
            if(mouseY>=buttons[i].y&&mouseY<=buttons[i].y+buttons[i].h){
                
                toggleButton(i);
                
            }
            
        }
    }

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

void arduinoGrapher::setupGraphs(){
    
    for (int i=0; i<20; i++){
        
        graph newGraph;
        //setup colours
        newGraph.points.clear();
        graphs.push_back(newGraph);
    }
    
}


void arduinoGrapher::drawGraph(int posX, int posY, int graphWidth, int height, float graphScale){ //graph scale is the number of seconds represented
    
    float xScaling = ((float)graphWidth/graphScale)/1000000;
    
    int pinId = 0;
    
    vector<ofPoint>graphPoints;
    
    
    if(serial.messages.size()>0){
        
//        graphPoints.push_back(ofPoint(0, height));
        
        
        for(int i=0; i<serial.messages.size(); i++){
            
            if (serial.messages[i].pinValues.size()>0){
                //                cout<<"message pin is "<<serial.messages[i].pinValues[0].pin<<endl;
                int timestamp = serial.messages[i].timestamp;

                int timestampOffset = serial.messages[serial.messages.size()-1].timestamp-timestamp;
                
                //                cout << "timestamp: "<<timestamp<<", first: "<<serial.messages[1].timestamp<<", offset: "<<timestampOffset<<endl;
                
                for (int j=0; j<serial.messages[i].pinValues.size();j++){
                    graphs[i].points.clear();
                    
                    
                    ofPoint vertex;
                    vertex.x = graphWidth-timestampOffset*xScaling; // 1 pixel per ms
                    vertex.y = height-((float)serial.messages[i].pinValues[j].value/1023*height);
//                    
//                    
                    graphs[serial.messages[i].pinValues[j].pin].points.push_back(vertex);
                    
//                    if (j==0){
//                        ofPoint vertex;
//                        vertex.x = graphWidth-timestampOffset*xScaling; // 1 pixel per ms
//                        vertex.y = height-((float)serial.messages[i].pinValues[j].value/1023*height);
//                        graphPoints.push_back(vertex);
//                        graphs[0].points.push_back(vertex);
//                    }
                    
                }
                
                
//                    ofPoint vertex;
//                    vertex.x = graphWidth-timestampOffset*xScaling; // 1 pixel per ms
//                    vertex.y = height-((float)serial.messages[i].pinValues[pinId].value/1023*height);
//                
//                
//                graphPoints.push_back(vertex);
            }
            
        }
        
        //        cout << xScaling<<endl;
        
        //        cout << "rightmost vertex is "<<graphPoints[graphPoints.size()-1].x<<endl;
        
        
        
        
//        graphPoints.push_back(ofPoint(graphWidth, height));
        
        
        
        
        ofTranslate(100, 100);
        
//        ofSetHexColor(0x00ff00);
//        ofRect(0, 0, graphWidth, (float)height/10);
        
        ofSetHexColor(blue);
//        ofBeginShape();
//            ofVertexes(graphPoints);
//        ofEndShape();
        
        glEnable(GL_LINE_SMOOTH);
//        glLineWidth(1.5);
        glBegin(GL_LINE_STRIP);
        
//        for (int i=0; i<graphs[0].points.size(); i++) {
//            glVertex2f(graphPoints[i].x, graphPoints[i].y);
//            glVertex2f(graphs[0].points[i].x, graphs[0].points[i].y);
//            
//        }
        
        glEnd();
        
        
        
        for (int i=0; i<graphs.size(); i++){
            if (graphs[i].points.size()>0){
                
                ofSetColor(200, 200, i*10+50);
                
                glBegin(GL_LINE_STRIP);
                
                for (int j=0; j<graphs[i].points.size(); j++){
                    
                    glVertex2f(graphs[i].points[j].x, graphs[i].points[j].y);
                    
                }
                
                glEnd();
            }
            
        }
    }
    
}
