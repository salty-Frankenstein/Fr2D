#ifndef FR2DCMD_H
#define FR2DCMD_H

#include<cstdio>
#include<cstdlib>
#include <conio.h>
#include<memory.h>
#include <windows.h>
#include<time.h>
#include<math.h>
//////the colour set defination
const char ColourSet[10]={' ','.','_','=','*','%','#','@'};//colours from 0~8

inline char Colour(int i){
	if(i<0||i>7)return '!';			//illeagal warning
	return ColourSet[i];			//return the no.i colour
} 

#define HEI 0
#define LEN 1
//////the FR windows based on CMD class
class FR_WIN_CMD{
	public:
		FR_WIN_CMD(int a,int b,int f);				//the height,length and the fps
		~FR_WIN_CMD(){};
		void run(bool (*msg)(),void (*callback)(FR_WIN_CMD* HWND));	//a function pointer to control the message loop
													//a callback fuction with a FR_WIN_CMD class handle
													//to do all rendering stuff
		void putpixel(int x,int y,int i);			//put a pixel in the position (x,y) with colour i
		bool out(int x,int y);						//judging if (x,y) is out of the window
		int size(int type);							//return the size of the window
	private:
		char **data;	//the window
		int height;		
		int length;
		int f;			//the frame per second
};

FR_WIN_CMD::FR_WIN_CMD(int a,int b,int fps=30){
	height=a;
	length=b;  
	f=fps;
	int i,j;
    data=(char**)malloc(sizeof(char*)*a);    
    for(i=0;i<a;i++)    
    	data[i]=(char*)malloc(sizeof(char)*b+1);
    
    for(i=0;i<a;i++)
    	for(j=0;j<=b;j++){
    		data[i][j]=Colour(0); 
    		if(j==b)data[i][j]='\0'; 
		}
    	
}

void FR_WIN_CMD::run(bool (*msg)(),void (*callback)(FR_WIN_CMD* HWND)){
	clock_t vst,ved,start,end;
	double t;
	while(!msg()){
		vst=clock();
		start=clock();
		system("cls");
		
		callback(this);
		for(int i=0;i<height;i++)
			puts(data[i]);
		
		//printf("%d",vst-ved);
		//system("pause");
		printf("FPS:%lf",t);
		while (1) {
			end=clock();
			if (end - start > 1000/f)break;
		}
		ved=clock();
		t=1.0/(0.001*(ved-vst));
		
		//Sleep(10);
		
		
	}
}

void FR_WIN_CMD::putpixel(int x,int y,int i){
	if(out(x,y))return;
	data[x][y]=Colour(i);
}

bool FR_WIN_CMD::out(int x,int y){
	if(x<0||x>=height||y<0||y>=length)
		return true;
	return false;
}

int FR_WIN_CMD::size(int type){
	if(type==HEI)return height;
	if(type==LEN)return length;
}
//////end of the FR windows based on CMD class

namespace FR2D_CMD{
	bool in(double x,double y){
		if(abs(x-y)<1)return true;
		return false;
	}
	void line_slope(double a,double b,int colour,FR_WIN_CMD* HWND){	//draw a line y=ax+b with colour i
		int i,j;
		
		for(i=0;i<=HWND->size(HEI);i++)
			for(j=0;j<=HWND->size(LEN);j++)
				if(in(a*i+b,j))HWND->putpixel(i,j,colour);
		
	}
	void line(double a,double b,int colour,FR_WIN_CMD* HWND){	//draw a line y=kx+b with colour i
	}
	void circle(double a,double b,double r,int colour,FR_WIN_CMD* HWND){//draw a circle (x-a)^2+(y-b)^2=r^2
		int i,j;
		for(i=0;i<=HWND->size(HEI);i++)
			for(j=0;j<=HWND->size(LEN);j++)
				if(in((i-a)*(i-a)+(j-b)*(j-b),r*r))HWND->putpixel(i,j,colour);
		
	}
}

#endif
