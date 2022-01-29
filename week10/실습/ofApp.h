#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    /* WaterFall-related member variables Regions */
    // flag variables
    int draw_flag;
    int load_flag;

    // Line segment and dot related variables
    int line_num, dot_num;
    float dot_diameter;

	// index for dots
	int dot_index = 0;
    
    /* WaterFall-related member functions */
    void processOpenFileSelection(ofFileDialogResult openFileResult);
	// 2nd week portion.
    void initializeWaterLines();

	/* WaterFall-related member structure */
	struct line {
		int x1; int y1;
		int x2; int y2;
	}; line *linearr;

	struct dot {
		int x; int y;
	}; dot *dotarr;
};
