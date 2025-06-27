#include "iGraphics.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define screenwidth 600
#define screenheight 600
#define R 10
#define C 12
int bricks[R][C];
int brickWidth = 50;
int brickHeight = 30;
int topMargin = 80; // keep space for better looking
int leftMargin = (600 - (C * brickWidth)) / 2; // center horizontally
int bar_x = 200, bar_y = 0, bar_width = 100, bar_height = 10, bar_speed = 5;
int ball_x = 235, ball_y = 10, ball_radius = 10, dx = 5, dy = 5;
int score = 0;
int lives = 3;
int highscore=0;
int currentview = 0;
int currentLevel = 1;
char playerinfo[50];
int namesize=0;
int index=0;

typedef struct {
    char name[30];
    int score;
} playerrecord;



int easy[R][C] = {
    {1,0,1,0,1,0,1,0,1,0,1,0},
    {0,1,0,1,0,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,1,0,1,0,1,0},
    {0,1,0,1,0,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,1,0,1,0,1,0},
    {0,1,0,1,0,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,1,0,1,0,1,0},
    {0,1,0,1,0,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,1,0,1,0,1,0},
    {0,1,0,1,0,1,0,1,0,1,0,1}
};

int medium[R][C] = {
    {0,1,1,0,0,1,1,0,0,1,1,0},
    {1,0,0,1,1,0,0,1,1,0,0,1},
    {1,0,0,1,1,0,0,1,1,0,0,1},
    {0,1,1,0,0,1,1,0,0,1,1,0},
    {0,1,1,0,0,1,1,0,0,1,1,0},
    {1,0,0,1,1,0,0,1,1,0,0,1},
    {1,0,0,1,1,0,0,1,1,0,0,1},
    {0,1,1,0,0,1,1,0,0,1,1,0},
    {0,1,1,0,0,1,1,0,0,1,1,0},
    {1,0,0,1,1,0,0,1,1,0,0,1}
};

int hard[R][C] = {
    {0,0,0,0,0,1,1,0,0,0,0,0},
    {0,0,1,0,1,1,1,1,0,1,0,0},
    {0,1,0,1,0,1,1,0,1,0,1,0},
    {1,0,1,0,1,0,0,1,0,1,0,1},
    {0,1,0,1,0,1,1,0,1,0,1,0},
    {1,0,1,0,1,0,0,1,0,1,0,1},
    {0,1,0,1,0,1,1,0,1,0,1,0},
    {0,0,1,0,1,1,1,1,0,1,0,0},
    {0,0,0,0,0,1,1,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0}
};

void save_score_to_file() {
    FILE *fp = fopen("leaderboard.txt", "a");  // append mode
    if (fp != NULL) {
        fprintf(fp, "%s %d\n", playerinfo, score);
        fclose(fp);
    }
}
bool scoreSaved = false;

void saveHighScoreToFile() {
    FILE *fp = fopen("highscore.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "%d\n", highscore);
        fclose(fp);
    }
}

void loadHighScoreFromFile() {
    FILE *fp = fopen("highscore.txt", "r");
    if (fp != NULL) {
        fscanf(fp, "%d", &highscore);
        fclose(fp);
    }
}


void setLevel(int level){
    currentLevel = level;
    dx = 5 + (level - 1) * 2;
    dy = 5 + (level - 1) * 2;
    if (level == 1) memcpy(bricks, easy, sizeof(easy));
    else if (level == 2) memcpy(bricks, medium, sizeof(medium));
    else if (level == 3) memcpy(bricks, hard, sizeof(hard));
    else memcpy(bricks, easy, sizeof(easy));
// renew ball and bar pos
    ball_x = 235;
    ball_y = 10;

    bar_x = 200;

//renew scoring and lives
    score = 0;
    lives = 3;




}
playerrecord leaderboardArray[100];
int leaderboardCount = 0;
void loadLeaderboard() {
    FILE *fp = fopen("leaderboard.txt", "r");
    if (!fp) {
        leaderboardCount = 0;
        return;
    }
    leaderboardCount = 0;
    while (fscanf(fp, "%s %d", leaderboardArray[leaderboardCount].name, &leaderboardArray[leaderboardCount].score) == 2) {
        leaderboardCount++;
        if (leaderboardCount >= 100) break;
    }
    fclose(fp);
    
    // Sort by score descending
    for (int i = 0; i < leaderboardCount - 1; i++) {
        for (int j = i + 1; j < leaderboardCount; j++) {
            if (leaderboardArray[j].score > leaderboardArray[i].score) {
                playerrecord temp = leaderboardArray[i];
                leaderboardArray[i] = leaderboardArray[j];
                leaderboardArray[j] = temp;
            }
        }
    }
}
void welcomepage() {
    iShowImage(0, 0, "sprites\\homepage3.jpg");
}

