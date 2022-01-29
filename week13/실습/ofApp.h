#pragma once

#include "ofMain.h"
class ofApp : public ofBaseApp {

public:
	// OpenFrameWorks Basic Functions
	void setup();
	void draw();
	void keyPressed(int key);
	void keyReleased(int key);
	void processOpenFileSelection(ofFileDialogResult openFileResult);

	// Maze Variables and Functions
	int loadFlag;
	int mazeFlag;
	int DFSFlag;
	int BFSFlag;
	int messageFlag1;
	int messageFlag2;
	int messageFlag3;

	int cellLength = 56; // height, width of one cell
	int WIDTH;		// Width of Maze
	int HEIGHT;		// Height of Maze
	string *mazeLines; // store each line of maze.maz
	int **maze; // check visited cells

	// Function and variables for executing DFS
	struct cell {
		int row_idx;
		int col_idx;
	};

	cell *stack; // stores shortest path
	int stackNum = 0; // index of stack top
	cell *trace; // store trace(every cell visitied)
	int traceNum = 0;

	int top_i = 0;
	int top_j = 0;

	void DFS();

};