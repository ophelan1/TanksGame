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
#define UP 'A';
#define DOWN 'B';
#define LEFT 'D';
#define RIGHT 'C';

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
	double deltaX;
	double deltaY;
	double theta;
	int color;
	int lives;
} blueTank, redTank;

void fireBullet(int, int);
void setLocations(void);
void setTanks(void);
void drawTanks(void);
void drawTanks2(void);
void applyChange(char, int);
void checkBoard(void);
void drawBodies(void);
void drawTreads(void);
void drawBullets(void);
void drawBullets2(void);
char calcCpuDirection(int, int);
int set_lives(int);

int main(void){
	srand(time(NULL));	// initializes random int generator

	//This part of the code sets up the program
		int bullet1Num = 0;
		int bullet2Num = 0;
		int returnFireCount = 0;

		printf("Now we are going to establish the variables\n");
		char c;
		char q = ' ';
		gfx_open(XSIZE, YSIZE, "TANKS");
		printf("The Board is now open\n");

		setTanks();
		printf("The Tanks are now set to their original positions\n");
		drawTanks();
		printf("The Board is now drawn\n");
		gfx_color(255,255,255);
		strcpy(livesStr,"Lives: Red 10 || Blue 10");
		start_t = clock();
		randTime = (1 + (rand() % 3));

	// Now everything is set up

	while(c != 'q'){
		gfx_text((XSIZE/2) - 20,YSIZE/8, "TANKS");
		gfx_text((XSIZE/2) - 20,(YSIZE/8)+30, "Game by Jose Badilla and Owen Phelan");
		gfx_text((XSIZE/2) - 20,(YSIZE/8)+60, "Difficulty: Left click for EASY || Right click for HARD");
		redTank.lives = 10;
		blueTank.lives = 10;
		strcpy(livesStr,"Lives: Red 10 || Blue 10");
		q = gfx_wait();

		if(q == 1){
			while(1){
				gfx_text(3*XSIZE/4,YSIZE/8, livesStr);

				if ( (redTank.lives == 0) || (blueTank.lives == 0) ){
					break;
				}

				gfx_color(255,255,255);
				gfx_text(3*XSIZE/4,YSIZE/8, livesStr);
				if( gfx_event_waiting() ){
			    	c = gfx_wait();
			    	
			    	if(c == 'q'){   //Quits the program if they click q
						break;
					}
					else if( c == 1){ // Fires a bullet 
						bullet1Num++;
						bullet2Num++;
						if(bullet1Num == 100){ 
							bullet1Num = 0;
						}
						if(bullet2Num == 100){ 
							bullet2Num = 0;
						}
						while(bullets1[bullet1Num][0] != 0){	//This while loop means that it won't overwrite any bullets that are on the board already;
							bullet1Num++;
						}
						while(bullets2[bullet2Num][0] != 0){
							bullet2Num++;
						}
						fireBullet(BLUE, bullet1Num);
						fireBullet(RED, bullet2Num);

					}
					else{ // Takes the user input and moves the tank positions if its valid
						applyChange(c , BLUE);
					}
				}

				else{ // If there is no button click, it animates any bullets on the board
					usleep(2000);
					drawTanks();
					applyChange(q , RED);
					returnFireCount++;
					end_t = clock();
					total_t = (double)(end_t-start_t)/CLOCKS_PER_SEC;

					if((randTime >= (total_t - 1)) && (randTime <= (total_t + 1))){
						for (randBullets = 0; randBullets < 10; randBullets++){		//Allows CPU to shoot when user is not shooting
							bullet2Num++;
							if(bullet2Num == 100){ 
								bullet2Num = 0;
							}
							fireBullet(RED, bullet2Num);
						}
						randTime = randTime + (1 + (rand() % 3));
					}
					if(returnFireCount == 20){
						returnFireCount = 0;
						q = calcCpuDirection(redTank.xPos, redTank.yPos);
				}
			}

				gfx_color(255,255,255);
				gfx_text(3*XSIZE/4,YSIZE/8, livesStr);
			}
		}
		else if(q == 3){
			while(1){
				gfx_text(3*XSIZE/4,YSIZE/8, livesStr);

				if ( (redTank.lives == 0) || (blueTank.lives == 0) ){
					break;
				}

				gfx_color(255,255,255);
				gfx_text(3*XSIZE/4,YSIZE/8, livesStr);
				if( gfx_event_waiting() ){
			    	c = gfx_wait();
			    	
			    	if(c == 'q'){   //Quits the program if they click q
						break;
					}
					else if( c == 1){ // Fires a bullet 
						bullet1Num++;
						bullet2Num++;
						if(bullet1Num == 100){ 
							bullet1Num = 0;
						}
						if(bullet2Num == 100){ 
							bullet2Num = 0;
						}
						while(bullets1[bullet1Num][0] != 0){	//This while loop means that it won't overwrite any bullets that are on the board already;
							bullet1Num++;
						}
						while(bullets2[bullet2Num][0] != 0){
							bullet2Num++;
						}
						fireBullet(BLUE, bullet1Num);
						fireBullet(RED, bullet2Num);

					}
					else{ // Takes the user input and moves the tank positions if its valid
						applyChange(c , BLUE);
					}
				}

				else{ // If there is no button click, it animates any bullets on the board
					usleep(2000);
					drawTanks2();
					applyChange(q , RED);
					returnFireCount++;
					end_t = clock();
					total_t = (double)(end_t-start_t)/CLOCKS_PER_SEC;

					if((randTime >= (total_t - 1)) && (randTime <= (total_t + 1))){
						for (randBullets = 0; randBullets < 10; randBullets++){		//Allows CPU to shoot when user is not shooting
							bullet2Num++;
							if(bullet2Num == 100){ 
								bullet2Num = 0;
							}
							fireBullet(RED, bullet2Num);
						}
						randTime = randTime + (1 + (rand() % 3));
					}
					if(returnFireCount == 20){
						returnFireCount = 0;
						q = calcCpuDirection(redTank.xPos, redTank.yPos);
				}
			}

				gfx_color(255,255,255);
				gfx_text(3*XSIZE/4,YSIZE/8, livesStr);
			}			
		}
	}
} // End Main



