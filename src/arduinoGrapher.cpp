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
int purple = 0xC100E3;
int yellow = 0xF0D000;

bool pinStatus [20] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, /*true*/false, false, false, false, false, false};
int pinColors[18] = {0xffffff, 0xeeeeee, 0xdddddd, 0xcccccc, 0xbbbbbb, 0xaaaaaa, 0x999999,0x888888,0x777777,0x666666,0x555555,0x444444, white, blue, green, orange, yellow, purple};

int graphX = 100;
int graphY = 80;
int graphW = 1000;
int graphH = 600;
int graphTimescale = 30;

int startButtonX = graphX+graphW/2;
int startButtonY = graphY+graphH+40;
int startButtonW = 140;
int startButtonH = 40;

int resetButtonX = graphX+graphW/2+150;
int resetButtonY = graphY+graphH+40;
int resetButtonW = 140;
int resetButtonH = 40;

int baudButtonX = graphX+graphW/2-200;
int baudButtonY = graphY+graphH+40;
int baudButtonW = 240;
int baudButtonH = 40;

bool drawBaudPane = false;
int baudPaneX = 600;
int baudPaneY = 462;
int baudPaneW = 180;
int baudPaneH = 350;

float xScaling = ((float)graphW/graphTimescale)/1000000;


bool cursorActive;

struct button {
    int x, y, w, h;
    bool on, hover;
    string name, description; //name is the id for the arduino
};

bool showValueDifference = false;


bool insideCentreRect(int x, int y, int rx, int ry, int rw, int rh){
    if ((x>(rx-(rw/2)))&&(x<(rx+(rw/2)))){
        if ((y>(ry-(rh/2)))&&(y<(ry+(rh/2)))){
            return true;
        }
    }
    
    return false;
}


vector<button> buttons;

void setupGui(){
    for (int i=0; i<sizeof(pinStatus); i++){
        button newButton;
        
        newButton.x = 50*i+100;
        newButton.y = 750;
        newButton.w = 40;
        newButton.h = 40;
        
        if(i>13){
            newButton.name = "a"+ofToString(i-14);
            newButton.description = "Analogue "+newButton.name;
        }else if(i<2){
            newButton.name = ofToString(i);
            newButton.description = "Digital "+newButton.name+" [NC]";
        }else{
            newButton.name = ofToString(i);
            newButton.description = "Digital "+newButton.name;
        }
        
        
        newButton.on = pinStatus[i];
        newButton.hover = false;
        
        buttons.push_back(newButton);
        
    }
}

