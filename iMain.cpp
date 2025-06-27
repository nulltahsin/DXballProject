#include "iGraphics.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>
#include <math.h>

#define screenwidth 600
#define screenheight 600
/*#define cols 17
#define rows 10
#define brickHeight 10
#define brickWidth 30
#define maxscores 10
#define brickPadding 1
#define maxpowerup 5
#define maxball 4*/

int bar_x = 200, bar_y = 0, bar_width = 100, bar_height = 10, bar_speed = 5;
int ball_x = 235,ball_y = 10, ball_radius = 10, dx = 5,  dy =5;

int currentview=0;

void welcomepage(){

    iShowImage(0,0,"sprites\\homepage3.jpg");
}

void maingame(){

        iShowImage(0,0,"sprites\\background1.jpg");

    iSetColor(255,255,0);
    iFilledCircle(ball_x,ball_y,ball_radius);

   // iSetColor(255,0,0);
   // iFilledRectangle(bar_x,bar_width,bar_width,bar_height);
   iShowImage(bar_x,bar_y,"sprites\\bar.png");
}

void exitpage(){

     iShowImage(0,0,"sprites\\exitgame.jpeg");
}


void iDraw()
{
    iClear();
    if (currentview==0)
    {
       welcomepage();
    }
    else if (currentview==1)
    {
        maingame();
    }
        else if (currentview==5)
    {
        exitpage();
    }
    
}


void populate_images(){



}


void iMouseMove(int mx, int my)
{
    // place your codes here
}


void iMouseDrag(int mx, int my)
{
    // place your codes here
}

// currentview ==0 is menu page
// currentview==1 is main game page
//currentview==5 is exit game 
//others we will add ahead in the project 
void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
       if (currentview==0)
       {
        if (mx>=200 && mx<= 364 && my>=300 && my<=430)
        {
            currentview=1;
        }
     /*   if (mx>=150 && mx<=364 && my>=195 && my<=255)
        {
            currentview=2;
        }
        if (mx>=150 && mx<=364 && my>=265 && my<=305)
        {
            currentview=3;
        }
        if (mx>=150 && mx<=364 && my>=315 && my<=375)
        {
            currentview=4;
        }
            */
        if (mx>=150 && mx<=364&& my>=70 && my<=150)
        {
            currentview=5;
        }
        
       }
       
    }
   
}

/*
function iMouseWheel() is called when the user scrolls the mouse wheel.
dir = 1 for up, -1 for down.
*/
void iMouseWheel(int dir, int mx, int my)
{
    // place your code here
}

/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    
}

void ballmovement() {

    


    ball_x += dx;
    ball_y += dy;

    //Reflect from left &right walls
    if (ball_x - ball_radius <= 0 || ball_x + ball_radius >= screenwidth) 
    {
        dx = -dx;
    }

    // REflect from top wall
    if (ball_y + ball_radius >= screenheight)
     {
        dy = -dy;
    }

   

// if Ball hits the bar
    if (ball_y <= bar_y + bar_height && ball_x + ball_radius >= bar_x && ball_x <= bar_x + bar_width) 
    {
        dy = -dy;
       
    }
    if (ball_y<0)
    {
    

    ball_x = (bar_x + bar_width / 2) - ball_radius; //setting the ball on the bar
    ball_y = bar_y + bar_height + 5; //centerizing the ball on top of the bar
    dx = 5; dy = 5; //resetting spped
    }
 

    
}
/*
function iSpecialKeyboard() is called whenver user hits special keys likefunction
keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11,
GLUT_KEY_F12, GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN,
GLUT_KEY_PAGE_UP, GLUT_KEY_PAGE_DOWN, GLUT_KEY_HOME, GLUT_KEY_END,
GLUT_KEY_INSERT */
void iSpecialKeyboard(unsigned char key){
    if (key == GLUT_KEY_LEFT) {
        bar_x -= 20;
        if (bar_x < 0)
        {
             bar_x = 0;
        }
    }
    if (key == GLUT_KEY_RIGHT) {
        bar_x += 20;
        if (bar_x + bar_width > screenwidth)
        {
            bar_x = screenwidth - bar_width;
        }
    }
}
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    populate_images();
    iSetTimer(20,ballmovement);
    iInitialize(screenwidth,screenheight,"DX Ball");
    return 0;
}