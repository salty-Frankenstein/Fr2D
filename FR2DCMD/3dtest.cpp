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

///3D
using namespace FR3DCMD;
FR3D_CMD fr3d(&fr2d,0,0,0,50);
//vertex tv1,tv2,tv3;
buffer B(&fr3d);

////////////////////
double k=0,r=10;
int x1a=0,y1a=0; 
vertex cube[7];

void thrDinit(){
	double k=10;
	v_init(cube[0],1*k,1*k,50+1*k,5);
	v_init(cube[1],1*k,1*k,50+-1*k,5);
	v_init(cube[2],-1*k,1*k,50+1*k,5);
	v_init(cube[3],-1*k,1*k,50+-1*k,5);
	v_init(cube[4],1*k,-1*k,50+1*k,5);
	v_init(cube[5],1*k,-1*k,50+-1*k,5);
	v_init(cube[6],-1*k,-1*k,50+1*k,5);
	v_init(cube[7],-1*k,-1*k,50+-1*k,5);
	for(int i=0;i<=7;i++)
		B.add_vertex(&cube[i]);
	B.add_index(0,2,3);
	B.add_index(0,1,3);
	
	B.add_index(0,4,1);
	B.add_index(4,1,5);
	
	B.add_index(0,2,6);
	B.add_index(0,4,6);
	
	B.add_index(1,7,5);
	B.add_index(1,7,3);
	
	B.add_index(7,2,3);
	B.add_index(7,2,6);
	
	B.add_index(7,4,5);
	B.add_index(7,4,6);
	
}
void thrDtest(){
	//fr3d.triangle(tv1,tv2,tv3);
	B.render();
}

void show(FR_WIN_CMD* HWND){
	win.clear(); 
	thrDtest();
	for(int i=0;i<=7;i++)
		fr3d.x_rotate(cube[i],0.05); 
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
	
	win.run(msg,show);
	return 0;
}
