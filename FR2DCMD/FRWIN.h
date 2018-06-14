#ifndef FRWIN_H
#define FRWIN_H

#include<cstdio>
#include<cstdlib>
#include <conio.h>
#include<memory.h>
#include <windows.h>
#include<time.h>
#include<math.h>


//////the colour set defination
const char ColourSet[10] = { ' ','.','_','=','*','%','#','@' };//colours from 0~8

inline char Colour(int i) {
	if (i<0 || i>7)return '!';			//illeagal warning
	return ColourSet[i];			//return the no.i colour
}

#define HEI 0
#define LEN 1
//////the FR windows based on CMD class
class FR_WIN_CMD {
public:
	FR_WIN_CMD(int a, int b, int f);				//the height,length and the fps
	~FR_WIN_CMD() {};
	void run(bool(*msg)(), void(*callback)(FR_WIN_CMD* HWND));	//a function pointer to control the message loop
																//a callback fuction with a FR_WIN_CMD class handle
																//to do all rendering stuff
																//for dynamic showing
	void show();												//for stastic showing
	void putpixel(int x, int y, int i);			//put a pixel in the position (x,y) with colour i
	bool out(int x, int y);						//judging if (x,y) is out of the window
	int size(int type);							//return the size of the window
	void clear();								//clear the screen
									
private:
	char **data;	//the window
	int height;
	int length;
	int f;			//the frame per second
};

FR_WIN_CMD::FR_WIN_CMD(int a, int b, int fps = 30) {
	height = a;
	length = b;
	f = fps;
	int i, j;
	data = (char**)malloc(sizeof(char*)*a);
	for (i = 0; i<a; i++)
		data[i] = (char*)malloc(sizeof(char)*b + 1);

	for (i = 0; i<a; i++)
		for (j = 0; j <= b; j++) {
			data[i][j] = Colour(0);
			if (j == b)data[i][j] = '\0';
		}

}

void FR_WIN_CMD::run(bool(*msg)(), void(*callback)(FR_WIN_CMD* HWND)) {  
	clock_t vst, ved, start, end;
	double t;
	while (!msg()) {
		vst = clock();
		start = clock();
		system("cls");

		callback(this);
		for (int i = 0; i<height; i++)
			puts(data[i]);
		//system("pause");
		printf("FPS:%lf", t);
		while (1) {
			end = clock();
			if (end - start > 1000 / f)break;
		}
		ved = clock();
		t = 1.0 / (0.001*(ved - vst));

	}
}

void FR_WIN_CMD::show(){
	system("cls");
	for (int i = 0; i<height; i++)
		puts(data[i]);
}

void FR_WIN_CMD::putpixel(int x, int y, int i) {
	if (out(x, y))return;
	data[x][y] = Colour(i);
}

bool FR_WIN_CMD::out(int x, int y) {
	if (x<0 || x >= height || y<0 || y >= length)
		return true;
	return false;
}

int FR_WIN_CMD::size(int type) {
	if (type == HEI)return height;
	if (type == LEN)return length;
}

void FR_WIN_CMD::clear() {
	for (int i = 0; i<height; i++)
		for (int j = 0; j<length; j++)
			data[i][j] = Colour(0);
}
//////end of the FR windows based on CMD class

#endif
