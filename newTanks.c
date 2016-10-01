#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "gfx.h"
#define BLUE 10
#define RED -10
#define XSIZE 1000
#define YSIZE 800
#define UP 'A'
#define DOWN 'B'
#define LEFT 'D'
#define RIGHT 'C'
#define WIDTH 24

int locations[YSIZE][XSIZE];
double bullets1[100][5]; //BLUE TANK BULLETS - Whether it's on the board, x position, y position, delta x, delta y
double bullets2[100][5]; //RED TANK BULLETS - Whether it's on the board, x position, y position, delta x, delta y
int redDirection = 1; //1 equal to moving up / down, 2 equal to moving side to side
int blueDirection = 1;

char livesStr[30];	//prints current lives onto graphics screen
int randTime;	//initializes randomized time for CPU shooter
int randBullets;	//CPU randomly fires 10 bullets every now and then

clock_t start_t, end_t, total_t;

struct tank {
	int xPos;
	int yPos;
	int color;
	int lives;
} blueTank, redTank;

void gameTitle(void);
void fireBullet(int);
void setLocations(void);
void setTanks(void);
void drawTanks(void);
void applyChange(char, int);
void checkBoard(void);
void drawBodies(void);
void drawTreads(void);
void drawBullets(void);
char calcCpuDirection(int, int);
int set_lives(int);
void printScore(void);

int main(void){
	srand(time(NULL));	// initializes random int generator

	//This part of the code sets up the program
		char c = 't';								//The movement direction of BLUE
		char q;
		char p;
		gfx_open(XSIZE, YSIZE, "TANKS");
		setTanks();
		drawTanks();

		int bullet1Num = 0;
		int bullet2Num = 0;
		int returnFireCount = 0;
		randTime = (1 + (rand() % 3));
		p = calcCpuDirection(redTank.xPos , redTank.yPos);		//The movement direction of RED

	// Now everything is set up

	while(c != 'q'){
		gameTitle();
		q = gfx_wait();

		if(q == 1){

			while(1){
			if ( (redTank.lives == 0) || (blueTank.lives == 0) ) break;

				if( gfx_event_waiting() ){
			    	c = gfx_wait();
			    	
			    	if(c == 'q'){   //Quits the program if they click q
						break;
					}
					else if( c == 1){ // Fires a bullet 
						fireBullet(BLUE);
						fireBullet(RED);

					}
					else{ // Takes the user input and moves the tank positions if its valid
						applyChange(c , BLUE);
					}
				}

				else{ // If there is no button click, it animates any bullets on the board
					usleep(2000);
					applyChange(p , RED);
					returnFireCount++;

					if(returnFireCount == 20){
						returnFireCount = 0;
						p = calcCpuDirection(redTank.xPos, redTank.yPos);
						if(randTime == 2){
								fireBullet(RED);
						}
						else{
							srand(time(NULL));
							randTime = (1 + (rand() % 3));
						}
					}
				}
			}
		}
		else if(q == 3){
			while(1){
				drawTanks();
				if ( (redTank.lives == 0) || (blueTank.lives == 0) ) break;


				if( gfx_event_waiting() ){
			    	c = gfx_wait();
			    	
			    	if(c == 'q'){   //Quits the program if they click q
						break;
					}
					else if( c == 1){ // Fires a bullet 
						fireBullet(BLUE);
						fireBullet(RED);

					}
					else{ // Takes the user input and moves the tank positions if its valid
						applyChange(c , BLUE);
					}
				}

				else{ // If there is no button click, it animates any bullets on the board
					usleep(2000);
					applyChange(p , RED);
					returnFireCount++;

					if(returnFireCount == 20){
						q = calcCpuDirection(redTank.xPos, redTank.yPos);
						
						if( (randTime == 2) || (randTime == 3) ){
								fireBullet(RED);
						}
						else{
							srand(time(NULL));
							randTime = (1 + (rand() % 3));
						}

						returnFireCount = 0;
					}
				}

				drawTanks();
			}			
		}
	}

return(0);
} // End Main



/*################### FUNCTIONS ######################*/
void setTanks(void){
	blueTank.xPos = 750;
	blueTank.yPos = 550;
	blueTank.lives = 10;
	blueTank.color = BLUE;

	redTank.xPos = 50;
	redTank.yPos = 50;
	redTank.color = RED;
	redTank.lives = 10;

	int i, j;

	for(i = 0 ; i < 100 ; i++){
	for(j = 0 ; j < 5 ; j++){
		bullets1[i][j]= 0;
		bullets2[i][j]= 0;
	}
	}

	setLocations();
}

void drawTanks(void){
	gfx_clear();
	setLocations();

	drawBodies();

	drawTreads();

	drawBullets();
	printScore();

	checkBoard();
}

void setLocations(void){
	int xi, xj;
	for( xi = 0 ; xi < XSIZE ; xi++){
	for( xj = 0 ; xj < YSIZE ; xj++){
		locations[xj][xi] = 0;
	}
	}
}

