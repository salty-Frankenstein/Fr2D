#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<FR2DCMD.h> 
using namespace std;

////////////////////

FR_WIN_CMD win(80,160,20);

/// 2D
using namespace FR2DCMD;
FR2D_CMD fr2d(1,&win,-80,-80);
//FR2D_CMD fr2d(1,&win);
buffer B(&fr2d);

////////////////////
vertex v[10000];
vertex vt1,vt2,vt3;
void twoDinit(){
	int i;
	for(i=1;i<=5000;i++){
		if(i%2)v_init(v[i],0,0,1);
		else  v_init(v[i],100,i/6,7);
	}
	v_init(vt1,0,0,1);
	v_init(vt2,-100,100,7);
	v_init(vt3,0,10,7);
	//B.add_vertex(&v1);
	//B.add_index(0,1,2);
}

void twoDtest(){
	//B.render();
	int i;
	//fr2d.segline2(vt1,vt2,1);
	//fr2d.segline(vt1,vt2,1);
	//fr2d.segline2(vt1,vt3,1);
	//fr2d.O_rotate(vt1,0.05);
	//fr2d.O_rotate(vt2,0.03);
	i=1;
	//fr2d.segline(v[i],v[i+1],1);
	//fr2d.O_rotate(v[i+1],0.03);
	
	for(i=1;i<=4000;i+=2)
		fr2d.segline(v[i],v[i+1],1);
	for(i=1;i<=4000;i++)
		fr2d.O_rotate(v[i],0.03);
		
		
}

void show(FR_WIN_CMD* HWND){
	win.clear();
	twoDtest();
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
	twoDinit();
	win.run(msg,show);
	return 0;
}
