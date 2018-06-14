#include<FR2DCMD.h> 

////////////////////

FR_WIN_CMD win(80,160,20);

/// 2D
using namespace FR2DCMD;
FR2D_CMD fr2d(1,&win,-80,-80);
buffer B(&fr2d);

////////////////////
vertex v1,v2,v3;
void twoDinit(){
	double k=15;
	v_init(v1,-sqrt(3)*k,-1*k,1);	//triangle vertex initialization 
	v_init(v2,sqrt(3)*k,-1*k,4);
	v_init(v3,0*k,2*k,7);
}

void twoDtest(){
	fr2d.triangle(v1,v2,v3);
	
	fr2d.O_rotate(v1,0.05);
	fr2d.O_rotate(v2,0.05);
	fr2d.O_rotate(v3,0.05);
	
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
