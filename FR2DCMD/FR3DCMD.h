#ifndef FR3DCMD_H
#define FR3DCMD_H
#include<vector>

#include<FRWIN.h>
#include<FR2DCMD.h>
// FRSL
#include<matrix.h>	

namespace FR3DCMD{

	struct vertex {
		double x, y, z;
		int colour;
	};

	void v_init(vertex &v1, double x, double y, double z,int colour) {
		v1.x = x;
		v1.y = y;
		v1.z = z;
		v1.colour = colour;
	}
	class FR3D_CMD {
	public:
		FR3D_CMD(FR2DCMD::FR2D_CMD *H, double x = 0, double y = 0, double z=0,double dist=100) {
			F2DH = H;
			ox = x;
			oy = y;
			oz = z;
			d = dist;
		}
		~FR3D_CMD() {}

		void triangle(vertex v1, vertex v2, vertex v3) {
			FR2DCMD::vertex v2d1=trans_2d(v1), v2d2 = trans_2d(v2), v2d3 = trans_2d(v3);
			/*
			FR2DCMD::v_init(v2d1, v1.x, v1.y, v1.colour);
			FR2DCMD::v_init(v2d2, v2.x, v2.y, v2.colour);
			FR2DCMD::v_init(v2d3, v3.x, v3.y, v3.colour);
			*/
			/*
			printf("%lf %lf\n", v1.x, v1.y);
			printf("%lf %lf\n", v2.x, v2.y);
			printf("%lf %lf\n", v3.x, v3.y);
			printf("%lf \n%lf %lf\n",d, v2d1.x, v2d1.y);
			printf("%lf %lf\n", v2d2.x, v2d2.y);
			printf("%lf %lf\n", v2d3.x, v2d3.y);
			system("pause");
			*/
			F2DH->triangle(v2d1, v2d2, v2d3);
		}

		FR2DCMD::vertex trans_2d(vertex ori) {
			matrix o(1,4), trans1(4,4),trans2(4,4);
			o.elex(0, 0, ori.x);			o.elex(0, 1, ori.y);			o.elex(0, 2, ori.z);			o.elex(0, 3, 1);
			trans1.elex(0, 0, 1);			trans1.elex(0, 1, 0);			trans1.elex(0, 2, 0);			trans1.elex(0, 3, 0);
			trans1.elex(1, 0, 0);			trans1.elex(1, 1, 1);			trans1.elex(1, 2, 0);			trans1.elex(1, 3, 0);
			trans1.elex(2, 0, 0);			trans1.elex(2, 1, 0);			trans1.elex(2, 2, 1);			trans1.elex(2, 3, 1/d);
			trans1.elex(3, 0, 0);			trans1.elex(3, 1, 0);			trans1.elex(3, 2, 0);			trans1.elex(3, 3, 0);
			/*
			trans2.elex(0, 0, 1);			trans2.elex(0, 1, 0);			trans2.elex(0, 2, 0);			trans2.elex(0, 3, 0);
			trans2.elex(1, 0, 0);			trans2.elex(1, 1, 1);			trans2.elex(1, 2, 0);			trans2.elex(1, 3, 0);
			trans2.elex(2, 0, 0);			trans2.elex(2, 1, 0);			trans2.elex(2, 2, 0);			trans2.elex(2, 3, 0);
			trans2.elex(3, 0, 0);			trans2.elex(3, 1, 0);			trans2.elex(3, 2, 0);			trans2.elex(3, 3, 1);
			*/
			matrix ans = o * trans1;
			FR2DCMD::vertex t;
			//ans = ans * trans2;
			double dt=ans.ele(0,3);
			ans.elex(0, 0, ans.ele(0,0)/dt);
			ans.elex(0, 1, ans.ele(0, 1)/dt);
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
			o.elex(0, 0, ori.x);			o.elex(0, 1, ori.y);			o.elex(0, 2, ori.z);			o.elex(0, 3,1);

			matrix ans = o * trans;
			ori.x = ans.ele(0, 0);
			ori.y = ans.ele(0, 1);
			ori.z = ans.ele(0, 2);
		}
	private:
		FR2DCMD::FR2D_CMD * F2DH;
		double ox, oy,oz;
		double d;
	};

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

	private:

		std::vector<vertex*> v;
		std::vector<int> index;
		int num;
		int index_ed;
		FR3D_CMD *HND;
	};
}
#endif
