
/* ===================================================================

20181202 김수미 / maze 2주차 과제

맥북에서 작성하여 윈도우 창 지원이 안돼 waterfall 과제에서처럼 키보드
키를 누르면 미로와 미로 탈출 경로를 화면에 그릴 수 있게 작성했습니다.

====================================================================== */

#include "ofApp.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(1000);
	ofBackground(255, 255, 255);
	ofSetLineWidth(5);
	cout << "Press 'L' to select (.maz) file." << endl;
	cout << "Press 'Q' to quit." << endl;

	// init flags
	loadFlag = 0;
	mazeFlag = 0;
	DFSFlag = 0;
	BFSFlag = 0;
	messageFlag1 = 1;
	messageFlag2 = 1;
	messageFlag3 = 1;
}


//--------------------------------------------------------------
void ofApp::draw() {
	// 1. Draw Maze on the screen
	if (loadFlag && mazeFlag) {
		int startX = 50;
		int startY = 50;
		ofSetColor(0, 0, 0);
		ofNoFill();

		for (int i = 0; i < (HEIGHT * 2) + 1; i++) {
			for (int j = 0; j < (WIDTH * 2) + 1; j++) {
				if (i % 2 == 0 && j % 2 == 1) {
					if (mazeLines[i][j] == '-') {
						ofDrawLine(startX, startY, startX + cellLength, startY);
						startX += cellLength;
					}
					else if (mazeLines[i][j] == ' ') startX += cellLength;
				}
				if (i % 2 == 1 && j % 2 == 0) {
					if (mazeLines[i][j] == '|') {
						ofDrawLine(startX, startY, startX, startY + cellLength);
						startX += cellLength;
					}
					else if (mazeLines[i][j] == ' ') startX += cellLength;
				}
			}
			startX = 50;
			if (i % 2 == 1)startY += cellLength;
		}

		if (messageFlag1) {
			cout << " " << endl;
			cout << "Maze drawn on the screen." << endl;
			cout << ">> Press 'D' to execute DFS." << endl;
			cout << ">> Press 'B' to execute BFS." << endl;
			cout << ">> Press 'L' to open other (.maz) file." << endl;
			cout << ">> Press 'Q' to quit." << endl;
			messageFlag1 = 0;
		}
	}

	// 2. Execute Depth First Search
	if (DFSFlag) {
		int startX = 50 + (cellLength / 2);
		int startY = 50 + (cellLength / 2);

		ofSetColor(0, 0, 255);
		for (int i = 0; i < traceNum - 1; i++) {
			int lineY2 = 50 + (trace[i].row_idx)*cellLength + cellLength / 2;
			int lineX2 = 50 + (trace[i].col_idx)*cellLength + cellLength / 2;
			int lineY1 = 50 + (trace[i + 1].row_idx)*cellLength + cellLength / 2;
			int lineX1 = 50 + (trace[i + 1].col_idx)*cellLength + cellLength / 2;
			ofDrawLine(lineX1, lineY1, lineX2, lineY2);
		}

		ofSetColor(255, 0, 0);
		for (int i = 0; i < stackNum - 1; i++) {
			int lineY2 = 50 + (stack[i].row_idx)*cellLength + cellLength / 2;
			int lineX2 = 50 + (stack[i].col_idx)*cellLength + cellLength / 2;
			int lineY1 = 50 + (stack[i + 1].row_idx)*cellLength + cellLength / 2;
			int lineX1 = 50 + (stack[i + 1].col_idx)*cellLength + cellLength / 2;
			ofDrawLine(lineX1, lineY1, lineX2, lineY2);
		}

		// start point
		ofSetColor(0, 0, 255);
		ofFill();
		ofDrawRectangle(startX - 2.5, startY - 2.5, 5, 5);

		// end point
		ofSetColor(255, 0, 0);
		ofDrawRectangle(startX + cellLength*(WIDTH - 1) - 2.5, startY + cellLength*(HEIGHT - 1) - 2.5, 5, 5);

		// print confirm message
		if (messageFlag2) {
			cout << " " << endl;
			cout << "DFS executed." << endl;
			cout << ">> Press 'B' to execute BFS." << endl;
			cout << ">> Press 'L' to open other (.maz) file." << endl;
			cout << ">> Press 'Q' to quit." << endl;
			messageFlag2 = 0;
		}

	}

	// 3. Execute Breadth First Search
	if (BFSFlag) {
		int startX = 50 + (cellLength / 2);
		int startY = 50 + (cellLength / 2);

		// start point
		ofSetColor(66, 245, 46);
		ofFill();
		ofDrawRectangle(startX - 2.5, startY - 2.5, 5, 5);

		// end point
		ofSetColor(255, 0, 0);
		ofDrawRectangle(startX + cellLength * (WIDTH - 1) - 2.5, startY + cellLength * (WIDTH - 1) - 2.5, 5, 5);

		if (messageFlag3) {
			// print confirm message
			cout << " " << endl;
			cout << "BFS executed." << endl;
			cout << ">> Press 'D' to execute BFS." << endl;
			cout << ">> Press 'L' to open other (.maz) file." << endl;
			cout << ">> Press 'Q' to quit." << endl;
			messageFlag3 = 0;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'd' || key == 'D') {
		if (!loadFlag) {
			cout << ">> You must open file first." << endl;
			return; }
		DFSFlag = 1;
		BFSFlag = 0;
		messageFlag2 = 1;

		// init maze array
		maze = new int*[HEIGHT];
		for (int i = 0; i < HEIGHT; i++) maze[i] = new int[WIDTH];
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) maze[i][j] = 0;
		}

		// create stack and trace array
		stack = new cell[HEIGHT*WIDTH];
		trace = new cell[HEIGHT*WIDTH*HEIGHT*WIDTH];
		stackNum = 0;
		traceNum = 0;

		DFS();
	}

	if (key == 'b' || key == 'B') {
		if (!loadFlag) {
			cout << ">> You must open file first." << endl;
			return; }
		DFSFlag = 0;
		BFSFlag = 1;
		messageFlag3 = 1;
	}

	if (key == 'q' || key == 'Q') {
		delete[] stack;
		delete[] trace;
		for (int i = 0; i < HEIGHT; i++) delete[] maze[i];
		delete[] maze;
		cout << " " << endl;
		cout << "Allocated memories have been freed." << endl;
		_Exit(0);
	}


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'l' || key == 'L') {
		// init memories and flags
		if (loadFlag == 1) {
			delete[] stack;
			delete[] trace;
			for (int i = 0; i < HEIGHT; i++) delete[] maze[i];
			delete[] mazeLines;
			mazeFlag = 0;
			DFSFlag = 0;
			BFSFlag = 0;
			messageFlag1 = 1;
			messageFlag2 = 1;
			messageFlag3 = 1;
		}

		DFSFlag = 0;
		BFSFlag = 0;
		mazeLines = new string[100];
		
		// Open the Open File Dialog
		ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
		string fileName = openFileResult.getName();
		
		// Check whether the user opened a file
		if (fileName.find(".maz") == string::npos) {
			cout << ">> You should select (.maz) file" << endl;
		}
		else if (fileName.find(".maz") != string::npos && openFileResult.bSuccess) {
			ofLogVerbose("User selected a file");
			cout << ">> File name is '" << openFileResult.getName() << "'." << endl;
			cout << ">> We found the target file." << endl;
			processOpenFileSelection(openFileResult);
			loadFlag = 1;
		}
		else cout << ">> Target file does not exists." << endl;
	}
}

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {
	openFileResult.getPath();
	ifstream input;
	input.open(openFileResult.filePath);

	string buffer;
	HEIGHT = 0;

	while (input.peek() != EOF) {
		getline(input, buffer);
		WIDTH = buffer.length();
		mazeLines[HEIGHT] = buffer;
		HEIGHT++;
	}

	WIDTH = (WIDTH - 1) / 2;
	HEIGHT = (HEIGHT - 1) / 2;
	mazeFlag = 1;
	input.close();
}

