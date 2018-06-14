#include<FR2DCMD.h> 
#include<FR3DCMD.h> 

////////////////////

FR_WIN_CMD win(80,160,20);

/// 2D
FR2DCMD::FR2D_CMD fr2d(1,&win,-80,-80);

///3D
using namespace FR3DCMD;
FR3D_CMD fr3d(&fr2d,0,0,0,150);
buffer B(&fr3d);

////////////////////
const int cube_num=7;
const double camera_speed=3;
bool rotate=false;
struct cube{
	vertex v[8];
	double x,y,z,s;
};
cube c[cube_num];
void thrDinit(){
	double k=30,tx,ty,tz,ts;	//cube vertex initialization
	srand((unsigned)time(NULL));
	for(int i=0;i<cube_num;i++){
		k=10*rand()/double(RAND_MAX)+5;
		tx=100*rand()/double(RAND_MAX)-50;
		ty=100*rand()/double(RAND_MAX)-50;
		tz=50*rand()/double(RAND_MAX)+100;
		ts=0.4*rand()/double(RAND_MAX)-0.2;
		B.cube(c[i].v,tx,ty,tz,20-k);
		c[i].x=tx;
		c[i].y=ty;
		c[i].z=tz;
		c[i].s=ts;
	}
		
	
}
void thrDtest(){
	B.render();
	if(rotate)
	for(int i=0;i<cube_num;i++){
		fr3d.rotate_cube(c[i].s,x,c[i].v,c[i].x,c[i].y,c[i].z);
		fr3d.rotate_cube(c[i].s,y,c[i].v,c[i].x,c[i].y,c[i].z);
		fr3d.rotate_cube(c[i].s,z,c[i].v,c[i].x,c[i].y,c[i].z);
	}
		
}

void show(FR_WIN_CMD* HWND){
	win.clear(); 
	thrDtest();
}

void kup(){
	for(int i=0;i<cube_num;i++){
		for(int j=0;j<8;j++)
			c[i].v[j].y-=camera_speed;
		c[i].y-=camera_speed;
	}
}

void kdown(){
	for(int i=0;i<cube_num;i++){
		for(int j=0;j<8;j++)
			c[i].v[j].y+=camera_speed;
		c[i].y+=camera_speed;
	}
}

void kleft(){
	for(int i=0;i<cube_num;i++){
		for(int j=0;j<8;j++)
			c[i].v[j].x-=camera_speed;
		c[i].x-=camera_speed;
	}
}

void kright(){
	for(int i=0;i<cube_num;i++){
		for(int j=0;j<8;j++)
			c[i].v[j].x+=camera_speed;
		c[i].x+=camera_speed;
	}
}

bool msg(){
	char ch;
	if(kbhit()){
		ch=getch(); 
		if(ch==27)
			return true;
		else if(ch==72)
			kup();
		else if(ch==80)
			kdown();
		else if(ch==75)
			kleft();
		else if(ch==77)
			kright();
		else if(ch=='a')rotate=!rotate;
		
	}
	return false;
}

int main(){
	thrDinit();
	
	system("pause");
	win.run(msg,show);
	system("pause");
	win.clear();
	B.render();
	win.show(); 
	return 0;
}
