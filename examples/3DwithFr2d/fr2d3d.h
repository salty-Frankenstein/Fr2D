#pragma once
#include "fr2d.h"
#include <vector>
#include <d2d1_1helper.h>

enum Axis { X, Y, Z };

void v_init(D2D_VECTOR_3F &v1, float x, float y, float z) {
	v1.x = x;
	v1.y = y;
	v1.z = z;
}

class Fr2D_3D {
public:
	Fr2D_3D(Fr2D* h, float x = 0, float y = 0, float z = 0, float dist = 100) {
		ox = x;
		oy = y;
		oz = z;
		d = dist;
		hdl = h;
	}

	void DrawTriangle(Fr2DBrush &fr2dbrush, D2D_VECTOR_3F v1, D2D_VECTOR_3F v2, D2D_VECTOR_3F v3) {
		D2D_VECTOR_3F v2d1 = Trans2D(v1), v2d2 = Trans2D(v2), v2d3 = Trans2D(v3);
		hdl->DrawTriangle(fr2dbrush, v2d1.x, v2d1.y, v2d2.x, v2d2.y, v2d3.x, v2d3.y);
	}

	D2D_VECTOR_3F Trans2D(D2D_VECTOR_3F ori) {
		D2D1::Matrix4x4F o(ori.x, ori.y, ori.z, 1,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0

		);
		D2D1::Matrix4x4F trans(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 1, 1, 1 / d,
			0, 0, 0, 0
		);
		D2D1::Matrix4x4F ans = o * trans;
		D2D_VECTOR_3F t;
		float dt = ans._14;
		ans._11 /= dt;
		ans._12 /= dt;
		t.x = ans._11 + 400;
		t.y = ans._12 - 1200;
		return t;
	}

	void XRotate(D2D_VECTOR_3F &ori, float angle) {
		float  sa = sin(angle), ca = cos(angle);
		D2D1::Matrix4x4F o(ori.x, ori.y, ori.z, 1,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);
		D2D1::Matrix4x4F trans(1, 0, 0, 0,
			0, ca, -sa, 0,
			0, sa, ca, 0,
			0, 0, 0, 1
		);
		D2D1::Matrix4x4F ans = o * trans;
		ori.x = ans._11;
		ori.y = ans._12;
		ori.z = ans._13;
	}

	void YRotate(D2D_VECTOR_3F &ori, float angle) {
		float  sa = sin(angle), ca = cos(angle);
		D2D1::Matrix4x4F o(ori.x, ori.y, ori.z, 1,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);
		D2D1::Matrix4x4F trans(ca, 0, -sa, 0,
			0, 1, 0, 0,
			sa, 0, ca, 0,
			0, 0, 0, 0
		);
		D2D1::Matrix4x4F ans = o * trans;
		ori.x = ans._11;
		ori.y = ans._12;
		ori.z = ans._13;
	}

	void RotateCube(Axis t, float angle, D2D_VECTOR_3F v[], float tx, float ty, float tz) {
		switch (t) {
		case X:
			for (int i = 0; i < 8; i++) {
				v[i].y -= ty;
				v[i].z -= tz;
				XRotate(v[i], angle);
				v[i].y += ty;
				v[i].z += tz;
			}
			break;
		case Y:
			for (int i = 0; i < 8; i++) {
				v[i].z -= tz;
				v[i].x -= tx;
				YRotate(v[i], angle);
				v[i].z += tz;
				v[i].x += tx;
			}
			break;
		case Z:
			break;
		}
	}
private:
	Fr2D * hdl;
	float ox, oy, oz;
	float d;
};

class Buffer {
public:
	Buffer(Fr2D_3D* h) {
		num = 0;
		indexEd = 0;
		hdl = h;
	}

	void Render(Fr2DBrush fr2dbrush) {
		D2D_VECTOR_3F vt[3];
		int time = 0;
		for (int i = 0; i<indexEd; i++) {
			vt[time] = *v[index[i]];
			time++;
			if (time == 3) {
				time = 0;
				hdl->DrawTriangle(fr2dbrush, vt[0], vt[1], vt[2]);
			}
		}
	}

	void add_vertex(D2D_VECTOR_3F *vt) {
		v.push_back(vt);
		num++;
	}

	void add_index(int i, int j, int k) {
		index.push_back(i);
		index.push_back(j);
		index.push_back(k);
		indexEd += 3;
	}

	void Cube(D2D_VECTOR_3F cube[], float x, float y, float z, float size) {	//return a vertex buffer of a cube
		v_init(cube[0], 1 * size, 1 * size, 1 * size);
		v_init(cube[1], 1 * size, 1 * size, -1 * size);
		v_init(cube[2], -1 * size, 1 * size, 1 * size);
		v_init(cube[3], -1 * size, 1 * size, -1 * size);
		v_init(cube[4], 1 * size, -1 * size, 1 * size);
		v_init(cube[5], 1 * size, -1 * size, -1 * size);
		v_init(cube[6], -1 * size, -1 * size, 1 * size);
		v_init(cube[7], -1 * size, -1 * size, -1 * size);	//cube vertex initialization

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
	std::vector<D2D_VECTOR_3F*> v;
	std::vector<int> index;
	int num;
	int indexEd;
	Fr2D_3D *hdl;
};

struct Cube {
	D2D_VECTOR_3F v[8];
	double x, y, z, s;
};
