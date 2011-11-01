#include "ofMain.h"
#include "arduinoGrapher.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
    window.setGlutDisplayString("rgba double samples>=8 depth"); //antialiasing

	ofSetupOpenGL(&window, 1200,924, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new arduinoGrapher());

}