/*################### FUNCTIONS ######################*/
void setTanks(void){
	blueTank.xPos = 750;
	blueTank.yPos = 550;
	blueTank.deltaX = 0;
	blueTank.deltaY = 0;
	blueTank.theta = 2.356;
	blueTank.color = BLUE;

	redTank.xPos = 50;
	redTank.yPos = 50;
	redTank.deltaX = 0;
	redTank.deltaY = 0;
	redTank.theta = 2.356;
	redTank.color = RED;

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

	checkBoard();
}

void drawTanks2(void){
	gfx_clear();
	setLocations();

	drawBodies();

	drawTreads();

	drawBullets2();

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
					blueDirection = 1;
					if( blueTank.yPos > 20 ){
						blueTank.yPos = blueTank.yPos-10;
					}
					break;
				case 's':
					blueDirection = 1;
					if( blueTank.yPos < (YSIZE - 20) ){
						blueTank.yPos = blueTank.yPos+10;
					}
					break;
				case 'a':
					blueDirection = 2;
					if( blueTank.xPos > 20 ){
						blueTank.xPos = blueTank.xPos-10;
					}
					break;
				case 'd':
					blueDirection = 2;
					if( blueTank.xPos < (XSIZE - 20) ){
						blueTank.xPos = blueTank.xPos+10;
					}
					break;
				default:
					printf("The Input is not Valid or is a Mouse Click\n");
					break;
		}
	}
	else if(color == RED){
		switch(direction){
				case 'w':
					redDirection = 1;
					if( (redTank.yPos - 27) > 0){
						redTank.yPos = redTank.yPos-6;
						drawTanks();
					}
					break;
				case 's':
					redDirection = 1;
					if( (redTank.yPos + 27) < YSIZE){
						redTank.yPos = redTank.yPos+6;
						drawTanks();
					}
					break;
				case 'a':
					redDirection = 2;
					if( (redTank.xPos - 27) > 0){
						redTank.xPos = redTank.xPos-6;
						drawTanks();
					}
					break;
				case 'd':
					redDirection = 2;
					if( (redTank.xPos + 27) < XSIZE){
						redTank.xPos = redTank.xPos+6;
						drawTanks();
					}
					break;
				default:
					printf("The Input is not Valid or is a Mouse Click\n");
					break;
		}	
	}
}