void arduinoGrapher::drawButtons(){
    
    for (int i=0; i<buttons.size(); i++){
        
        if (i>1){
            ofSetHexColor(pinColors[i-2]);
        }else{
            ofSetHexColor(black);
        }
        
        
        if (buttons[i].hover){
            ofRect(buttons[i].x-2, buttons[i].y-2, buttons[i].w+4, buttons[i].h+4);
        }else{
            ofRect(buttons[i].x, buttons[i].y, buttons[i].w, buttons[i].h);
        }
        
//        cout << buttons[i].x<<endl;
        
        if (!buttons[i].on){
            ofSetHexColor(offBlack);
            ofRect(buttons[i].x+2, buttons[i].y+2, buttons[i].w-4, buttons[i].h-4);
        }
        
        ofPushMatrix();
            ofSetHexColor(white);
            ofTranslate(buttons[i].x, buttons[i].y);
            ofRotateZ(90);
            verdana.drawString(buttons[i].description, 45, -16);
        ofPopMatrix();
        
//        if (buttons[i].hover){
//            ofSetHexColor(white);
//            ofCircle(buttons[i].x+(buttons[i].w/2), buttons[i].y+buttons[i].h+5, 5);
//            
//        }
        
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

bool insideStartButton(int x, int y){
    
    return insideCentreRect(x, y, startButtonX, startButtonY, startButtonW, startButtonH);
}

void arduinoGrapher::drawGui(){
    
//    ofTranslate(100, 500);
    
    drawButtons();
    drawResetButton();
    drawBaudChangeButton();
    
    ofPushMatrix();
        ofTranslate(startButtonX, startButtonY);
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofSetHexColor(white);
            if (insideStartButton(mouseX, mouseY)){
                ofRect(0, 0, startButtonW+5, startButtonH+5);
            }else{
                ofRect(0, 0, startButtonW, startButtonH);
            }
        
        ofSetHexColor(offBlack);
        ofRect(0, 0, startButtonW-5, startButtonH-5);
        ofSetHexColor(white);
        verdana20.drawString(recievingData?"Stop":"Start", -45, 10);

    
        ofSetRectMode(OF_RECTMODE_CORNER);
    ofPopMatrix();
    
    if (drawBaudPane){
        drawBaudGui();
    }
    
    ofPushMatrix();
    
        ofTranslate(850, graphY+graphH+30);
        ofSetHexColor(darkGrey);
        verdana.drawString("Current baud rate: "+ofToString(serial.baudRate), 0, 0);
        verdana.drawString("Current buffer size: "+ofToString(serial.messages.size()), 0, 24);
    
    ofPopMatrix();
    
    
    
}

bool insideResetButton(int x, int y){
    if ((x>(resetButtonX-(resetButtonW/2)))&&(x<(resetButtonX+(resetButtonW/2)))){
        if ((y>(resetButtonY-(resetButtonH/2)))&&(y<(resetButtonY+(resetButtonH/2)))){
            return true;
        }
    }
    
    return false;
}

void arduinoGrapher::drawResetButton(){
    ofPushMatrix();
    ofTranslate(resetButtonX, resetButtonY);
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetHexColor(white);
    if (insideResetButton(mouseX, mouseY)){
        ofRect(0, 0, resetButtonW+5, resetButtonH+5);
    }else{
        ofRect(0, 0, resetButtonW, resetButtonH);
    }
    
    ofSetHexColor(offBlack);
    ofRect(0, 0, resetButtonW-5, resetButtonH-5);
    ofSetHexColor(white);
    verdana20.drawString("Reset", -45, 10);
    
    
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofPopMatrix();
}

bool insideBaudButton(int x, int y){
    if ((x>(baudButtonX-(baudButtonW/2)))&&(x<(baudButtonX+(baudButtonW/2)))){
        if ((y>(baudButtonY-(baudButtonH/2)))&&(y<(baudButtonY+(baudButtonH/2)))){
            return true;
        }
    }
    
    return false;
}


void arduinoGrapher::drawBaudChangeButton(){
    
    ofPushMatrix();
    ofTranslate(baudButtonX, baudButtonY);
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetHexColor(white);
    if (insideBaudButton(mouseX, mouseY)){
        ofRect(0, 0, baudButtonW+5, baudButtonH+5);
    }else{
        ofRect(0, 0, baudButtonW, baudButtonH);
    }
    
    ofSetHexColor(offBlack);
    ofRect(0, 0, baudButtonW-5, baudButtonH-5);
    ofSetHexColor(white);
    verdana20.drawString("Change Baud", -105, 10);
    
    
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofPopMatrix();
    
}

void arduinoGrapher::drawBaudGui(){
    
    ofPushMatrix();
    ofTranslate(baudPaneX, baudPaneY);
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetHexColor(black);
    ofRect(0, 0, baudPaneW+8, baudPaneH+8);
    ofSetHexColor(offBlack);
    ofRect(0, 0, baudPaneW, baudPaneH);
    
    ofTranslate(-baudPaneW/2, -baudPaneH/2+50);
    
    for (int i=0; i<11; i++){
        if (insideCentreRect(mouseX, mouseY, baudPaneX, i*28+baudPaneY-(baudPaneH/2)+38, 150, 25)){
            ofCircle(20, 28*i-12, 10);
            ofSetHexColor(darkGrey);
        }else{
            ofSetHexColor(white);
        }
        verdana20.drawString(ofToString(serial.baudRates[i]), 30, 28*i);
    }
    
    
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofPopMatrix();
    
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
    
//    updatePointSelection();
    
    
}



//--------------------------------------------------------------
void arduinoGrapher::setup(){
    
    ofSetDataPathRoot("../Resources/");
    ofSetWindowTitle("Arduino Pin Grapher");
    
    cout <<ofToDataPath("verdana.ttf")<<endl;
    
    ofSetFrameRate(60); //stop hogging cpu cycles
    
    setupGui();
    setupGraphs();
    
    serial.setupDevices();
    
//    serial.println("{pin_on:a0}");
//    serial.println("{print:begin}");
    
    ofBackgroundHex(offBlack);
    
    verdana.loadFont("verdana.ttf", 12, true, true);
    verdana20.loadFont("verdana.ttf", 24, true, true);
    
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
//    
//    if (serial.messages.size()>0){
//        ofSetWindowTitle("Buffer size: "+ofToString(serial.messages.size())+" first timestamp: "+ofToString(serial.messages[0].timestamp));
//    }
    
    handleGui();
    
    
//    cout << "running"<<endl;
    
}

void arduinoGrapher::drawProbes(){
    
    ofSetLineWidth(1);
    ofSetHexColor(white);
    float pos1x, pos2x, pos1y, pos2y;
    int pos1yVal, pos2yVal;
    if (probes.size()>0){
        drawProbe(probes[0], pos1x, pos1y, pos1yVal);
    }
    if (probes.size()>1){
        drawProbe(probes[1], pos2x, pos2y, pos2yVal);
        
        ofSetHexColor(white);
        ofLine(pos1x, graphY-45, pos2x, graphY-45);
        float middle = (pos1x+pos2x)/2;
        verdana.drawString(ofToString((float)(abs(probes[0].timestamp-probes[1].timestamp))/1000000), middle, graphY-50);
        
        if (showValueDifference){
            if (pos1y>0&&pos2y>0){
                ofLine(graphX+graphW+25, pos1y, graphX+graphW+25, pos2y);
                float middleY = (pos1y+pos2y)/2;
                verdana.drawString(ofToString((float)abs(pos1yVal-pos2yVal)), graphX+graphW+27, middleY+6);
            }
        }
    }
    
}

void arduinoGrapher::drawProbe(probe probe, float& xPos, float& yPos, int& yVal){
    ofPushMatrix();
    
    
        int timestampOffset = serial.messages[serial.messages.size()-1].timestamp-probe.timestamp;
        xPos = graphW-timestampOffset*xScaling+graphX;
    
        ofTranslate(xPos, 0);
        ofSetHexColor(white);
        ofLine(0, graphY-60, 0, graphY+graphH);
        verdana.drawString(ofToString((float)probe.timestamp/1000000), 0, graphY-65);
    
        yPos = -1;
    
        showValueDifference = false;
    
        for (int i=0; i<probe.pinValues.size(); i++){
            
            ofSetHexColor(pinColors[probe.pinValues[i].pin-2]);
            ofRect(10, graphY-40+(i*13), 10, 10);
            verdana.drawString(ofToString(probe.pinValues[i].value), 25, graphY-30+(i*14));
            
            if (buttons[probe.pinValues[i].pin].hover){
                showValueDifference = true;
                yVal = probe.pinValues[i].value;
                int value = (probe.pinValues[i].pin>13)? probe.pinValues[i].value:probe.pinValues[i].value*1024;
                float height =  graphH-((float)value/1023*graphH-graphY);
                yPos = height;
                ofPushMatrix();
                ofTranslate(-xPos, 0);
                ofLine(graphX, height, graphX+graphW+50, height);
                verdana.drawString(ofToString(yVal), graphX+graphW+55, height+6);
                ofPopMatrix();
            }
            
        }
    
    ofPopMatrix();
}

void arduinoGrapher::drawProbeCursor(){
    
    
//    if (!recievingData){ //useless otherwise, and helps free up the cpu from having to parse the incoming packets
//        cout << "not recieving data"<<endl;
        if (mouseY>graphY&&mouseY<(graphY+graphH)){
            if (mouseX>graphX&&mouseX<(graphX+graphW)) { //inside graph
                cursorActive = true;
                
                ofSetLineWidth(1);
                ofSetHexColor(orange);
                ofLine(mouseX, graphY, mouseX, graphY+graphH);
                
                return;
//                cout << "inside graph"<<endl;
            }
        }
    
            cursorActive = false;
        
//    }
    
}

void arduinoGrapher::addProbe(int x){

    
    probe newProbe;
    
    newProbe.x = x-graphX;

    int closestX = -1;
    int closestTimestamp = -1;
//    float xScaling = ((float)graphW/graphTimescale)/1000000;
    
    int closestIndex = -1;
    
    if(serial.messages.size()>0){
        
        //        graphPoints.push_back(ofPoint(0, height));
        
        for(int i=0; i<serial.messages.size(); i++){
            
            if (serial.messages[i].pinValues.size()>0){
                //                cout<<"message pin is "<<serial.messages[i].pinValues[0].pin<<endl;
                int timestamp = serial.messages[i].timestamp;
                int timestampOffset = serial.messages[serial.messages.size()-1].timestamp-timestamp;
                
                int xPos = graphW-timestampOffset*xScaling;
                
                if (abs(xPos-newProbe.x)<abs(closestX-newProbe.x)){
                    closestX = xPos;
                    closestTimestamp = timestamp;
                    closestIndex = i;
                }
                
            }
        }
    }
    
    newProbe.pinValues = serial.messages[closestIndex].pinValues;
    
    newProbe.timestamp = closestTimestamp;
    
    cout << "closest timestamp found is"<<closestTimestamp<<endl;
    
    if(probes.size()>1){
        probes.erase(probes.begin());
    }
    probes.push_back(newProbe);

}

//--------------------------------------------------------------
void arduinoGrapher::draw(){
    

    drawGraph(graphX, graphY, graphW, graphH, graphTimescale);
    drawGui();
    if(!drawBaudPane) drawProbeCursor();
    drawProbes();

    
//    ofSetHexColor(orange);
//    ofCircle(mouseX, mouseY, 10);
    
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
    
    if (drawBaudPane){ //overrides all other commands
        
        if (insideCentreRect(mouseX, mouseY, baudPaneX, baudPaneY, baudButtonW, baudPaneH)){
            
            for(int i=0; i<11; i++){
                
                if (insideCentreRect(mouseX, mouseY, baudPaneX, i*28+baudPaneY-(baudPaneH/2)+38, 150, 25)){
                    serial.setBaud(serial.baudRates[i]);
                    serial.serialConnection.flush(true, true);
                    
                    serial.println("{print:begin}");
                    recievingData = true;
                    
                    drawBaudPane = false;
                }
            }
            
        }else{
            drawBaudPane = false;
        }
        
    }else{
        
        
        if (mouseY>graphY&&mouseY<(graphY+graphH)){
            if (mouseX>graphX&&mouseX<(graphX+graphW)) { //inside graph
                
                if (cursorActive){
                    
                    cout << "adding probe"<<endl;
                    if(serial.messages.size()>0){
                        addProbe(x);
                    }
                    
                }
                
            }
        }else if (insideStartButton(mouseX, mouseY)){
            if(recievingData){
                serial.println("{print:end}");
                recievingData = false;
            }else{
                serial.println("{print:begin}");
                recievingData = true;
            }
        }else if(insideResetButton(mouseX, mouseY)){
            
            serial.messages.clear();
            probes.clear();
            
            
        }else if(insideBaudButton(mouseX, mouseY)){
            
            if(!drawBaudPane){
                
                serial.println("{print:end}");
                recievingData = false;
                
                drawBaudPane = true;
                
            }
            
            
        }else{
            for (int i=0; i<buttons.size(); i++){
                
                if(mouseX>=buttons[i].x&&mouseX<=buttons[i].x+buttons[i].w){
                    if(mouseY>=buttons[i].y&&mouseY<=buttons[i].y+buttons[i].h){
                        
                        toggleButton(i);
                        
                    }
                    
                }
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
    
    ofColor color;
    color.r = 0;
    color.g = 0;
    color.b = 255;
    
    
    
}


void arduinoGrapher::drawGraph(int posX, int posY, int graphWidth, int height, float graphScale){ //graph scale is the number of seconds represented
    
    float xScaling = ((float)graphWidth/graphScale)/1000000;
    
    int pinId = 0;
    
    ofSetHexColor(0x222222);
    ofRect(posX, posY, graphWidth, height);
    
    vector<ofPoint>graphPoints;
    graph graphs[20]; //created every loop ensures it is clear. performance must suck tho
//    vector<graph>graphs;
//    
    for (int i=0; i<20; i++){
        
        graph newGraph;
        //setup colours
        graphs[i] = newGraph;
    }
    
    
    if(serial.messages.size()>0){
        
//        graphPoints.push_back(ofPoint(0, height));
        
        
        for(int i=0; i<serial.messages.size(); i++){
            
            if (serial.messages[i].pinValues.size()>0){
                //                cout<<"message pin is "<<serial.messages[i].pinValues[0].pin<<endl;
                int timestamp = serial.messages[i].timestamp;

                int timestampOffset = serial.messages[serial.messages.size()-1].timestamp-timestamp;
                
                // cout << "timestamp: "<<timestamp<<", first: "<<serial.messages[1].timestamp<<", offset: "<<timestampOffset<<endl;
//                cout << graphs[i].points.size()<<endl;
//                /*if (graphs[i].points.size()>0) */graphs[i].points.clear();
                
//                cout << graphs[i].points.size()<<endl;
                
                for (int j=0; j<serial.messages[i].pinValues.size();j++){
                    
                    int pin = serial.messages[i].pinValues[j].pin;
                    
                    ofPoint vertex;
                    vertex.x = graphWidth-timestampOffset*xScaling+posX; // 1 pixel per ms
                    int value = (pin>13)? serial.messages[i].pinValues[j].value:serial.messages[i].pinValues[j].value*1024;
                    vertex.y = height-((float)value/1023*height-posY);
//                    
//                  
                    
                    
                    graphs[pin].points.push_back(vertex);
//                    cout << "accessing graph # "<<serial.messages[i].pinValues[j].pin<<endl;
//                    graphs[j].points.push_back(vertex);

                    
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
        
        
        ofPushMatrix();
        
//        ofTranslate(100, 100);
        
//        ofSetHexColor(0x00ff00);
//        ofRect(0, 0, graphWidth, (float)height/10);
        
        ofSetHexColor(blue);
//        ofBeginShape();
//            ofVertexes(graphPoints);
//        ofEndShape();
        
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(1.5);
        
        for (int i=2; i<20; i++){
            if (graphs[i].points.size()>0){
                
                ofSetHexColor(pinColors[i-2]);
                
                glBegin(GL_LINE_STRIP);
                
                for (int j=0; j<graphs[i].points.size(); j++){
                    
                    glVertex2f(graphs[i].points[j].x, graphs[i].points[j].y);
                    
                }
                
                glEnd();
            }
            
        }
        
        
        ofPopMatrix();
        
    }
    
}
