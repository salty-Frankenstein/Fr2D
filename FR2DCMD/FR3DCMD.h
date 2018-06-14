#ifndef FR3DCMD_H
#define FR3DCMD_H
#include<vector>

#include<FRWIN.h>
#include<FR2DCMD.h>
// FRSL
#include<matrix.h>	

namespace FR3DCMD {

	struct vertex {
		double x, y, z;
		int colour;
	};

	void v_init(vertex &v1, double x, double y, double z, int colour) {
		v1.x = x;
		v1.y = y;
		v1.z = z;
		v1.colour = colour;
	}

	enum axis {x,y,z};

	class FR3D_CMD {
	public:
		FR3D_CMD(FR2DCMD::FR2D_CMD *H, double x = 0, double y = 0, double z = 0, double dist = 100) {
			F2DH = H;
			ox = x;
			oy = y;
			oz = z;
			d = dist;
		}
		~FR3D_CMD() {}

		void triangle(vertex v1, vertex v2, vertex v3) {
			FR2DCMD::vertex v2d1 = trans_2d(v1), v2d2 = trans_2d(v2), v2d3 = trans_2d(v3);
			F2DH->triangle(v2d1, v2d2, v2d3);
		}

		FR2DCMD::vertex trans_2d(vertex ori) {
			matrix o(1, 4), trans1(4, 4), trans2(4, 4);
			o.elex(0, 0, ori.x);			o.elex(0, 1, ori.y);			o.elex(0, 2, ori.z);			o.elex(0, 3, 1);
			trans1.elex(0, 0, 1);			trans1.elex(0, 1, 0);			trans1.elex(0, 2, 0);			trans1.elex(0, 3, 0);
			trans1.elex(1, 0, 0);			trans1.elex(1, 1, 1);			trans1.elex(1, 2, 0);			trans1.elex(1, 3, 0);
			trans1.elex(2, 0, 0);			trans1.elex(2, 1, 0);			trans1.elex(2, 2, 1);			trans1.elex(2, 3, 1 / (d));
			trans1.elex(3, 0, 0);			trans1.elex(3, 1, 0);			trans1.elex(3, 2, 0);			trans1.elex(3, 3, 0);

			matrix ans = o * trans1;
			FR2DCMD::vertex t;

			double dt = ans.ele(0, 3);
			ans.elex(0, 0, ans.ele(0, 0) / dt);
			ans.elex(0, 1, ans.ele(0, 1) / dt);
			t.x = ans.ele(0, 0);
			t.y = ans.ele(0, 1);
			t.colour = ori.colour;
			return t;
		}

		void x_rotate(vertex &ori, double angle) {
			matrix o(1, 4), trans(4, 4);
			double  sa = sin(angle), ca = cos(angle);
			trans.elex(0, 0, 1);			trans.elex(0, 1, 0);			trans.elex(0, 2, 0);			trans.elex(0, 3, 0);
			trans.elex(1, 0, 0);			trans.elex(1, 1, ca);			trans.elex(1, 2, -sa);			trans.elex(1, 3, 0);
			trans.elex(2, 0, 0);			trans.elex(2, 1, sa);			trans.elex(2, 2, ca);			trans.elex(2, 3, 0);
			trans.elex(3, 0, 0);			trans.elex(3, 1, 0);			trans.elex(3, 2, 0);			trans.elex(3, 3, 1);
			o.elex(0, 0, ori.x);			o.elex(0, 1, ori.y);			o.elex(0, 2, ori.z);			o.elex(0, 3, 1);

			matrix ans = o * trans;
			ori.x = ans.ele(0, 0);
			ori.y = ans.ele(0, 1);
			ori.z = ans.ele(0, 2);
		}

		void y_rotate(vertex &ori, double angle) {
			matrix o(1, 4), trans(4, 4);
			double  sa = sin(angle), ca = cos(angle);
			trans.elex(0, 0, ca);			trans.elex(0, 1, 0);			trans.elex(0, 2, -sa);			trans.elex(0, 3, 0);
			trans.elex(1, 0, 0);			trans.elex(1, 1, 1);			trans.elex(1, 2, 0);			trans.elex(1, 3, 0);
			trans.elex(2, 0, sa);			trans.elex(2, 1, 0);			trans.elex(2, 2, ca);			trans.elex(2, 3, 0);
			trans.elex(3, 0, 0);			trans.elex(3, 1, 0);			trans.elex(3, 2, 0);			trans.elex(3, 3, 0);
			o.elex(0, 0, ori.x);			o.elex(0, 1, ori.y);			o.elex(0, 2, ori.z);			o.elex(0, 3, 1);

			matrix ans = o * trans;
			ori.x = ans.ele(0, 0);
			ori.y = ans.ele(0, 1);
			ori.z = ans.ele(0, 2);
		}