void applyChange(char direction, int color){
	if(color == BLUE){
		switch(direction){
				case 'w':
					if( (blueTank.yPos - WIDTH) > 10){
						blueDirection = 1;
						blueTank.yPos = blueTank.yPos-10;
					}
					else{blueTank.yPos++;}
					break;
				case 's':
					if( (blueTank.yPos + WIDTH) < YSIZE +10 ){
						blueDirection = 1;
						blueTank.yPos = blueTank.yPos-10;
					}
					else{blueTank.yPos--;}
					break;
				case 'a':
					if( (blueTank.xPos - WIDTH) > 10 ){
						blueDirection = 2;
						blueTank.xPos = blueTank.xPos-10;
					}
					else{blueTank.xPos++;}
					break;
				case 'd':
					if( (blueTank.xPos + WIDTH + 10) < XSIZE + 10 ){
						blueDirection = 2;
						blueTank.xPos = blueTank.xPos+10;
					}
					else{blueTank.xPos--;}
					break;
				default:
					break;
		}
	}
	else if(color == RED){
		switch(direction){
				case 'w':
					if( (redTank.yPos - WIDTH - 5) > 5){
						redTank.yPos = redTank.yPos-5;
					}
					break;
				case 's':
					if( (redTank.yPos + WIDTH + 5) < YSIZE-5){
						redTank.yPos = redTank.yPos+5;
					}
					break;
				case 'a':
					if( (redTank.xPos - WIDTH - 5) > 5){
						redTank.xPos = redTank.xPos-5;
					}
					break;
				case 'd':
					if( (redTank.xPos + WIDTH + 5) < YSIZE-5){
						redTank.xPos = redTank.xPos+5;
					}
					break;
				default: 
					break;
		}		
	}
}

void fireBullet(int color){
	double theta1, theta2;
	int x, y, deltaX, deltaY;
	int i,j;
	int check = 0;

	for( i = 0 ; i < 100 ; i++){
		if (bullets1[i][0] == 0) break;
		if ( i == 100){
			check = 1;
			break;
		}
	}

	for( j = 0 ; j < 100 ; j++){
		if (bullets2[j][0] == 0) break;
		if ( j == 100){
			check = 1;
			break;
		}
	}


	if( check == 0){
		if(color == BLUE){
			bullets1[i][0] = 1;
			bullets1[i][1] = (double) blueTank.xPos;
			bullets1[i][2] = (double) blueTank.yPos;

			x = gfx_xpos();
			y = gfx_ypos();
			deltaX = x - blueTank.xPos;		//difference between initial position of tank and bullet destination
			deltaY = y - blueTank.yPos;

			theta1 = atan2(deltaY, deltaX);

			bullets1[i][3] = cos(theta1);
			bullets1[i][4] = sin(theta1);
		}
		else if( color == RED ){
			bullets2[j][0] = 1;
			bullets2[j][1] = (double) redTank.xPos;
			bullets2[j][2] = (double) redTank.yPos;

			deltaX = blueTank.xPos - redTank.xPos;
			deltaY = blueTank.yPos - redTank.yPos;

			theta2 = atan2(deltaY, deltaX);

			bullets2[j][3] = cos(theta2);
			bullets2[j][4] = sin(theta2);
		}
	}
}

void checkBoard(void){
	int count = 0;
	for(count = 0 ; count < 100 ; count++){
		int xCheck1 = bullets1[count][1];
		int yCheck1 = bullets1[count][2];
		int xCheck2 = bullets2[count][1];
		int yCheck2 = bullets2[count][2];

		if( bullets1[count][0] == 1){

			if( (xCheck1 < 0) || (xCheck1 > XSIZE ) || (yCheck1 < 0) || (yCheck1  > YSIZE ) ){
				bullets1[count][0] = 0;
			}

			else if( locations[xCheck1][yCheck1] == 1 ){
				bullets1[count][0] = 0;
				redTank.lives--;
				setTanks();
			}
		}

		if( bullets2[count][0] == 1){

			if( (xCheck2 < 0) || (xCheck2 > XSIZE ) || (yCheck2 < 0) || (yCheck2  > YSIZE ) ){
				bullets2[count][0] = 0;
			}

			else if( locations[xCheck2][yCheck2] == 2 ){
				bullets2[count][0] = 0;
				blueTank.lives--;
				setTanks();
			}
		}

	}
}

void drawBodies(void){
	int i, j;
	
	//This draws the inner grey square
	for( i = 0 ; i < 36 ; i++){
	for( j = 0 ; j < 36 ; j++){
		gfx_color(192,192,192);
		gfx_point(blueTank.xPos-18+i, blueTank.yPos-18+j);
		gfx_point(redTank.xPos-18+i, redTank.yPos-18+j);
		locations[redTank.xPos-18+i][redTank.yPos-18+j] = 1;
		if(locations[blueTank.xPos-18+i][blueTank.yPos-18+j] != 0){
			setTanks();
		}
		else{
		locations[blueTank.xPos-18+i][blueTank.yPos-18+j] = 2;
		}
	}
	}

	//This draws the color inside the square
	for( i = 0 ; i < 10 ; i++){
	for( j = 0 ; j < 10 ; j++){
		gfx_color(0,0,255);
		gfx_point(blueTank.xPos-5+i, blueTank.yPos-5+j);
		gfx_color(255,0,0);
		gfx_point(redTank.xPos-5+i, redTank.yPos-5+j);
	}
	}
}

