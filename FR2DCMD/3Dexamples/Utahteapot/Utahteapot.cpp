#include<FR2DCMD.h> 
#include<FR3DCMD.h> 

////////////////////

FR_WIN_CMD win(80,180,8);

/// 2D
FR2DCMD::FR2D_CMD fr2d(1,&win,-80,-80);

///	3D

using namespace FR3DCMD;
FR3D_CMD fr3d(&fr2d,0,0,0,150);
buffer B(&fr3d);
vertex pot[9500*3];
////////////////////

FILE *fin=fopen("utahteapot.stl","rb");

void thrDinit(){	//read the utah teapot model 
	
	char waste[85];
	int n[10],p=0,colour=1;
	float t[5],k=1.8,s=0.1;	//s:rate
	fseek(fin, SEEK_SET, 0);
	
   	fread(waste, 1, 80, fin);
   	
	for(int i=0;i<80;i++)
		printf("%c",waste[i]);
		
	fread(n, 4, 1, fin);
	printf("\n %d\n",n[0]);
	printf("loading:");
	for(int i=0;i<n[0];i++){
		fread(t, 4, 3, fin);	//waste
		colour=8-(7*i/n[0]+1);
		fread(t, 4, 3, fin);	//the 1st vertex
		
		v_init(pot[p],s*t[0],s*t[1],s*t[2]+k,colour);
		B.add_vertex(&pot[p]);	//apply it into the vertex buffer
		p++;
		
		fread(t, 4, 3, fin);	//the 2nd vertex
		v_init(pot[p],s*t[0],s*t[1],s*t[2]+k,colour);
		B.add_vertex(&pot[p]);
		p++;
		
		fread(t, 4, 3, fin);	//the 3rd vertex
		v_init(pot[p],s*t[0],s*t[1],s*t[2]+k,colour);
		B.add_vertex(&pot[p]);
		p++;
		
		B.add_index(p-1,p-2,p-3);	//build 
		
		fread(waste, 1, 2, fin);	//waste
		if(i%100==0)printf(".");
	}
	fclose(fin);
	system("pause");
}
void thrDtest(){
	int i;
	
	B.render();
	for(i=0;i<9438*3;i++)
			pot[i].z-=2;

	for(i=0;i<9438*3;i++){
		fr3d.x_rotate(pot[i],0.2); 
		fr3d.y_rotate(pot[i],0.3); 
	}
	for(i=0;i<9438*3;i++)
		pot[i].z+=2;
}

void show(FR_WIN_CMD* HWND){
	win.clear(); 
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
	
	/*
	win.run(msg,show);
	This model is too complex for dynamic showing. 
	*/
	int i;
	
	while(!msg()){
		win.clear(); 
		B.render();
		win.show();
	
		for(i=0;i<9438*3;i++)
			pot[i].z-=2;

		for(i=0;i<9438*3;i++){
			fr3d.x_rotate(pot[i],0.2); 
			fr3d.y_rotate(pot[i],0.3); 
		}
		for(i=0;i<9438*3;i++)
			pot[i].z+=2;
			
	}
	
	return 0;
}