void exitpage() {
    iShowImage(0, 0, "sprites\\exitgame.jpeg");
}
void levelpage(){
    iShowImage(0, 0, "sprites\\level1.jpg");
}
void instructions(){
    iShowImage(0, 0, "sprites\\instructions.png");
}
void gameover(){
    iShowImage(0, 0, "sprites\\gameover1.jpg");
}
void leaderboard(){
    iShowImage(0, 0, "sprites\\leaderboard.jpg");
    for (int i = 0; i < leaderboardCount && i < 10; i++) {
    char line[50];
    sprintf(line, "%d. %s : %d", i + 1, leaderboardArray[i].name, leaderboardArray[i].score);
    iSetColor(0,0,0);
    iText(200, 420 - i * 30, line, GLUT_BITMAP_HELVETICA_18);
}
}
void entername(){
     iShowImage(0, 0, "sprites\\name.jpg");
}
void populateimages(){

     
}


void maingame() {
    iShowImage(0, 0, "sprites\\background1.jpg");

    iSetColor(255, 255, 0);
    iFilledCircle(ball_x, ball_y, ball_radius);
    iShowImage(bar_x, bar_y, "sprites\\bar.png");

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            if (bricks[i][j] > 0) {
             int x = leftMargin + j * brickWidth;
            int y = screenheight - topMargin - (i + 1) * brickHeight;
                if (currentLevel == 3 && bricks[i][j] == 2)
                    iSetColor(255, 255, 0); //  two hit bricks
                else if (i % 3 == 0) iSetColor(255, 0, 0);
                else if (i % 3 == 1) iSetColor(0, 255, 0);
                else iSetColor(0, 0, 255);

                 iFilledRectangle(x, y, brickWidth, brickHeight);
            }
        }
    }
}

void iDraw() {
    iClear();
    if (currentview==1)
    {
         maingame();        
        char scoreText[20];
        char livesText[20];
        char levelText[20];

        sprintf(scoreText, "Score: %d", score);
        sprintf(livesText, "Lives: %d ", lives);
        sprintf(levelText, "Level %d", currentLevel);

        iSetColor(255, 0, 0);

        iText(20, 570, scoreText,  GLUT_BITMAP_HELVETICA_18);  
        iText(270, 570, levelText,  GLUT_BITMAP_HELVETICA_18);   
        iText(480, 570, livesText,  GLUT_BITMAP_HELVETICA_18);  

}

     

    
    


    else if (currentview == 0) welcomepage(); 
    else if(currentview==2) levelpage();
    else if (currentview == 5) exitpage();
    else if(currentview==3) instructions();
    else if(currentview==6)//gameover
    {
         gameover();
        char scoreshow[30];
        char highscoreshow[30];
        sprintf(scoreshow , "Your Score : %d",score);
       sprintf(highscoreshow , "High Score : %d",highscore);
       iSetColor(255,255,255);
       iText(200,250,scoreshow,GLUT_BITMAP_TIMES_ROMAN_24);
       iText(200,220,highscoreshow,GLUT_BITMAP_TIMES_ROMAN_24);
       iText(150,170,"Press 'Home' to return to Main Menu",GLUT_BITMAP_HELVETICA_18);
    }
   else if(currentview==4) {
    
    leaderboard();
     
}
     else if(currentview==7){
        entername();
       iSetColor(0,0,0);
       iText(130,325,playerinfo,GLUT_BITMAP_HELVETICA_18);

       iSetColor(255,0,0);
      iText(80,150,"Press Right to start game and 'END' for backspace",GLUT_BITMAP_TIMES_ROMAN_24);

     }


}