void fireBullet(int color, int bulletNumber){
	double theta1, theta2;
	int x, y, deltaX, deltaY;

	if(color == BLUE){
		bullets1[bulletNumber][0] = 1;
		bullets1[bulletNumber][1] = (double) blueTank.xPos;
		bullets1[bulletNumber][2] = (double) blueTank.yPos;

		x = gfx_xpos();
		y = gfx_ypos();
		deltaX = x - blueTank.xPos;		//difference between initial position of tank and bullet destination
		deltaY = y - blueTank.yPos;

		theta1 = atan2(deltaY, deltaX);

		bullets1[bulletNumber][3] = cos(theta1);
		bullets1[bulletNumber][4] = sin(theta1);
	}
	else if(color == RED){
		bullets2[bulletNumber][0] = 1;
		bullets2[bulletNumber][1] = (double) redTank.xPos;
		bullets2[bulletNumber][2] = (double) redTank.yPos;

		deltaX = blueTank.xPos - redTank.xPos;
		deltaY = blueTank.yPos - redTank.yPos;

		theta2 = atan2(deltaY, deltaX);

		bullets2[bulletNumber][3] = cos(theta2);
		bullets2[bulletNumber][4] = sin(theta2);
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
				printf("The blue bullet is out of bounds\n");
			}

			else if( locations[xCheck1][yCheck1] == 1 ){
				bullets1[count][0] = 0;
				redTank.lives = set_lives(redTank.lives);
				sprintf(livesStr, "Lives: Red %d || Blue %d", redTank.lives, blueTank.lives);
				setTanks();
				printf("The blue Bullet Hit the other Tank\n");
			}
		}

		if( bullets2[count][0] == 1){

			if( (xCheck2 < 0) || (xCheck2 > XSIZE ) || (yCheck2 < 0) || (yCheck2  > YSIZE ) ){
				bullets2[count][0] = 0;
				printf("The red bullet is out of bounds\n");
			}

			else if( locations[xCheck2][yCheck2] == 2 ){
				bullets2[count][0] = 0;
				blueTank.lives = set_lives(blueTank.lives);
				sprintf(livesStr, "Lives: Red %d || Blue %d", redTank.lives, blueTank.lives);
				setTanks();
				printf("The red Bullet Hit the other Tank\n");
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
			if(locations[blueTank.xPos-26+i][blueTank.yPos-22+j] != 0){
				setTanks();
			}
			else if(locations[blueTank.xPos-26+i][blueTank.yPos-22+j] != 0){
				setTanks();
			}
			else{
				locations[blueTank.xPos-26+i][blueTank.yPos-22+j] = 2;
				locations[blueTank.xPos+18+i][blueTank.yPos-22+j] = 2;
			}
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
			if(locations[blueTank.xPos-22+j][blueTank.yPos-26+i] != 0){
				setTanks();
			}
			else if(locations[blueTank.xPos-22+j][blueTank.yPos+18+i] != 0){
				setTanks();
			}
			else{
				locations[blueTank.xPos-22+j][blueTank.yPos-26+i] = 2;
				locations[blueTank.xPos-22+j][blueTank.yPos+18+i] = 2;
			}
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

void drawBullets2(void){
	int bulletCount, i, j;

	for( bulletCount = 0 ; bulletCount < 400 ; bulletCount++){
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

	for( bulletCount = 0 ; bulletCount < 400 ; bulletCount++){
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

int set_lives(int lives) {

	--lives;
	return lives;
}






