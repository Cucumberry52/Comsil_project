/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.

	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

	=========================================================================
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	=========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
#include <algorithm>  
#include <cstdio>
#include <cstdlib>    
#include <ctime>      
#include <cmath>      


#define _CRT_SECURE_NO_WARNINGS

using namespace std;


int M, N;

struct room {
	int  id;
	bool R;
	bool D;
};

room** maze;
char** field;

int next_id = 1;

// 수평에대한 검사
void R_calculation(int y) {
	maze[y][N - 1].R = true;

	for (int x = 0; x < N - 1; x++) {
		maze[y][x].R = true;

		if (rand() % 2 == 0 && maze[y][x].id != maze[y][x + 1].id) {
			maze[y][x].R = false;

			int n_id = maze[y][x].id;
			int o_id = maze[y][x + 1].id;

			maze[y][x + 1].id = n_id;

			if (y > 0 && !maze[y - 1][x + 1].D) {
				bool no_more_o_id = 0;
				for (int temp_y = y; temp_y >= 0; temp_y--) {
					if (temp_y != y) no_more_o_id = 1;
					for (int temp_x = 0; temp_x < N;temp_x++) {
						if (maze[temp_y][temp_x].id == o_id) {
							maze[temp_y][temp_x].id = n_id;
							no_more_o_id = 0;
						}
					}

					if (no_more_o_id) break;
				}
			}


		}
	}
}



// 아래 벽의 유무를 정하는 함수
void B_calculation(int y) {


	bool flag = 1;
	int current_id = -1;

	for (int x = 0; x < N; x++) {
		maze[y][x].D = true;
	}

	for (int x = 0; x < N - 1; x++) {
		if (rand() % 2 == 0) {
			maze[y][x].D = false;
			flag = 0;
		}

		if (maze[y][x].id != maze[y][x + 1].id) {
			if (flag) maze[y][x - (rand() % (x - current_id))].D = false;

			current_id = x;
			flag = 1;
		}

		if (x == N - 2 && flag) {
			if (maze[y][x].id != maze[y][x + 1].id) maze[y][x + 1].D = false;
			else  maze[y][x + 1 - (rand() % (x + 1 - current_id))].D = false;
		}

	}


	for (int x = 0; x < N; x++) {

		if (maze[y][x].D == 1) {
			maze[y + 1][x].id = next_id++;
		}

		else {
			maze[y + 1][x].id = maze[y][x].id;
		}
	}
}

//마지막 줄에대한 함수
void Last_calculation(int y) {

	int o_id; int n_id;

	for (int x = 0; x < N - 1; ++x) {
		maze[y][x].D = true;

		if (maze[y][x].id != maze[y][x + 1].id) {
			maze[y][x].R = false;
			o_id = maze[y][x + 1].id;
			n_id = maze[y][x].id;

			maze[y][x + 1].id = n_id;

			if (!maze[y - 1][x + 1].D) {
				bool no_more_o_id = 0;
				for (int temp_y = y; temp_y >= 0; temp_y--) {
					if (temp_y != y) no_more_o_id = 1;
					for (int temp_x = 0; temp_x < N;temp_x++) {
						if (maze[temp_y][temp_x].id == o_id) {
							maze[temp_y][temp_x].id = n_id;
							no_more_o_id = 0;
						}
					}

					if (no_more_o_id) break;
				}
			}


		}

	}

	maze[y][N - 1].D = true;

}

// 본질적인 미로 함수
void Maze() {

	maze = new room * [M];
	for (int i = 0; i < M; i++) maze[i] = new room[N];

	for (int x = 0; x < N; ++x) {
		maze[0][x].id = next_id++;
	}

	for (int y = 0; y < M - 1; ++y) {
		R_calculation(y);
		B_calculation(y);
	}
	R_calculation(M - 1);
	Last_calculation(M - 1);
}

//  미로  생성함수

