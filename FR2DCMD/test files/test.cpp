#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<FR2DCMD.h> 
#include<FR3DCMD.h> 
using namespace std;

////////////////////

FR_WIN_CMD win(80,160,20);

/// 2D
FR2DCMD::FR2D_CMD fr2d(1,&win,-80,-80);
FR2DCMD::vertex v1,v2,v3;
FR2DCMD::buffer B(&fr2d);

///3D
FR3DCMD::FR3D_CMD fr3d(&fr2d,0,0,0,5);
FR3DCMD::vertex tv1,tv2,tv3;
FR3DCMD::buffer tB(&fr3d);

////////////////////
double k=0,r=10;
int x1a=0,y1a=0; 
FR2DCMD::vertex vcir;
FR3DCMD::vertex cube[7];
void twoDinit(){
	double t=16;
	FR2DCMD::v_init(vcir,0,0,4);
	FR2DCMD::v_init(v1,t*2,t*2,4);
	FR2DCMD::v_init(v2,t*4,t*2,2);
	FR2DCMD::v_init(v3,t*3,t*(2+sqrt(3)),6);
	
	B.add_vertex(&v1);
	B.add_vertex(&v2);
	B.add_vertex(&v3);
	B.add_vertex(&vcir);
	B.add_index(0,1,2);
	B.add_index(0,3,2);
	B.add_index(1,2,3);
}

void thrDinit(){
	double t=16;
	//FR3DCMD::v_init(vcir,0,0,4);
	FR3DCMD::v_init(tv1,t*2,t*2,5,4);
	FR3DCMD::v_init(tv2,t*4,t*2,6,2);
	FR3DCMD::v_init(tv3,t*3,t*(2+sqrt(3)),7,6);
	

	
	
	
}
void thrDtest(){
	tv1.z+=0.1;
	tv2.z+=0.1;
	tv3.z+=0.1;
	fr3d.triangle(tv1,tv2,tv3);
	//tB.render();
}
void twoDtest(){
	fr2d.line_slope(k,0,3,1);
	k+=0.05;
	fr2d.circle(vcir.x,vcir.y,r,3,1);
	//fr2d.circle(vcir.x,-vcir.y,r,4,1);
	//fr2d.circle(-vcir.x,vcir.y,r,5,1);
	fr2d.circle(-vcir.x,-vcir.y,r,6,1);

	r+=0.4;
	vcir.x++;
	vcir.y++;
	fr2d.O_rotate(vcir,0.1);
	
	//fr2d.segline(v1,v2,1); 
	//v2.y++;
	//v2.x--;
	
	//fr2d.triangle(v1,v2,v3);
	B.render();
	fr2d.O_rotate(v1,0.03);
	fr2d.O_rotate(v2,-0.05);
	fr2d.O_rotate(v3,0.04);
}

void show(FR_WIN_CMD* HWND){
	win.clear(); 
	//twoDtest();
	thrDtest();
}

bool msg(){
	char ch;
	if(kbhit()){
		ch=getch();
		if(27==ch)
		return true;
	}
	return false;
}

int main(){
	thrDinit();
	system("pause");
	twoDinit();
	
	win.run(msg,show);
	return 0;
}