void ofApp::DFS() {
	top_i = 0; top_j = 0;
	cell v;  v.row_idx = 0; v.col_idx = 0;
	stack[stackNum] = v;
	stackNum++;
	trace[traceNum] = v;
	traceNum++;
	maze[top_i][top_j] = 1;

	while (stackNum != 0) {
		if (top_i == HEIGHT - 1 && top_j == WIDTH - 1) return;

		// no right wall : can go right
		if (mazeLines[top_i * 2 + 1][(top_j + 1) * 2] == ' ' && maze[top_i][top_j + 1] == 0) {
			top_j++;
			cell a;
			a.row_idx = top_i;
			a.col_idx = top_j;
			stack[stackNum] = a;
			stackNum++;
			trace[traceNum] = a;
			traceNum++;
			maze[top_i][top_j] = 1;
			continue;
		}

		// no down wall : can go down
		else if (mazeLines[(top_i + 1) * 2][top_j * 2 + 1] == ' ' && maze[top_i + 1][top_j] == 0) {
			top_i++;
			cell a;
			a.row_idx = top_i;
			a.col_idx = top_j;
			stack[stackNum] = a;
			stackNum++;
			trace[traceNum] = a;
			traceNum++;
			maze[top_i][top_j] = 1;
			continue;
		}

		// no left wall : can go left
		else if (mazeLines[top_i * 2 + 1][top_j * 2] == ' ' && maze[top_i][top_j - 1] == 0) {
			top_j--;
			cell a;
			a.row_idx = top_i;
			a.col_idx = top_j;
			stack[stackNum] = a;
			stackNum++;
			trace[traceNum] = a;
			traceNum++;
			maze[top_i][top_j] = 1;
			continue;
		}

		// no up wall : can go up
		else if (mazeLines[top_i * 2][top_j * 2 + 1] == ' ' && maze[top_i - 1][top_j] == 0) {
			top_i--;
			cell a;
			a.row_idx = top_i;
			a.col_idx = top_j;
			stack[stackNum] = a;
			stackNum++;
			trace[traceNum] = a;
			traceNum++;
			maze[top_i][top_j] = 1;
			continue;
		}

		// can't move : go backward
		else {
			stackNum--;
			top_i = stack[stackNum-1].row_idx;
			top_j = stack[stackNum-1].col_idx;
			cell a;
			a.row_idx = top_i;
			a.col_idx = top_j;
			trace[traceNum] = a;
			traceNum++;
		}
	}
}