void drawTreads(void){

	int i, j;
	gfx_color(160,160,160);
	
	//This Draws the Treads
	if(blueDirection == 1){
		for( i = 0 ; i < 8 ; i++){
		for( j = 0 ; j < 44 ; j++){
			gfx_point(blueTank.xPos - 26 + i , blueTank.yPos - 22 + j);
			gfx_point(blueTank.xPos + 18 + i , blueTank.yPos - 22 + j);		
			gfx_point(redTank.xPos - 26 + i , redTank.yPos - 22 + j);
			gfx_point(redTank.xPos + 18 + i , redTank.yPos - 22 + j);	
			locations[redTank.xPos-26+i][redTank.yPos-22+j] = 1;
			locations[redTank.xPos+18+i][redTank.yPos-22+j] = 1;
			locations[blueTank.xPos-26+i][blueTank.yPos-22+j] = 2;
			locations[blueTank.xPos+18+i][blueTank.yPos-22+j] = 2;
		}
		}
	}	
	else if(blueDirection == 2){
		for( i = 0 ; i < 8 ; i++){
		for( j = 0 ; j < 44 ; j++){
			gfx_point(blueTank.xPos - 22 + j , blueTank.yPos - 26 + i );	
			gfx_point(blueTank.xPos - 22 + j , blueTank.yPos + 18 + i );	
			gfx_point(redTank.xPos - 26 + i , redTank.yPos - 22 + j);
			gfx_point(redTank.xPos + 18 + i , redTank.yPos - 22 + j);	
			locations[redTank.xPos-26+i][redTank.yPos-22+j] = 1;
			locations[redTank.xPos+18+i][redTank.yPos-22+j] = 1;
			locations[blueTank.xPos-22+j][blueTank.yPos-26+i] = 2;
			locations[blueTank.xPos-22+j][blueTank.yPos+18+i] = 2;
		}
		}
	}
}

void drawBullets(void){
	int bulletCount, i, j;

	for( bulletCount = 0 ; bulletCount < 100 ; bulletCount++){
		if(bullets1[bulletCount][0] == 1){
			for( i = 0 ; i < 4 ; i++){
			for( j = 0 ; j < 4 ; j++){
				gfx_color(255,255,0);
				gfx_point(bullets1[bulletCount][1]-2+i, bullets1[bulletCount][2]-2+j);
			}
			}
			bullets1[bulletCount][1] = bullets1[bulletCount][1] + 2*bullets1[bulletCount][3];
			bullets1[bulletCount][2] = bullets1[bulletCount][2] + 2*bullets1[bulletCount][4];
		}
	}

	for( bulletCount = 0 ; bulletCount < 100 ; bulletCount++){
		if(bullets2[bulletCount][0] == 1){
			for( i = 0 ; i < 4 ; i++){
			for( j = 0 ; j < 4 ; j++){
				gfx_color(0,255,255);
				gfx_point(bullets2[bulletCount][1]-2+i, bullets2[bulletCount][2]-2+j);
			}
			}
			bullets2[bulletCount][1] = bullets2[bulletCount][1] + 2*bullets2[bulletCount][3];
			bullets2[bulletCount][2] = bullets2[bulletCount][2] + 2*bullets2[bulletCount][4];
		}
	}
}

char calcCpuDirection(int xPos, int yPos){
	char direction;
	int t = rand() % 6;
	int boolean = 0;	//determines final direction

	while (boolean != 1){

		if(t == 1){
			if(yPos < 2){
				continue;
			}
			direction = 'w';
			boolean = 1;
		}
		else if(t == 2){
			if(yPos > (YSIZE - 2)){
				continue;
			}			
			direction = 's';
			boolean = 1;
		}
		else if(t == 3){
			if(xPos < 2){
				continue;
			}
			direction = 'a';
			boolean = 1;
		}
		else if(t == 4){
			if(xPos > (XSIZE - 2)){
				continue;
			}			
			direction = 'd';
			boolean = 1;
		}
		else{
			direction = 'l';
			boolean = 1;
		}
	}
	return(direction);
}

void gameTitle(void){
		gfx_text((XSIZE/2) - 20,YSIZE/8, "TANKS");
		gfx_text((XSIZE/2) - 20,(YSIZE/8)+30, "Game by Jose Badilla and Owen Phelan");
		gfx_text((XSIZE/2) - 20,(YSIZE/8)+60, "Difficulty: Left click for EASY || Right click for HARD");
}

void printScore(void){

	sprintf(livesStr, "Lives: Red %d || Blue %d", redTank.lives, blueTank.lives);
	gfx_color(255,255,255);
	gfx_text(3*XSIZE/4,YSIZE/8, livesStr);
}





