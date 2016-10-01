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
#define BULLETMAX 100
#define UP 'A';
#define DOWN 'B';
#define LEFT 'D';
#define RIGHT 'C';

void fireBullet(int);
void setLocations(void);
void setTanks(void);
void drawTanks(void);
void applyChange(char, int);
void checkBoard(void);
void drawBodies(void);
void drawTreads(void);
void drawBullets(void);
void drawBullets2(void);
char calcCpuDirection(int, int);
int set_lives(int);
void drawScore(void);

int locations[YSIZE][XSIZE];
double bullets1[100][5]; //BLUE TANK BULLETS - Whether it's on the board, x position, y position, delta x, delta y
double bullets2[100][5]; //RED TANK BULLETS - Whether it's on the board, x position, y position, delta x, delta y
int redOrientation = 1; //1 equal to moving up / down, 2 equal to moving side to side
int blueOrientation = 1;

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
	int lives; //Tanks is a strcut that, when graphed is 36 units wide, with 8 wide streds on either side that stick out four on each side in the direction of the tank
} blueTank, redTank;




int main(void){

//############### This Part Sets Up the Code, and allows the player to select a difficulty ########

	//######### VARIABLES & Launch Code################ This part of the code sets up the program
		redTank.lives = 10;
		blueTank.lives = 10;
		int bullet1Num = 0;												//  bullet 1 Num    = The number bullet from the blue tank
		int bullet2Num = 0;												//  bullet 2 Num    = The number buller fired from the red tank
		int returnFireCount = 0;										//  returnFireCount = Keeps track of how often the Red tank should fire without being prompted
		char c;															//  c               = The User Input In Game
		char d = ' ';															//  d               = The Direction the Tanks is moving in 
		char q;															//  q               = The Difficulty Selection
		int randTime = (1 + (rand() % 3));								//  randTime        = Random numner 1, 2, or 3.

	
	// Sets the opening Board
		gfx_open(XSIZE, YSIZE, "TANKS");
		setTanks();
		drawTanks();
	
	//################### ONCE THE INITITALS ARE SET UP THE PROGRAMS ASKS FOR USER INPUT #########################
/*If the User selects 1, then the code will run -Level - 1 - explained later. If it selects Level 3, then the same code at difficulty - Level 2.*/

	while(c != 'q'){

		q = gfx_wait();

		if(q == 1){
			while(1){
				if ( (redTank.lives == 0) || (blueTank.lives == 0) ){
					break;
				}

	/*######################## LEVEL - 1 - ##################################################
	If the user clicks on something Level - 2 - Begins, If they do not, then Level - 3 Begins */


		/*######################## LEVEL - 2 - ################################################
		The User Inputs Something
		 If the user selects q, the while loop exits and the program ends. If the user selects 1 (left click) - 2(A) - occurs. 
		 if the user selects anyghing else - 2(B) occurs.*/
				
				if( gfx_event_waiting() ){
			    	c = gfx_wait();
	
					    	if(c == 'q'){   //Quits the program if they click q
								break;
							}

					/*############################ -LEVEL 2(A) - ##########################
				 	 If the User enters fire ( in which case the CPU does too) bullet1 and are bullet2 is fired respectively.*/
							else if( c == 1){  
								fireBullet(BLUE);
								fireBullet(RED);
							}
					/*#################################### - LEVEL 2(B) - ########################## 
					 If the User enters a number, and it is not not 'fire' or 'quit', then the function applyChange moves the tanks, 
					 as log as it's a legitimate inut, ignores is if it is not.*/
							else{ 
								applyChange(c , BLUE);
							}
				}
				/*########################## - LEVEL 3 - ########################################
				THe User Does Not Input Something
			     If there is no button click, it waits for a tiny bit of time (becaucse the majority of the time it will register 'no input')
			     and then draws the tanks in the same poision they were, and then applies the change in position of the red tank.*/		
				
				else{ 
							usleep(8000);
							applyChange(d , RED);
							returnFireCount++;

							//Decides where the CPU will move randometly
							if(returnFireCount == 4){
								if(randTime == 2){
									srand(time(NULL));
									randTime =(1 + (rand() % 3));
									d = calcCpuDirection(redTank.xPos, redTank.yPos);

								}
							}
							
							//Fires Every Random 5 Iterations
							if(returnFireCount == 5){							
									returnFireCount = 0;
									fireBullet(RED);
							}
				}


			drawTanks();

			}
		}
	/*################################ END OF LEVEL - 1 - #######################################*/
	/*The End of level one can only be reached if the user enters 'c'*/


/*####################################################################################################################*/
	}//End of the "Difficulty While" Loop 
} // End Main



