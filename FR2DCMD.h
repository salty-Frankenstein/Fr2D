#ifndef FR2DCMD_H
#define FR2DCMD_H

#include<FRWIN.h>
// FRSL \\

#include<matrix.h>	
namespace FR2DCMD {
	struct vertex {
		double x, y;
		int colour;
	};
	void v_init(vertex &v1, double x, double y, int colour) {
		v1.x = x;
		v1.y = y;
		v1.colour = colour;
	}
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
		void triangle(vertex v1, vertex v2, vertex v3) {
			segline(v1, v2, 1);
			segline(v2, v3, 1);
			segline(v1, v3, 1);
		}
		void O_rotate(vertex &ori, double angle);
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
		double y1;
		for (i = 0; i <= height; i++)
			for (j = 0; j <= length; j++) {
				trans_pixel(i, j);
				if (x>bx || x<sx || y>by || y<sy)continue;
				y1 = (x - a.x)*(b.y - a.y) / (b.x - a.x) + a.y;
				if (in(y, y1, pensize)) {
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
		double y1, y2, t;
		for (i = 0; i <= height; i++)
			for (j = 0; j <= length; j++) {
				trans_pixel(i, j);
				t = r * r - (x - a)*(x - a);
				if (t < 0)continue;
				y1 = sqrt(t) + b;
				y2 = -sqrt(t) + b;

				if (in(y, y1, pensize) || in(y, y2, pensize))HWND->putpixel(i, j, colour);
			}
	}

	void FR2D_CMD::O_rotate(vertex &ori, double angle) {
		matrix o(1, 2), trans(2, 2);
		double  sa = sin(angle), ca = cos(angle);
		trans.elex(0, 0, ca);
		trans.elex(0, 1, -sa);
		trans.elex(1, 0, sa);
		trans.elex(1, 1, ca);
		o.elex(0, 0, ori.x);
		o.elex(0, 1, ori.y);
		matrix ans = o * trans;
		ori.x = ans.ele(0, 0);
		ori.y = ans.ele(0, 1);
	}
}


#endif