		void z_rotate(vertex &ori, double angle) {
			matrix o(1, 4), trans(4, 4);
			double  sa = sin(angle), ca = cos(angle);
			trans.elex(0, 0, ca);			trans.elex(0, 1, sa);			trans.elex(0, 2, 0);			trans.elex(0, 3, 0);
			trans.elex(1, 0, -sa);			trans.elex(1, 1, ca);			trans.elex(1, 2, 0);			trans.elex(1, 3, 0);
			trans.elex(2, 0, 0);			trans.elex(2, 1, 0);			trans.elex(2, 2, 1);			trans.elex(2, 3, 0);
			trans.elex(3, 0, 0);			trans.elex(3, 1, 0);			trans.elex(3, 2, 0);			trans.elex(3, 3, 1);
			o.elex(0, 0, ori.x);			o.elex(0, 1, ori.y);			o.elex(0, 2, ori.z);			o.elex(0, 3, 1);

			matrix ans = o * trans;
			ori.x = ans.ele(0, 0);
			ori.y = ans.ele(0, 1);
			ori.z = ans.ele(0, 2);
		}

		void rotate_cube(double angle, axis t, vertex v[], double tx,double ty,double tz) {
			
			//for (int i = 0; i < 8; i++)
			switch (t) {

			case x: {
				for (int i = 0; i < 8; i++) {
					v[i].y -= ty;
					v[i].z -= tz;
					x_rotate(v[i], angle);
					v[i].y += ty;
					v[i].z += tz;
				}
					
				break;
			}

			case y: {
				for (int i = 0; i < 8; i++) {
					v[i].z -= tz;
					v[i].x -= tx;
					y_rotate(v[i], angle);
					v[i].z += tz;
					v[i].x += tx;
				}
				break;
			}

			case z: {
				for (int i = 0; i < 8; i++) {
					v[i].y -= ty;
					v[i].x -= tx;
					z_rotate(v[i], angle);
					v[i].y += ty;
					v[i].x += tx;
				}
				break;
			}

			}
		}

	private:
		FR2DCMD::FR2D_CMD * F2DH;
		double ox, oy, oz;
		double d;
	};

	//////end of FR3D_CMD class

	class buffer {

	public:

		buffer(FR3D_CMD *H) {
			num = 0;
			index_ed = 0;
			HND = H;
		}
		~buffer() {}

		void render() {
			vertex vt[3];
			int time = 0;
			for (int i = 0; i<index_ed; i++) {
				vt[time] = *v[index[i]];
				time++;
				if (time == 3) {
					time = 0;
					HND->triangle(vt[0], vt[1], vt[2]);
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

		void cube(vertex cube[], double x, double y, double z, double size) {	//return a vertex buffer of a cube
			v_init(cube[0], 1 * size, 1 * size, 1 * size, 1);
			v_init(cube[1], 1 * size, 1 * size, -1 * size, 2);
			v_init(cube[2], -1 * size, 1 * size, 1 * size, 3);
			v_init(cube[3], -1 * size, 1 * size, -1 * size, 4);
			v_init(cube[4], 1 * size, -1 * size, 1 * size, 5);
			v_init(cube[5], 1 * size, -1 * size, -1 * size, 6);
			v_init(cube[6], -1 * size, -1 * size, 1 * size, 7);
			v_init(cube[7], -1 * size, -1 * size, -1 * size, 0);	//cube vertex initialization

			this->add_index(num + 0, num + 2, num + 3);
			this->add_index(num + 0, num + 1, num + 3);

			this->add_index(num + 0, num + 4, num + 1);
			this->add_index(num + 4, num + 1, num + 5);

			this->add_index(num + 0, num + 2, num + 6);
			this->add_index(num + 0, num + 4, num + 6);

			this->add_index(num + 1, num + 7, num + 5);
			this->add_index(num + 1, num + 7, num + 3);

			this->add_index(num + 7, num + 2, num + 3);
			this->add_index(num + 7, num + 2, num + 6);

			this->add_index(num + 7, num + 4, num + 5);
			this->add_index(num + 7, num + 4, num + 6);				//build the vertex buffer


			for (int i = 0; i <= 7; i++)
				this->add_vertex(&cube[i]);			//apply the vertex into the buffer

			for (int i = 0; i <= 7; i++) {
				cube[i].x += x;
				cube[i].y += y;
				cube[i].z += z;
			}

		}

	private:

		std::vector<vertex*> v;
		std::vector<int> index;
		int num;
		int index_ed;
		FR3D_CMD *HND;
	};
}
#endif