void iMouseMove(int mx, int my) {

    bar_x = mx - bar_width / 2;

    // stop bar from going out screen
    if (bar_x < 0) bar_x = 0;
    if (bar_x + bar_width > screenwidth) bar_x = screenwidth - bar_width;
}
void iMouseDrag(int mx, int my) {}
void iMouseWheel(int dir, int mx, int my) {}
/*

currentview=0 ---menupage
currentview=1 ---maingame
currentview=2 ---levelpage
currentview=3 ---instructions
currentview=4 ---leaderboard
currentview=5 ---exitpage
currentview=6 ---gameover
currentview=7 ---entername
*/
void iMouse(int button, int state, int mx, int my) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (currentview == 0) {
            if (mx >= 200 && mx <= 364 && my >= 360 && my <= 430) {

                currentview=7;
                    playerinfo[0] = '\0'; // resets name
                      index = 0;
                
            }
        else if (mx >= 150 && mx <= 364 && my >= 70 && my <= 150) {
                currentview = 5;
            }
        else if (mx >= 150 && mx <= 364 && my >=300 && my <=360 ) 
           {
                 currentview = 7;
                playerinfo[0] = '\0';
                index = 0;
             
            }
        else if (mx >= 150 && mx <= 364 && my >=150 && my <=230 ) 
           {
                currentview = 3;
             
            }                 
        else if (mx >= 150 && mx <= 364 && my >=250 && my <=300 ) 
           {
                currentview = 4;
              loadLeaderboard();
            } 

        }
       else if(currentview==2){
                 if (mx >= 200 && mx <= 364 && my >=300 && my <=370 ) 
                 {
                currentview = 1;
                setLevel(3);
                           }
         else if (mx >= 200 && mx <= 364 && my >=440 && my <=520 ) 
                {
                currentview = 1;
                setLevel(1);
                           }
           else  if (mx >= 160 && mx <= 390 && my >=380 && my <=430 )
                  {
                currentview = 1;
                setLevel(2);
                           }

                          }

         
}
}
void iKeyboard(unsigned char key) {
    if (key == '1') setLevel(1);
    if (key == '2') setLevel(2);
    if (key == '3') setLevel(3);
    if (key == 'q' || key == 'Q') {
    currentview = 5;


}
    if (key == 'p' || key == 'P') {
    currentview = 2;    
}
  

    if (currentview == 7) {
        if (index < 29 && key >= 32 && key <= 126) {
            playerinfo[index++] = key;
            playerinfo[index] = '\0';
        }
    }

}
void iSpecialKeyboard(unsigned char key) {
    if (key ==GLUT_KEY_HOME) {
         if (currentview==1)
         {
            currentview=6;

         }
      else {
    currentview = 0;
    score=0;
    lives=3;
    scoreSaved=false;
}

    }
    if (currentview == 7) {
        if (key == GLUT_KEY_RIGHT) {
            currentview = 2;     
            scoreSaved=false; // Start game
        } else if (key == GLUT_KEY_END) {
            if (index > 0) {
                index--;
                playerinfo[index] = '\0';
            }
        }
    }


}


void ballmovement() {
    if (currentview!=1)
    {
        return ;
    }
    
    ball_x += dx;
    ball_y += dy;

    if (ball_x - ball_radius <= 0 || ball_x + ball_radius >= screenwidth) dx = -dx;
    if (ball_y + ball_radius >= screenheight) dy = -dy;



    for (int i = 0; i < R; i++) {
    for (int j = 0; j < C; j++) {
        if (bricks[i][j] > 0) {
            int brickX = leftMargin + j * brickWidth;
            int brickY = screenheight - topMargin - (i + 1) * brickHeight;

            //  if ball overlaps with this brick
            if (ball_x + ball_radius >= brickX &&
                ball_x - ball_radius <= brickX + brickWidth &&
                ball_y + ball_radius >= brickY &&
                ball_y - ball_radius <= brickY + brickHeight) {

               // brick breaking
                if (bricks[i][j] == 1) {
                    bricks[i][j] = 0;
                    score=score+5;  
                } 
                
                else if (bricks[i][j] == 2) {
                    bricks[i][j] = 1;
                    score=score +5;  
                }

                dy = -dy;  
                goto skipLoop;  // exit both loops  after a hit
            }
        }
    }
}
skipLoop:;

    if (ball_y <= bar_y + bar_height &&
        ball_x + ball_radius >= bar_x && ball_x <= bar_x + bar_width)
        dy = -dy;


    if (ball_y<0)
    {
        lives--;
    }

    if (lives <= 0) {
    

    if (score > highscore) {
        highscore = score;
        saveHighScoreToFile(); 
    }

    currentview = 6;

    if (!scoreSaved) {
        save_score_to_file(); 
        scoreSaved = true;
    }

    return;
}



    if (ball_y < 0) {
        ball_x = bar_x + bar_width / 2;
        ball_y = bar_y + bar_height + 5;

    }


    
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    loadHighScoreFromFile();
    setLevel(1);
    iSetTimer(20, ballmovement);
    iInitialize(screenwidth, screenheight, "DX Ball");
    return 0;
}