void Field() {
	field = new char* [2 * M + 1];
	for (int i = 0; i < 2 * M + 1; i++) field[i] = new char[2 * N + 1];


	int H = 2 * M + 1, W = 2 * N + 1;

	for (int i = 0; i < H; ++i) {
		for (int j = 0; j < W; ++j) {
			if (i % 2 == 0) {
				if (j % 2 == 0) field[i][j] = '+';
				else field[i][j] = '-';
			}

			else {
				if (j % 2 == 0) field[i][j] = '|';
				else field[i][j] = ' ';
			}

		}
	}

	for (int y = 0; y < M; ++y) {
		for (int x = 0; x < N; ++x) {
			int fx = 2 * x + 1;
			int fy = 2 * y + 1;

			//field[fy][fx] = maze[y][x].id%10  + '0';
			field[fy][fx] = ' ';

			if (!maze[y][x].R)   field[fy][fx + 1] = ' ';
			if (!maze[y][x].D)  field[fy + 1][fx] = ' ';
		}
	}

	FILE* fp = fopen("maze0.maz", "w");

	H = 2 * M + 1; W = 2 * N + 1;
	for (int i = 0; i < H; ++i) {
		for (int j = 0; j < W; ++j) {
			fprintf(fp, "%c", field[i][j]);
			//cout << field[i][j]; // cmd창에서 그리기
		}
		fprintf(fp, "\n");
		//cout << '\n'; //cmd창에서 그리기
	}
	fclose(fp);
}



void free_maze() {
	for (int i = 0; i < M; i++)
		delete[] maze[i];
	delete[] maze;
}
void free_felid() {
	for (int i = 0; i < 2 * M + 1; i++)
		delete[] field[i];
	delete[] field;
}