/*################### FUNCTIONS ######################*/
void setTanks(void){
	blueTank.xPos = 750;
	blueTank.yPos = 650;
	blueTank.deltaX = 0;
	blueTank.deltaY = 0;
	blueTank.theta = 2.356;
	blueTank.color = BLUE;
	blueTank.lives = 10;

	redTank.xPos = 150;
	redTank.yPos = 150;
	redTank.deltaX = 0;
	redTank.deltaY = 0;
	redTank.theta = 2.356;
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

	checkBoard();
	drawScore();
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
/*apply change changes the positions of the tanks, depending on whether or not they CAN move in the desired direction
blueOrientation / redOrientation  = Which way the treads are faceing. 1 = up and down, 2 = side to side*/
	if(color == BLUE){
		switch(direction){
				case 'w':
					blueOrientation = 1;
					if( (blueTank.yPos - 34) > 0){
						blueTank.yPos = blueTank.yPos-10;
					break;
				}
				case 's':
					blueOrientation = 1;
					if( (blueTank.yPos + 34) < (YSIZE) ){
						blueTank.yPos = blueTank.yPos+10;
					}
					break;
				case 'a':
					blueOrientation = 2;
					if( (blueTank.xPos - 34) > 0 ){
						blueTank.xPos = blueTank.xPos-10;
					}
					break;
				case 'd':
					blueOrientation = 2;
					if( (blueTank.xPos + 34) < XSIZE ){
						blueTank.xPos = blueTank.xPos+10;
					}
					break;
				default:
					break;
		}
	}
	else if(color == RED){
		switch(direction){
				case 'w':
					redOrientation = 1;
					if( (redTank.yPos - 40) > 0){
						redTank.yPos = redTank.yPos-6;
					}
					break;
				case 's':
					redOrientation = 1;
					if( (redTank.yPos + 40) < YSIZE){
						redTank.yPos = redTank.yPos+6;
					}
					break;
				case 'a':
					redOrientation = 2;
					if( (redTank.xPos - 40) > 0){
						redTank.xPos = redTank.xPos-6;
					}
					break;
				case 'd':
					redOrientation = 2;
					if( (redTank.xPos + 40) < YSIZE){
						redTank.xPos = redTank.xPos+6;;
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
				printf("The blue bullet is out of bounds\n");
			}

			else if( locations[xCheck1][yCheck1] == 1 ){
				bullets1[count][0] = 0;
				redTank.lives = set_lives(redTank.lives);
				printf("#1\n");
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
				printf("#2\n");	
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
			printf("#3\n");
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
	if(blueOrientation == 1){
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
	else if(blueOrientation == 2){
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
			bullets1[bulletCount][1] = bullets1[bulletCount][1] + 2.5*bullets1[bulletCount][3];
			bullets1[bulletCount][2] = bullets1[bulletCount][2] + 2.5*bullets1[bulletCount][4];
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
			bullets2[bulletCount][1] = bullets2[bulletCount][1] + 2.5*bullets2[bulletCount][3];
			bullets2[bulletCount][2] = bullets2[bulletCount][2] + 2.5*bullets2[bulletCount][4];
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
			direction = ' ';
			boolean = 1;
		}
	}
	return(direction);
}

int set_lives(int lives) {

	--lives;
	return lives;
}

void drawScore(void){
	char livesStr[30];
	sprintf(livesStr, "Lives: Red %d || Blue %d", redTank.lives, blueTank.lives);
	gfx_color(255,255,255);
	gfx_text(3*XSIZE/4,YSIZE/8, livesStr);
}









