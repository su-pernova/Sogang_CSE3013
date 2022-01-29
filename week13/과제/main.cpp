#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
	// setup the GL context HD Window
	ofSetupOpenGL(1536, 1152, OF_WINDOW);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
}