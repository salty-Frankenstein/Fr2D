#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<FR2DCMD.h> 
using namespace std;

////////////////////

FR_WIN_CMD win(80,160,20);
FR2DCMD::FR2D_CMD fr2d(1,&win,-80,-80);
FR2DCMD::vertex v1,v2,v3;

////////////////////
double k=0,r=1;
int x1=0,y1=0; 

void show(FR_WIN_CMD* HWND){
	win.clear();
	//fr2d.line_slope(k,0,3,3);
	/*
	fr2d.circle(x1,y1,r,4,1);
	fr2d.circle(x1,-y1,r,4,1);
	fr2d.circle(-x1,y1,r,4,1);
	fr2d.circle(-x1,-y1,r,4,1);
	k+=0.15;
	r++;
	x1++;
	y1++;
	*/
	//fr2d.segline(v1,v2,1); 
	//v2.y++;
	//v2.x--;
	fr2d.triangle(v1,v2,v3);
	fr2d.O_rotate(v1,0.05);
	fr2d.O_rotate(v2,0.05);
	fr2d.O_rotate(v3,0.05);
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
	system("pause");
	/*
	v1.x=5;
	v1.y=6;
	v1.colour=1;
	v2.x=35;
	v2.y=20;
	v2.colour=7;
	*/
	double t=16;
	FR2DCMD::v_init(v1,t*2,t*2,4);
	FR2DCMD::v_init(v2,t*4,t*2,2);
	FR2DCMD::v_init(v3,t*3,t*(2+sqrt(3)),6);
	win.run(msg,show);
	return 0;
}
