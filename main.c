#include "graphics.h"
#include "genlib.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "extgraph.h"
#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
    InitConsole();
}

int btndown=0,num;
double xx,yy,endx,endy,word_x,word_y;
char s[1000]; //the string now
int length;  
int eraser,mode,started,characters;
double start_x[200],start_y[200],end_x[200],end_y[200];
int type[200];
int move,zoom;                                     
//=======================================================
void DrawAll();
void mouseEventCallback(int x,int y,int btn,int status);
void keyboardEventCallback(int key,int event);
void DrawBox(double x, double y, double width, double height);
void DrawUI(void);
//==========================================================
void Main()
{
    InitConsole();
    InitGraphics();
    DrawUI();
    registerMouseEvent(mouseEventCallback);
    registerKeyboardEvent(keyboardEventCallback);  
}
//================================================================
void keyboardEventCallback(int key,int event){     
     if (event==0) {
        s[length]=(char)key;
        length++;
        s[length]=0;
        MovePen(word_x,word_y);
        DrawTextString(s);
        printf("key %s\n",s);
     }  
}
//========================================================
void mouseEventCallback(int x,int y,int btn,int status){
     if ((mode >= 1)||(move == 1)||(zoom == 1)) {
        if ((btn == 1)&&(status == 0)){
           btndown = 1;
           started = 1;
           xx = (double)x / GetXResolution();
           yy = GetWindowHeight() - (double)y / GetXResolution();
        }
        if ((btn == 1)&&(status == 2)){
           if (started != 1)return;
           btndown = 0;    
           endx = (double)x / GetXResolution();
           endy = GetWindowHeight() - (double)y / GetXResolution();
           started = 0;
           if ((endx == xx)&&(endy == yy)) return;
           if (mode != 0){
              num++;
              start_x[num] = xx;           start_y[num] = yy;
              end_x[num] = endx;          end_y[num] = endy;   
              if (mode == 1){
			    type[num] = 1; 
			  }
              if (mode == 2){
			  	type[num] = 2;
			  }
              if (mode == 3){
			  	type[num] = 3;
			  }
              DisplayClear(); 
              DrawUI();
              DrawAll();     
           }
           if ((move == 1)&&(mode == 0)){
              start_x[num] += endx - xx;           start_y[num] += endy - yy;
              end_x[num] += endx - xx;          end_y[num] += endy - yy;
              DisplayClear(); DrawUI();     DrawAll();  
           }
           if ((zoom == 1)&&(mode == 0)){
              end_x[num] += endx - xx;          end_y[num] += endy - yy;
              DisplayClear(); DrawUI();     DrawAll();  
           }
        }  
       // 绘图,缩放模式 
     }  
     if (characters == 1) {
        word_x = (double)x / GetXResolution();
        word_y = GetWindowHeight() - (double)y / GetXResolution();
        s[0] = 0; length = 0;
     }
     //按钮判断
     if ((btn == 1)&&(status == 0)){
        btndown = 1;
        if ((x >= 9) && (x <= 50)) {
        	if ((y >= 16) && (y <= 48)) {
        	 	DrawUI();      mode = 1; 
            	move = 0;      zoom = 0;
         	}
         	if ((y >= 67) && (y <= 95)) {
         		DrawUI();      mode = 2;
				move = 0;      zoom = 0; 
         	}
         	if ((y >= 114) && (y <= 144)) {
         		DrawUI();      mode = 3;
            	move = 0;      zoom = 0;
         	}
         	if ((y >= 163) && (y <= 193)) {
         		DrawUI();      characters=1; 
				mode = 0;      zoom = 0;
				move = 0;
			}
			if ((y >= 212) && (y <= 242)) {
				move = 1 - move;
                zoom = 0; mode = 0;
                printf("premove%d\n",move);
			}
			if ((y >= 261) && (y <= 291)) {
				zoom = 1 - zoom;
                move = 0; mode = 0;
                printf("ready to adjust\n");
			}		
        }
        if ((y >= 347)&&(y <= 374)){ 
           if ((x >= 278)&&(x <= 336)){  //清屏   
              DisplayClear();
              DrawUI();  num = 0;  mode = 0;
           }
           if ((x >= 348)&&(x <= 431)){  //删除 
              DisplayClear(); if (num > 0) num--;
              DrawUI();    DrawAll();  
           }
        }
     }
     if ((btn == 1)&&(status == 2)){
        btndown = 0;   
     }
}

//============================================================
void DrawUI(void){
	 SetPenColor("Blue");
     DrawBox(0.1,3.5,0.45,0.3);
     DisplayText(0.15,3.6,"直线");
     DrawBox(0.1,3.0,0.45,0.3);
     DisplayText(0.15,3.1,"矩形");
     DrawBox(0.1,2.5,0.45,0.3);
     DisplayText(0.15,2.6,"椭圆");
     DrawBox(0.1,2.0,0.45,0.3);
     DisplayText(0.15,2.1,"文字");
     SetPenColor("Red");
     DrawBox(2.9,0.1,0.6,0.3);
     DisplayText(2.95,0.2,"清屏");
     DrawBox(3.6,0.1,0.9,0.3);
     DisplayText(3.65,0.2,"删除上一个");
     SetPenColor("Green");
     DrawBox(0.1,1.5,0.45,0.3);
     DisplayText(0.15,1.6,"移动");
     DrawBox(0.1,1.0,0.45,0.3);
     DisplayText(0.15,1.1,"缩放");
     SetPenColor("Black");
     DisplayText(0.12,0.5,"选择绘图模式后通过拖动放置图形,点击移动或者缩放后,通过拖动操作，只对最后放置的图形");
}
//============================================================
void DrawBox(double x, double y, double width, double height)
{
    MovePen(x, y);
    DrawLine(0.0, height);
    DrawLine(width, 0.0);
    DrawLine(0.0, -height);
    DrawLine(-width, 0.0);
}
void DrawAll(){
     int i;
     printf("num=%d\n",num);
     for (i = 1;i <= num;i++){
         if (type[i] == 1){
            MovePen(start_x[i],start_y[i]);
            DrawLine(end_x[i] - start_x[i],end_y[i] - start_y[i]); 
         }
         if (type[i] == 2){
            DrawBox(start_x[i],start_y[i],end_x[i] - start_x[i],end_y[i] - start_y[i]);
         }
         if (type[i] == 3){
            MovePen(end_x[i],start_y[i] + (end_y[i] - start_y[i]) / 2.0);
            DrawEllipticalArc((end_x[i] - start_x[i]) / 2.0,(end_y[i] - start_y[i])/2,0,360.0);
         }
     }
}     
