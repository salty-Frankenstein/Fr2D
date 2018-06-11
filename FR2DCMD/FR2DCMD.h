#ifndef FR2DCMD_H
#define FR2DCMD_H
#include<vector>

#include<FRWIN.h>
// FRSL

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

		FR2D_CMD(int rate, FR_WIN_CMD *H, double x = 0, double y = 0) {
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
		void circle(vertex v, double r, int pensize);							//draw a circle inited with a vertex
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
		int trans_x(int x) {
			double j = (x-oy)/screen_rate;
			int t = (int)j;
			if (j - t > 0.5)return t + 1;
			return t;
		}
		double trans_y(int y) {
			double i = (ox-y)/(2* screen_rate) +height;
			int t = (int)i;
			if (i - t > 0.5)return t + 1;
			return t;
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
		double k,y1, rate_a, rate_b, bx, sx, by, sy;
		bx = std::max(a.x, b.x);
		sx = std::min(a.x, b.x);
		by = std::max(a.y, b.y);
		sy = std::min(a.y, b.y);
		
		int i, j,colour;
		if (a.x - b.x != 0) {
			k = (a.y - b.y) / (a.x - b.x);
			trans_pixel(0, 0);
			if (x < sx)x = sx;
			y1 = ((x-a.x)*(b.y - a.y)) / (b.x - a.x) + a.y; 
			for (j = 0; j < length; j++) {
				trans_pixel(i, j);
				if (x>bx || x<sx || y1>by || y1<sy)continue;
				rate_a = fabs((x - a.x) / (b.x - a.x));
				rate_b = fabs((b.x - x) / (b.x - a.x));
				colour = rate_b * a.colour + rate_a * b.colour;
				HWND->putpixel(trans_y(y1), trans_x(x), colour);
				y1 += k;
			}

		}
		else {
			for (i = 0; i < height; i++) {
				trans_pixel(i, j);

				if (y>by || y<sy)continue;
				rate_a = fabs((y - a.y) / (b.y - a.y));
				rate_b = fabs((b.y - y) / (b.y - a.y));
				colour = rate_b * a.colour + rate_a * b.colour;
				HWND->putpixel(trans_y(y), trans_x(a.x), colour);
			}
		}

	}
	void FR2D_CMD::circle(double a, double b, double r, int colour, int pensize = 1) {
		int i, j;
		double y1, y2, t, st;
		for (i = 0; i <= height; i++)
			for (j = 0; j <= length; j++) {
				trans_pixel(i, j);
				t = r * r - (x - a)*(x - a);
				if (t < 0)continue;
				st = sqrt(t);
				y1 = st + b;
				y2 = -st + b;

				if (in(y, y1, pensize) || in(y, y2, pensize))HWND->putpixel(i, j, colour);
			}
	}

	void FR2D_CMD::circle(vertex v, double r,  int pensize = 1) {
		int i, j,colour=v.colour;
		double y1, y2, t,a=v.x,b=v.y;
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
	//////end of FR2D_CMD class

	class buffer {

	public:

		buffer(FR2D_CMD *H) {
			num = 0;
			index_ed = 0;
			HND = H;
		}
		~buffer() {}

		void render() {
			//printf("%d %d", num, index_ed);
			//system("pause");

			vertex vt[3];
			int time = 0;
			for (int i = 0; i<index_ed; i++) {
				vt[time] =* v[index[i]];
				time++;
				if (time == 3) {
					time = 0;
					HND->triangle(vt[0],vt[1],vt[2]);
					//printf("%d %d %d", vt[0].colour, vt[1].colour, vt[2].colour);
					//system("pause");
				}
			}
		}

		void add_vertex(vertex *vt) {
			v.push_back(vt);
			num++;
		}

		void add_index(int i, int j, int k) {
			index.push_back(i);
			index.push_back(j);
			index.push_back(k);
			index_ed += 3;
		}

	private:
		std::vector<vertex*> v;
		std::vector<int> index;
		int num;
		int index_ed;
		FR2D_CMD *HND;
	};

}


#endif