#pragma once

#include "ofMain.h"
#include "serial.h"

class arduinoGrapher : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        Serial serial;
    
        void handleGui();
        void toggleButton(int i);
        
        struct graph {
            vector<ofPoint>points;
            int color;
        };
    
        void setupGraphs();
    
//        graph graphs[20];
        vector <graph>graphs;
    
        void drawGraph(int posX, int posY, int width, int height, float graphScale);
		
};
