#include "ofApp.h"
#include <cmath>

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(1000); // Limit the speed of our program to 15 frames per second
    // The background before we do anything with the brush
    ofBackground(255,255,255); // draw white background
    ofSetLineWidth(4);
    
    draw_flag = 0;
    load_flag = 0;
	water_flag = 0;
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
    
    
	if (draw_flag) {
		// Draw the line segment and dot in which water starts to flow in the screen.
		// Note that after drawing line segment and dot, you have to make selected water start dot colored in red.

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
	}

	// Draw waterfall(only draw when water_flag is 1)
	if (water_flag) { // water drops down
		ofSetColor(0, 0, 255);

		dotdraw[draw_num].x = water_x;
		dotdraw[draw_num].y = water_y;
		draw_num++;

		for (int i = 0; i < draw_num; i++) {
			ofDrawCircle(dotdraw[i].x, dotdraw[i].y, 9);
		}
		
		// Water falls straight down until it reaches end of screen or shelf
		// 1) Reaches end of screen
		if (candidate_num == 0 && water_y < (ofGetHeight() - 40)) {
			water_y = water_y + 1;
			slope_a = (standard_y - water_y) / (standard_x - water_x); }
		// 2) Reaches shelf
		else if (abs(slope_a - slope_b) > EPSILON) {
			water_y = water_y + 1;
			slope_a = (standard_y - water_y) / (standard_x - water_x); }
		else { // 3) Reached shelf : flows along the shelf
			if (candidate_num != 0) {
				if(standard_num == 1 && water_x != min_line.x2){
					// flows to (x2, y2)
					water_y = water_y + 1;
					water_x = (1.0 / slope_b) + water_x;
					if (slope_b > 0) { if (water_x > min_line.x2) water_x = min_line.x2; }
					if (slope_b < 0) { if (water_x < min_line.x2) water_x = min_line.x2; }
				}
				else if(standard_num == 2 && water_x != min_line.x1){
					// flows to (x1, y1)
					water_y = water_y + 1;
					water_x = (1.0 / slope_b) + water_x;
					if (slope_b > 0) { if (water_x > min_line.x1) water_x = min_line.x1; }
					if (slope_b < 0) { if (water_x < min_line.x1) water_x = min_line.x1; }
				}
				else { // 4) Reached end of shelf : water falls straight down until reaches end of screen or shelf
					min_line.x1 = 0;
					min_line.y1 = 0;
					min_line.x2 = 0;
					min_line.y2 = 0;
					candidate_num = 0;
					min_y = 999999;
					// deallocate existing candidate_line
					delete[] candidate_line;
					candidate_line = new line[line_num];

					for (int i = 0; i < line_num; i++) {
						if (linearr[i].x1 < water_x && linearr[i].x2 > water_x) {
							if (linearr[i].y1 > water_y  || linearr[i].y2 > water_y) {
								candidate_line[candidate_num].x1 = linearr[i].x1;
								candidate_line[candidate_num].y1 = linearr[i].y1;
								candidate_line[candidate_num].x2 = linearr[i].x2;
								candidate_line[candidate_num].y2 = linearr[i].y2;
								candidate_num++;
							}
						}
						else if (linearr[i].x2 < water_x && linearr[i].x1 > water_x) {
							if (linearr[i].y1 > water_y || linearr[i].y2 > water_y) {
								candidate_line[candidate_num].x1 = linearr[i].x1;
								candidate_line[candidate_num].y1 = linearr[i].y1;
								candidate_line[candidate_num].x2 = linearr[i].x2;
								candidate_line[candidate_num].y2 = linearr[i].y2;
								candidate_num++;
							}
						}
					}

					for (int i = 0; i < candidate_num; i++) {
						if (candidate_line[i].y1 < min_y) {
							min_y = candidate_line[i].y1;
							min_line.x1 = candidate_line[i].x1;
							min_line.y1 = candidate_line[i].y1;
							min_line.x2 = candidate_line[i].x2;
							min_line.y2 = candidate_line[i].y2;
							if (candidate_line[i].y2 < min_y) {
								min_y = candidate_line[i].y2;
								min_line.x1 = candidate_line[i].x1;
								min_line.y1 = candidate_line[i].y1;
								min_line.x2 = candidate_line[i].x2;
								min_line.y2 = candidate_line[i].y2;
							}
						}
					}

					water_y = water_y + 1;
					standard_num = 1;
					standard_x = min_line.x1;
					standard_y = min_line.y1;
					if (min_line.y2 < min_line.y1) {
						standard_num = 2;
						standard_x = min_line.x2;
						standard_y = min_line.y2;
					}

					slope_b = ((float)min_line.y1 - (float)min_line.y2) / ((float)min_line.x1 - (float)min_line.x2);
					slope_a = ((float)standard_y - water_y) / ((float)standard_x - water_x);
				}
			}
		}
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
        // Reset draw_flag
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
		if (draw_flag) {
			draw_num = 0;
			min_line.x1 = 0;
			min_line.y1 = 0;
			min_line.x2 = 0;
			min_line.y2 = 0;
			candidate_num = 0;
			min_y = 999999;

			candidate_line = new line[line_num];
			dotdraw = new dot[99999];

			for (int i = 0; i < line_num; i++) {
				if (linearr[i].x1 < dotarr[dot_index].x && linearr[i].x2 > dotarr[dot_index].x) {
					if (linearr[i].y1 > dotarr[dot_index].y || linearr[i].y2 > dotarr[dot_index].y) {
						candidate_line[candidate_num].x1 = linearr[i].x1;
						candidate_line[candidate_num].y1 = linearr[i].y1;
						candidate_line[candidate_num].x2 = linearr[i].x2;
						candidate_line[candidate_num].y2 = linearr[i].y2;
						candidate_num++;
					}
				}
				else if (linearr[i].x2 < dotarr[dot_index].x && linearr[i].x1 > dotarr[dot_index].x) {
					if (linearr[i].y1 > dotarr[dot_index].y || linearr[i].y2 > dotarr[dot_index].y) {
						candidate_line[candidate_num].x1 = linearr[i].x1;
						candidate_line[candidate_num].y1 = linearr[i].y1;
						candidate_line[candidate_num].x2 = linearr[i].x2;
						candidate_line[candidate_num].y2 = linearr[i].y2;
						candidate_num++;
					}
				}
			}

			for (int i = 0; i < candidate_num; i++) {
				if (candidate_line[i].y1 < min_y) {
					min_y = candidate_line[i].y1;
					min_line.x1 = candidate_line[i].x1;
					min_line.y1 = candidate_line[i].y1;
					min_line.x2 = candidate_line[i].x2;
					min_line.y2 = candidate_line[i].y2;
					if (candidate_line[i].y2 < min_y) {
						min_y = candidate_line[i].y2;
						min_line.x1 = candidate_line[i].x1;
						min_line.y1 = candidate_line[i].y1;
						min_line.x2 = candidate_line[i].x2;
						min_line.y2 = candidate_line[i].y2;
					}
				}
			}

			water_x = dotarr[dot_index].x;
			water_y = dotarr[dot_index].y + 1;
			standard_num = 1;
			standard_x = min_line.x1;
			standard_y = min_line.y1;
			if (min_line.y2 < min_line.y1) {
				standard_num = 2;
				standard_x = min_line.x2;
				standard_y = min_line.y2;
			}

			slope_b = ((float)min_line.y1 - (float)min_line.y2) / ((float)min_line.x1 - (float)min_line.x2);
			slope_a = ((float)standard_y - water_y) / ((float)standard_x - (float)dotarr[dot_index].x);

			water_flag = 1;
			draw();
		}
	}

	if (key == 'e' || key == 'E') {
		// End waterfall and clear water path
		// Now user can change dot
		if (water_flag) {
			delete[] candidate_line; // deallocate candidate_line
			delete[] dotdraw; // deallocate candidate_line
			water_flag = 0;
			draw();
		}
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

	// This is selection dot control part.
	// You can select dot in which water starts to flow by left, right direction key (<- , ->).
	if (key == OF_KEY_RIGHT) {
		if (!water_flag) {
			dot_index++;
			if (dot_index > dot_num - 1) dot_index = 0;
			cout << "Selcted Dot Coordinate is (" << dotarr[dot_index].x << ", " << dotarr[dot_index].y << ")" << endl;
			draw(); // Change position of a red dot.
		}
	}
	if (key == OF_KEY_LEFT) {
		if (!water_flag) {
			dot_index--;
			if (dot_index < 0) dot_index = dot_num - 1;
			cout << "Selcted Dot Coordinate is (" << dotarr[dot_index].x << ", " << dotarr[dot_index].y << ")" << endl;
			draw(); // Change position of a red dot.
		}
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


