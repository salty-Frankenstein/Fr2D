#ifndef FR2DCMD_H
#define FR2DCMD_H

#include<cstdio>
#include<cstdlib>
#include <conio.h>
#include<memory.h>
#include <windows.h>
#include<time.h>
#include<math.h>

// FRSL \\

#include<matrix.h>	
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

		printf("FPS:%lf", t);
		while (1) {
			end = clock();
			if (end - start > 1000 / f)break;
		}
		ved = clock();
		t = 1.0 / (0.001*(ved - vst));

	}
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

namespace FR2DCMD{
	struct vertex {
		double x, y;
		int colour;
	};

	class FR2D_CMD {
	public:
		FR2D_CMD(int rate, FR_WIN_CMD *H, int x = 0, int y = 0) {
			HWND = H;
			height = HWND->size(HEI);
			length = HWND->size(LEN);
			screen_rate = rate;
			ox = x;
			oy = y;
		}
		~FR2D_CMD() {}
		bool in(double x, double y, double diff) {
			if (abs(x - y)<diff)return true;
			return false;
		}
		void line_slope(double a, double b, int colour, int pensize);		//draw a line y=ax+b with colour i
		void segline(vertex a, vertex b, int pensize);						//draw a segline from a to b with colour i
		void circle(double a, double b, double r, int colour, int pensize);	//draw a circle (x-a)^2+(y-b)^2=r^2
	private:
		double screen_rate;
		double x, y;
		double ox, oy;			//the coordinate of the origin  
		int height;
		int length;
		FR_WIN_CMD *HWND;
		void trans_pixel(int i, int j) {
			y = screen_rate * 2 * (height - i) + ox;
			x = screen_rate * j + oy;
		}
		double trans_x(int j) {
			return screen_rate * j + oy;
		}
		double trans_y(int i) {
			return screen_rate * 2 * (height - i) + ox;
		}
	};
	void FR2D_CMD::line_slope(double a, double b, int colour, int pensize = 1) {
		int i, j;
		for (i = 0; i <= height; i++)
			for (j = 0; j <= length; j++) {
				trans_pixel(i, j);
				if (in(a*x + b, y, pensize))HWND->putpixel(i, j, colour);
			}
	}
	void FR2D_CMD::segline(vertex a, vertex b, int pensize = 1) {		//draw a segline from a to b with colour i
		double rate_a, rate_b, bx, sx, by, sy;
		bx = std::max(a.x, b.x);
		sx = std::min(a.x, b.x);
		by = std::max(a.y, b.y);
		sy = std::min(a.y, b.y);
		int i, j;
		int colour;
		for (i = 0; i <= height; i++)
			for (j = 0; j <= length; j++) {
				trans_pixel(i, j);
				if (x>bx || x<sx || y>by || y<sy)continue;

				if (in(((x - a.x)*(b.y - a.y)), ((y - a.y)*(b.x - a.x)), pensize)) {
					if (b.x != a.x) {
						rate_a = fabs((x - a.x) / (b.x - a.x));
						rate_b = fabs((b.x - x) / (b.x - a.x));
					}
					else {
						rate_a = fabs((y - a.y) / (b.y - a.y));
						rate_b = fabs((b.y - y) / (b.y - a.y));
					}
					colour = rate_a * a.colour + rate_b * b.colour;
					HWND->putpixel(i, j, colour);
				}
			}
	}
	void FR2D_CMD::circle(double a, double b, double r, int colour, int pensize = 1) {
		int i, j;
		for (i = 0; i <= height; i++)
			for (j = 0; j <= length; j++) {
				trans_pixel(i, j);
				if (in((x - a)*(x - a) + (y - b)*(y - b), r*r, pensize))HWND->putpixel(i, j, colour);
			}
	}
}
#endif