//--------------------------------------------------------------
void ofApp::setup() {

	cout << "N M: "; cin >> N >> M;

	if (N < 1 || M < 1) {
		cout << "Invalid size\n";
		return ;
	}

	srand((unsigned)time(NULL));
	Maze(); //by using this, enough to make file. but for next lecture , i made a Field 
	Field();

	free_maze();
	free_felid();

	start = 100; // 미로를 그리는 시작 위치 
	line_size = 100; // 선의 길이

	Goal = FALSE; // 목표지점에 도달했는지 플래그

	// 시작 위치 설정 및 나머지 위치 초기화
	X_position = start + line_size / 2.0f;
	Y_position = start + line_size / 2.0f;

	temp_X_position = 0;
    temp_Y_position = 0;

	temp2_X_position = 0;
	temp2_Y_position = 0;

	temp3_X_position = 0;
	temp3_Y_position = 0;

	temp4_X_position = 0;
	temp4_Y_position = 0;

////////////////////////////////////////////////////////////////////////////


	ofSetWindowShape(1200, 1200);
	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	//ofBackground(255, 255, 255);
	ofBackground(100);

	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;
	isbfs = false;

	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked

	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS", false, false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if (title == "Open") {
		readFile();
	}
	if (title == "Exit") {
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if (title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			DFS();
			isdfs = true;
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;

	}

	if (title == "Show BFS") {
	//	doTopmost(bChecked); // Use the checked value directly

		if (isOpen)
		{
			BFS();
			isbfs = true;
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
	}

	if (title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if (title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {



	ofSetColor(200, 100, 100);
	ofDrawRectangle((start + start / 2) - line_size / 8, (start + start / 2) - line_size / 8, 30, 30);
	ofDrawRectangle((start + start / 2) + line_size * (WIDTH - 3) / 2 - line_size / 8, (start + start / 2) + line_size * (HEIGHT - 3) / 2 - line_size / 8, 30, 30);


	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;

	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here
	
	
	for (i = 0; i < (HEIGHT - 1) / 2;i++) {
		for (j = 0; j < (WIDTH - 1) / 2; j++) {
			int x = start + j * line_size;
			int y = start + i * line_size;
			if (g.maze[i][j]->direction[0] == NULL) ofDrawLine(x, y, x + line_size, y);
			if (g.maze[i][j]->direction[1] == NULL) ofDrawLine(x + line_size, y, x + line_size, y + line_size);
			if (g.maze[i][j]->direction[2] == NULL) ofDrawLine(x, y + line_size, x + line_size, y + line_size);
			if (g.maze[i][j]->direction[3] == NULL) ofDrawLine(x, y,  x , y + line_size);
		}
	}
	
	ofSetColor(50);
	ofSetLineWidth(15);

	ofDrawLine(start, start, start + line_size * (WIDTH - 1) / 2, start);  // 위 테두리
	ofDrawLine(start + line_size * (WIDTH - 1) / 2, start, start + line_size * (WIDTH - 1) / 2, start + line_size * (HEIGHT - 1) / 2); //오른쪽 테두리
	ofDrawLine(start, start + line_size * (HEIGHT - 1) / 2.0f, start + line_size * (WIDTH - 1) / 2, start + line_size * (HEIGHT - 1) / 2.0f); // 아래 테두리
	ofDrawLine(start, start, start, start + line_size * (HEIGHT - 1) / 2); //왼쪽 테두리
	

	if (isdfs)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen)
			dfsdraw();
		else
			cout << "You must open file first" << endl;
	}

	if (isbfs)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen)
			bfsdraw();
		else
			cout << "You must open file first" << endl;
	}

	if (bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight() - 20);
	}


	if (isOpen) { 	// 미로 상에서 움직이는 점의 광선 그리기
		// 현재위치 광선
		ofSetLineWidth(7);

		ofSetColor(255, 0, 0, 100);
		light(0, line_size*3, X_position, Y_position);
		light(2, line_size*3, X_position, Y_position);
		light(4, line_size*3, X_position, Y_position);
		light(6, line_size*3, X_position, Y_position);


		ofSetColor(0, 100, 255, 100);
		light(1, line_size * 3, X_position, Y_position);

		ofSetColor(0, 255, 255, 100);
		light(3, line_size * 3, X_position, Y_position);

		ofSetColor(0, 255, 100, 100);
		light(5, line_size * 3, X_position, Y_position);

		ofSetColor(0, 100, 100, 100);
		light(7, line_size * 3, X_position, Y_position);

		// 직전위치 광선
		if (temp_X_position && temp_Y_position) {

			ofSetColor(255, 0, 0, 80);
			light(0, line_size * 3, temp_X_position, temp_Y_position);
			light(2, line_size * 3, temp_X_position, temp_Y_position);
			light(4, line_size * 3, temp_X_position, temp_Y_position);
			light(6, line_size * 3, temp_X_position, temp_Y_position);


			ofSetColor(0, 100, 255, 80);
			light(1, line_size * 3, temp_X_position, temp_Y_position);

			ofSetColor(0, 255, 255, 80);
			light(3, line_size * 3, temp_X_position, temp_Y_position);

			ofSetColor(0, 255, 100, 80);
			light(5, line_size * 3, temp_X_position, temp_Y_position);

			ofSetColor(0, 100, 100, 80);
			light(7, line_size * 3, temp_X_position, temp_Y_position);
		}

		// 직직전위치 광선
		if (temp2_X_position && temp2_Y_position) {

			ofSetColor(255, 0, 0, 55);
			light(0, line_size * 3, temp2_X_position, temp2_Y_position);
			light(2, line_size * 3, temp2_X_position, temp2_Y_position);
			light(4, line_size * 3, temp2_X_position, temp2_Y_position);
			light(6, line_size * 3, temp2_X_position, temp2_Y_position);

			ofSetColor(0, 100, 255, 55);
			light(1, line_size * 3, temp2_X_position, temp2_Y_position);

			ofSetColor(0, 255, 255, 55);
			light(3, line_size * 3, temp2_X_position, temp2_Y_position);

			ofSetColor(0, 255, 100, 55);
			light(5, line_size * 3, temp2_X_position, temp2_Y_position);

			ofSetColor(0, 100, 100, 55);
			light(7, line_size * 3, temp2_X_position, temp2_Y_position);
		}

		// 직직전위치 광선
		if (temp3_X_position && temp3_Y_position) {

			ofSetColor(255, 0, 0, 25);
			light(0, line_size * 3, temp3_X_position, temp3_Y_position);
			light(2, line_size * 3, temp3_X_position, temp3_Y_position);
			light(4, line_size * 3, temp3_X_position, temp3_Y_position);
			light(6, line_size * 3, temp3_X_position, temp3_Y_position);

			ofSetColor(0, 100, 255, 25);
			light(1, line_size * 3, temp3_X_position, temp3_Y_position);

			ofSetColor(0, 255, 255, 25);
			light(3, line_size * 3, temp3_X_position, temp3_Y_position);

			ofSetColor(0, 255, 100, 25);
			light(5, line_size * 3, temp3_X_position, temp3_Y_position);

			ofSetColor(0, 100, 100, 25);
			light(7, line_size * 3, temp3_X_position, temp3_Y_position);
		}

		// 직직직직전위치 광선
		if (temp4_X_position && temp4_Y_position) {

			ofSetColor(255, 0, 0, 10);
			light(0, line_size * 3, temp4_X_position, temp4_Y_position);
			light(2, line_size * 3, temp4_X_position, temp4_Y_position);
			light(4, line_size * 3, temp4_X_position, temp4_Y_position);
			light(6, line_size * 3, temp4_X_position, temp4_Y_position);

			ofSetColor(0, 100, 255, 10);
			light(1, line_size * 3, temp4_X_position, temp4_Y_position);

			ofSetColor(0, 255, 255, 10);
			light(3, line_size * 3, temp4_X_position, temp4_Y_position);

			ofSetColor(0, 255, 100, 10);
			light(5, line_size * 3, temp4_X_position, temp4_Y_position);

			ofSetColor(0, 100, 100, 10);
			light(7, line_size * 3, temp4_X_position, temp4_Y_position);
		}

		//현재 움직이고 있는 점 그리기
		ofSetColor(255, 0, 0);
		ofDrawCircle(X_position, Y_position, line_size / 10);

		/*
		ofDrawLine(X_position,Y_position, X_position + line_size, Y_position); // 동서남북
		ofDrawLine(X_position, Y_position, X_position , Y_position+line_size);
		ofDrawLine(X_position, Y_position, X_position - line_size, Y_position);
		ofDrawLine(X_position, Y_position, X_position , Y_position - line_size);

		ofDrawLine(X_position, Y_position, X_position + line_size, Y_position + line_size); // 대각선
		ofDrawLine(X_position, Y_position, X_position + line_size, Y_position - line_size);
		ofDrawLine(X_position, Y_position, X_position - line_size, Y_position + line_size);
		ofDrawLine(X_position, Y_position, X_position - line_size, Y_position - line_size);
		*/
	}




	if (!Goal) {
		// 도착지점 x및 y좌표
		float goal_X = (start + start / 2) + line_size * (WIDTH - 3) / 2.0f;
		float goal_Y = (start + start / 2) + line_size * (HEIGHT - 3) / 2.0f;

		if (fabs(X_position - goal_X) <= line_size / 10 && fabs(Y_position - goal_Y) <= line_size / 10) {

			Goal = true;                 
			ofSystemAlertDialog("!! CLEAR !!");         // 새로운 창을 열어 clear 출력
			ofBackground(255, 255, 255); // 암흑미로 클리어했으므로 화면을 다시 밝게하기
		}
	}



} // end Draw

  
void ofApp::light(int case_num, int light_length, float cx, float cy)
{

	// 방향에 따른 이동 크기
	float dirX = d_x[case_num]; 
	float dirY = d_y[case_num];

	// 미로 크기
	int cols = (WIDTH - 1) / 2;
	int rows = (HEIGHT - 1) / 2;

	while (light_length-- > 0)
	{
		// 방향에 따른 이동 후 위치
		float nx = cx + dirX;
		float ny = cy + dirY;

		// 빛의 경로 그리기
		ofDrawLine(cx, cy, nx, ny);

		// 현재 위치가 속한 미로 상의 방 번호
		int c_cell_x = int((cx - start) / line_size);
		int c_cell_y = int((cy - start) / line_size);

		// 이동 후 위치가 속한 미로 상의 방 번호
		int n_cell_x = int((nx - start) / line_size);
		int n_cell_y = int((ny - start) / line_size);

		// 이동 방향에 따른 수평 벽 혹은 수직 벽과의 충돌 여부
		bool V_hit = false, H_hit = false;

		// 맵 바깥 경계 검사
		if (nx <= start || nx >= start + cols * line_size)  V_hit = true;
		if (ny <= start || ny >= start + rows * line_size)  H_hit = true;

		//현재 미로상의 방 번호가 설정한 범위내에 올바르게 존재한다면
		if ((0 <= c_cell_x && c_cell_x < cols && 0 <= c_cell_y && c_cell_y < rows) && (0 <= n_cell_x && n_cell_x < cols && 0 <= n_cell_y && n_cell_y < rows))
		{
			// 이동 방향에 따른 벽 검사. 미로의 방향과 점의 움직이는 방향에 대한 배열을 동기화시키는 작업

			// 점의 이동 방향과 미로상에서 확인해야할 방 방향을 매칭시켜주는 변수
			// 미로 상에서 방향은 0 1 2 3 각각 북 동 남 서를 의미한다.

			int maze_dirX;
			int maze_dirY;

			if (dirX > 0) {
				maze_dirX =  1;
			}
			else {
				maze_dirX = 3;
			}

			if (dirY > 0) {
				maze_dirY = 2;
			}
			else {
				maze_dirY = 0;
			}
			
			// 현재 방에서 이동하려는 방으로의 벽의 유무를 검사할 변수
			bool H_wall = FALSE;
			bool V_wall = FALSE;

			if (g.maze[c_cell_y][c_cell_x]->direction[maze_dirX] == NULL) H_wall = TRUE;
			if (g.maze[c_cell_y][c_cell_x]->direction[maze_dirY] == NULL) V_wall = TRUE;
			// 기본 정보들 초기 세팅 완료
			 

			// ***대각선으로 이동하려고 할 때*** 
			if ((n_cell_x != c_cell_x) && (n_cell_y != c_cell_y)){ 
			
				
				if (H_wall || V_wall){ // 1. 기존 방에서 이동하려는 방으로의 인접한 벽 검사
				
					V_hit = H_wall;
					H_hit = V_wall;
				}
				else{ // 2. 먼저 수평 방향으로 이동한 방에서 수직 이동 방향에 대한 벽 검사 || 먼저 수직 방향으로 이동한 방에서 수평 이동 방향에대한 벽 검사 
					if (g.maze[c_cell_y][n_cell_x]->direction[maze_dirY] == NULL || g.maze[n_cell_y][c_cell_x]->direction[maze_dirX] == NULL) {
						V_hit = true;
						H_hit = true;
					}
				}
			}
			

			//  ***단순히 수평이나 수직 방향으로 움직일 때***
			else{ 
			
				if ((n_cell_x != c_cell_x) && H_wall) V_hit = true;
				if ((n_cell_y != c_cell_y) && V_wall) H_hit = true;
			
			}
		}






		// 충돌 여부를 통한 다음 점의 

		if (!V_hit && !H_hit) { // 벽으로 부딪힌 일이 없으면 그냥 이동
			cx = nx;
			cy = ny;

		}

		else { // 벽이랑 부딪혔다면
			if (V_hit)   dirX = -dirX;
			if (H_hit) dirY = -dirY;
		
			//cx += dirX * 0.1f;
			//cy += dirY * 0.1f;
		}
		
	}
}
  
  
void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if (bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else {
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU));
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth() - ofGetWidth()) / 2, (ofGetScreenHeight() - ofGetHeight()) / 2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if (bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if (bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if (GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	// Escape key exit has been disabled but it can be checked here
	if (key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if (bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if (key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if (key == 'f') {
		bFullscreen = !bFullscreen;
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash

	}


	if (key == 'l') ofBackground(255, 255, 255);
	if (key == 'b') ofBackground(100);




	if (isOpen) {

		// 소문자 position들은 미로의 방 위치를 나타낸다.
		int x_position = int((X_position - start) / line_size);
		int y_position = int((Y_position - start) / line_size);

		// 이동에 대한 입력이 들어오면 각 위치에 대한 정보들을 한 칸 old한 정보로 이동시킨다.
		if (key == OF_KEY_RIGHT || key == OF_KEY_LEFT || key == OF_KEY_DOWN || key == OF_KEY_UP) {

			temp4_X_position = temp3_X_position, temp4_Y_position = temp3_Y_position;
			temp3_X_position = temp2_X_position, temp3_Y_position = temp2_Y_position;
			temp2_X_position = temp_X_position, temp2_Y_position = temp_Y_position;
			temp_X_position = X_position,  temp_Y_position = Y_position;
		}

		/*
		 하술한 방향에 따른 조건문들의 내용은 다음과 같습니다.
		 첫 if문으로 이동 후 점의 좌표가, 이동 전 점이 속한 방과 같은 미로의 방안에 있는지를 검사해서 만약 그렇다면 해당 방향으로 단순히 이동을 시킵니다.
		 두번째 else if문으로 이동으로 인한 방이 바뀌는 경우에, 사이 벽 존재 여부로 이동할 수 있으면 이동하고 아니면 못 이동하게 한다.
		*/
		if (key == OF_KEY_UP) {
			if (Y_position - start - line_size / 5 >= line_size * y_position) Y_position -= line_size / 5;
			else if (g.maze[y_position][x_position]->direction[0] != NULL && Y_position - line_size / 5 > start) Y_position -= line_size / 5;
		}

		else if (key == OF_KEY_RIGHT) {
			if (X_position - start + line_size / 5 <= line_size * x_position + line_size) X_position += line_size / 5;
			else if (g.maze[y_position][x_position]->direction[1] != NULL && X_position + line_size / 5 < start + line_size * (WIDTH - 1) / 2) X_position += line_size / 5;

		}

		else if (key == OF_KEY_DOWN) {
			if (Y_position - start + line_size / 5 <= line_size * y_position + line_size) Y_position += line_size / 5;
			else if (g.maze[y_position][x_position]->direction[2] != NULL && Y_position + line_size / 5 < start + line_size * (HEIGHT - 1) / 2) Y_position += line_size / 5;

		}
		
		else if (key == OF_KEY_LEFT) {
			if (X_position - start - line_size / 5 >= line_size * x_position) 	X_position -= line_size / 5;
			else if (g.maze[y_position][x_position]->direction[3] != NULL && X_position - line_size / 5 > start) X_position -= line_size / 5;

		}
		

		
	}


} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

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
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		printf("file name is %s\n", fileName.c_str());

		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(filePath);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}



			ofBuffer buffer(file);


			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;

			// Read file line by line
			int cnt = 0;


			// TO DO
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다

			string text = buffer.getText();
			HEIGHT = 0;
			WIDTH = 0;
			// 외곽임을 알리는 경계방
			boundary = &boundary_;
			boundary->visited = TRUE;
			boundary->q_visited = TRUE;

			for (int i = 0; text[i] != '\n'; i++) WIDTH += 1;
			WIDTH--;
			for (int i = 0; i < text.size(); i += (WIDTH + 2)) HEIGHT += 1;
			


			cout << "HEIGHT = "<<HEIGHT << " " << "WIDTH = " << WIDTH << endl;

			if (HEIGHT < 3 || WIDTH < 3) {
				cout << "too small maze to do anything!" << endl;
				return false;
			}


			g.maze = new space **[(HEIGHT - 1) / 2];
			for (int h = 0; h < (HEIGHT - 1) / 2; ++h) {
				g.maze[h] = new space * [(WIDTH - 1) / 2];
				for (int w = 0; w < (WIDTH - 1) / 2; w++) {
					space* room = new space;

					room->x = w;
					room->y = h;

					//인접한 방을 담는 배열
					room->direction[0] = NULL;
					room->direction[1] = NULL;
					room->direction[2] = NULL;
					room->direction[3] = NULL;

					// dfs시 이동한 방 방향을 담는 배열
					room->v_direction[0] = FALSE;
					room->v_direction[1] = FALSE;
					room->v_direction[2] = FALSE;
					room->v_direction[3] = FALSE;

					// bfs시 이동한 방 방향을 담는 배열
					room->q_direction[0] = FALSE;
					room->q_direction[1] = FALSE;
					room->q_direction[2] = FALSE;
					room->q_direction[3] = FALSE;
					
					// bfs시 이전 방 방향을 담는 변수 
					room->before = NULL; //큐


					if(w == 0) room->direction[3]  = boundary;
					else if (w == (WIDTH - 1) / 2 - 1) room->direction[1] = boundary;
					else if (h == 0) room->direction[0] = boundary;
					else if (h == (HEIGHT - 1) / 2  - 1) room->direction[2] = boundary;
					
					room->visited = FALSE;
					room->q_visited = FALSE;
					room->q_next = NULL;


					g.maze[h][w] = room;
				}
			}

			/*
			for (int h = 0; h < (HEIGHT - 1) / 2; ++h) {
				for (int w = 0; w < (WIDTH - 1) / 2; w++) {
					cout << g.maze[h][w]->x << "," << g.maze[h][w]->y << " ";
				}
				cout << '\n';
			}
			*/

			// 파일에서 3줄씩 읽기
			for (; cnt < HEIGHT - 2; cnt += 2) {

				string first = text.substr(cnt * (WIDTH + 2), WIDTH);
				string second = text.substr((cnt + 1) * (WIDTH + 2), WIDTH);
				string third = text.substr((cnt + 2) * (WIDTH + 2), WIDTH);

				//printf("%s\n", first.c_str());
				//printf("%s\n", second.c_str());

				for (int idx = 1; idx < WIDTH; idx += 2) {

					int x = (idx - 1) / 2;
					int y = cnt  / 2;
					if (first[idx] != '-' && y > 0 ) {
						g.maze[y][x]->direction[0] = g.maze[y - 1][x];
						g.maze[y - 1][x]->direction[2] = g.maze[y][x];
					}
					if (second[idx + 1] != '|' && x < (WIDTH - 1) / 2 - 1) {
						g.maze[y][x]->direction[1] = g.maze[y][x + 1];
						g.maze[y][x + 1]->direction[3] = g.maze[y][x];
					}
					if (third[idx] != '-' && y < (HEIGHT - 1) / 2  -1){
						g.maze[y][x]->direction[2] = g.maze[y + 1][x];
						g.maze[y + 1][x]->direction[0] = g.maze[y][x];
					}
					if (second[idx - 1] != '|' && x > 0) {
						g.maze[y][x]->direction[3] = g.maze[y][x - 1];
						g.maze[y][x - 1]->direction[1] = g.maze[y][x];
					}
				}

			}
			return true;
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}
void ofApp::freeMemory() {

	//TO DO
	// malloc한 memory를 free해주는 함수


	if (g.maze == nullptr) return; 

	
	for (int r = 0; r < (HEIGHT - 1) / 2; ++r) {
		for (int c = 0; c < (WIDTH - 1) / 2; ++c) {
			delete g.maze[r][c];
			g.maze[r][c] = nullptr;
		}
			delete[] g.maze[r];        
			g.maze[r] = nullptr;
	}


	delete[] g.maze;  
	g.maze = nullptr;
}

bool ofApp::DFS()//DFS탐색을 하는 함수
{
	//TO DO
	//DFS탐색을 하는 함수 ( 3주차)
	
	top = -1;
	Stack = new space* [(HEIGHT - 1) / 2 * (WIDTH - 1) / 2] {NULL, };
	//Stack = new space * [100] {NULL,};

	Stack[0] = g.maze[0][0];
	top++;
	g.maze[0][0]->visited = TRUE;
	while (top != -1) {
		if (Stack[top] == g.maze[(HEIGHT - 1) / 2 - 1][(WIDTH - 1) / 2 - 1]) {
			return 1;
		}
		int i;
		for (i = 0; i < 4;i++) {
			if (Stack[top]->direction[i] != NULL && Stack[top]->direction[i]->visited == FALSE) {
				Stack[top+1] = Stack[top]->direction[i];
				Stack[top]->v_direction[i] = TRUE;
				top++;
				Stack[top]->visited = TRUE;
				break;
			}
		}
		if(i == 4 )Stack[top--] = NULL;
	}

	
	return 0;
}

void ofApp::dfsdraw()
{
	
	//TO DO 
	//DFS를 수행한 결과를 그린다. (3주차 내용)
	
	ofSetLineWidth(10);

	int count = 0;
	int x_start = start + start/2;
	int y_start = start + start / 2;


	ofSetColor(0, 200, 0,200);


	//출발지 도착지
	ofDrawRectangle(x_start - line_size/8, y_start -  line_size / 8, 30, 30);
	ofDrawRectangle(x_start+line_size *(WIDTH - 3) / 2 -  line_size / 8, y_start + line_size * (HEIGHT - 3) / 2 -  line_size / 8, 30, 30);



	// dfs 탐색한 방향 그리기
	ofSetColor(0, 100, 0, 30);
	for (int i = 0; i < (HEIGHT - 1) / 2;i++) {
		for (int j = 0; j < (WIDTH - 1) / 2; j++) {
			int x = x_start + j * line_size ;
			int y = y_start + i * line_size ;
			if (i != 0 && g.maze[i][j]->v_direction[0]) ofDrawLine(x, y, x, y - line_size);
			if (j != (WIDTH - 1) / 2 - 1 && g.maze[i][j]->v_direction[1]) ofDrawLine(x , y, x + line_size, y );
			if (i != (HEIGHT - 1) / 2 -1&& g.maze[i][j]->v_direction[2]) ofDrawLine(x, y , x , y + line_size);
			if (j != 0 && g.maze[i][j]->v_direction[3]) ofDrawLine(x, y, x- line_size, y);
		}
	}

	// 스택을 통한 경로 그리기
	while (count < top) {
		ofSetColor(0, 100, (count % 5) * 50 + 100,200);

		if (Stack[count]->direction[0] == Stack[count + 1]) {
			ofDrawLine(x_start, y_start, x_start , y_start - line_size);
			y_start -= line_size;
		}
		else if (Stack[count]->direction[1] == Stack[count + 1]) {
			ofDrawLine(x_start , y_start, x_start + line_size, y_start);
			x_start += line_size;
		}
		else if (Stack[count]->direction[2] == Stack[count + 1]) {
			ofDrawLine(x_start, y_start, x_start, y_start + line_size);
			y_start += line_size;
		}
		else if (Stack[count]->direction[3] == Stack[count + 1]) {
			ofDrawLine(x_start, y_start, x_start - line_size, y_start );
			x_start -= line_size;
		}
		count++;
	}

	
}




bool  ofApp::BFS() {
	q_top = -1;
	line_size = 100;

	Queue = &Queue_;
	Queue->front = g.maze[0][0];
	Queue->rear = g.maze[0][0];

	q_top++;
	Queue->rear->q_visited = TRUE;

	// 큐로 bfs 구현
	while (q_top != -1) {
		space* current = Queue->front;
		Queue->front = Queue->front->q_next;
		//cout << current->x << " " << current->y << endl;

		q_top--;

		int i = 0;
		// 모든 인접한 방 넣기
		for (;i < 4;i++) {
			if (current->direction[i] != NULL && current->direction[i]->q_visited == FALSE){
		

				current->q_direction[i] = TRUE;
				current->direction[i]->before = current;

				if (current->direction[i] == g.maze[(HEIGHT - 1) / 2 - 1][(WIDTH - 1) / 2 - 1]) {
					return 1;
				}
				if (Queue->front != NULL) {
					Queue->rear->q_next = current->direction[i];
				}
				else {
					Queue->front = current->direction[i];
				}
				Queue->rear = current->direction[i];

			q_top++;
			Queue->rear->q_visited = TRUE;
			}
		}
		
	}
	return 0;
}

void  ofApp::bfsdraw() {
	ofSetLineWidth(10);

	int count = 0;
	int x_start = start + start / 2;
	int y_start = start + start / 2;


	ofSetColor(0, 200, 0, 200);

	// 출발지 도착지
	ofDrawRectangle(x_start - line_size / 8, y_start - line_size / 8, 30, 30);
	ofDrawRectangle(x_start + line_size * (WIDTH - 3) / 2 - line_size / 8, y_start + line_size * (HEIGHT - 3) / 2 - line_size / 8, 30, 30);


	// bfs시 탐색한 경로 그리기
	ofSetColor(0, 0, 100, 30);
	for (int i = 0; i < (HEIGHT - 1) / 2;i++) {
		for (int j = 0; j < (WIDTH - 1) / 2; j++) {
			int x = x_start + j * line_size;
			int y = y_start + i * line_size;
			if (i != 0 && g.maze[i][j]->q_direction[0]) ofDrawLine(x, y, x, y - line_size);
			if (j != (WIDTH - 1) / 2 - 1 && g.maze[i][j]->q_direction[1]) ofDrawLine(x, y, x + line_size, y);
			if (i != (HEIGHT - 1) / 2 - 1 && g.maze[i][j]->q_direction[2]) ofDrawLine(x, y, x, y + line_size);
			if (j != 0 && g.maze[i][j]->q_direction[3]) ofDrawLine(x, y, x - line_size, y);
		}
	}


	ofSetColor(100, 0, 100, 200);
	space* temp = g.maze[(HEIGHT - 1) / 2 - 1][(WIDTH - 1) / 2 - 1];

	int start_x, start_y;
	int end_x, end_y;
	int color = 50;
	int c = 0;
	
	// bfs 경로 구하기
	while (temp->before) {
			c += color;
			ofSetColor(c % 255, 0, 100, 200);
			start_x = start + line_size * temp->x + line_size/2;
			start_y = start + line_size * temp->y + line_size/2;
			end_x = start + line_size * temp->before->x + line_size/2;
			end_y = start + line_size * temp->before->y + line_size/2;

			ofDrawLine(start_x, start_y, end_x, end_y);
			//cout << temp->x << " " << temp->y << endl;
		
		temp = temp->before;
	}
}
// test