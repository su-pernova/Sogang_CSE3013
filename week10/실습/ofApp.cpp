#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(15); // Limit the speed of our program to 15 frames per second
    // The background before we do anything with the brush
    ofBackground(255,255,255); // draw white background
    ofSetLineWidth(5);
    
    draw_flag = 0;
    load_flag = 0;
    dot_diameter = 20.0f;
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(127,23,31);  // Set the drawing color to brown
    // Draw shapes for ceiling and floor
    ofDrawRectangle(0, 0, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofDrawRectangle(0, 728, 1024, 40); // Top left bottom at (50, 50), 100 wide x 100 high
    
    
    if( draw_flag ){
        /* COMSIL1-TODO 3 : Draw the line segment and dot in which water starts to flow in the screen.
         Note that after drawing line segment and dot, you have to make selected water start dot colored in red.*/

		// Draw lines.
		for (int i = 0; i < line_num; i++) {
			ofSetColor(127, 23, 31);
			ofSetLineWidth(5);
			ofDrawLine(linearr[i].x1, linearr[i].y1, linearr[i].x2, linearr[i].y2);
		}

		// Draw dots(circles).
		for (int i = 0; i < dot_num; i++) {
			ofFill();
			ofSetColor(0, 0, 0);
			ofDrawCircle(dotarr[i].x, dotarr[i].y, 10);
		}

		// Draw red a dot(circle).
		ofFill();
		ofSetColor(255, 0, 0);
		ofDrawCircle(dotarr[dot_index].x, dotarr[dot_index].y, 10);

        // 2nd week portion.
        //ofSetLineWidth(2);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'v' || key == 'V') {
        // HACK: only needed on windows, when using ofSetAutoBackground(false)
        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_" + ofGetTimestampString() + ".png");
    }

    if (key == 'q' || key == 'Q'){
        // Reset flags
        draw_flag = 0;

        // Free the dynamically allocated memory exits.
		delete[] linearr;
		cout << "Memory for line segment has been freed." << endl;
		delete[] dotarr;
		cout << "Memory for dot has been freed." << endl;
        _Exit(0);
    }

    if (key == 'd' || key == 'D'){
        if( !load_flag ) return;
        /* COMSIL1-TODO 2: This is draw control part.
        You should draw only after when the key 'd' has been pressed.
        */
		draw_flag = 1;
		draw();
	}

	if (key == 's' || key == 'S') {
		// 2nd week portion.
	}

	if (key == 'e' || key == 'E') {
		// 2nd week portion.
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (key == 'l' || key == 'L') {
		// Open the Open File Dialog
		ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a only txt for Waterfall");

		// Check whether the user opened a file
		if (openFileResult.bSuccess) {
			ofLogVerbose("User selected a file");
			cout << "We found the target file. " << endl;
			// We have a file, so let's check it and process it
			processOpenFileSelection(openFileResult);
			load_flag = 1;
		}
	}

	/* COMSIL1-TODO 4: This is selection dot control part.
	 You can select dot in which water starts to flow by left, right direction key (<- , ->).
	 */
	if (key == OF_KEY_RIGHT) {
		dot_index++;
		if (dot_index > dot_num-1) dot_index = 0;
		cout << "Selcted Dot Coordinate is (" << dotarr[dot_index].x << ", " << dotarr[dot_index].y  << ")" << endl;
		draw(); // Change position of a red dot.
	}
	if (key == OF_KEY_LEFT) {
		dot_index--;
		if (dot_index < 0) dot_index = dot_num - 1;
		cout << "Selcted Dot Coordinate is (" << dotarr[dot_index].x << ", " << dotarr[dot_index].y << ")" << endl;
		draw(); // Change position of a red dot.

	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {
	openFileResult.getPath();
	ifstream input;
	input.open(openFileResult.filePath);

	// input >> :  read txt file line by line
	// Save information of lines in linearr array.
	input >> line_num;
	cout << "The number of line is: " << line_num << endl;
	linearr = new line[line_num];
	for (int i = 0; i < line_num; i++) {
		line newline;
		input >> newline.x1;
		input >> newline.y1;
		input >> newline.x2;
		input >> newline.y2;
		// Check rather a line goes out from the screen or not.
		if (newline.x1 <= ofGetWidth() || newline.x2 <= ofGetWidth() || newline.y1 <= ofGetHeight() || newline.y2 <= ofGetHeight())
			this->linearr[i] = newline;
	}

	// Save information of dots in dotarr array.
	input >> dot_num;
	cout << "The number of dot is: " << dot_num << endl;
	dotarr = new dot[dot_num];
	for (int i = 0; i < dot_num; i++) {
		dot newdot;
		input >> newdot.x;
		input >> newdot.y;
		// Check rather a dot goes out from the screen or not.
		if (newdot.x <= ofGetWidth() || newdot.y <= ofGetHeight())
			this->dotarr[i] = newdot;
	}
	input.close();
}


void ofApp::initializeWaterLines() {
    ;
}


