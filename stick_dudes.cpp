#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>
#include<GL/glut.h>
using namespace std;

//stick dudes color:
GLfloat color[3] = { 1, 1, 1 };
//header title:
char title[] = "Stick dudes!";
//safekeeping key events for the game:
bool strokes[256];
//keep the text displayed:
char textView[256];
//flag to check if the game is over:
bool gamePauseFlag;
//save keeping the screen vertices to show thow each player:
int firstPlayerX, firstPlayerY, secondPlayerX, secondPlayerY;
//if the player corresponding to its number is crawling:
int firstPlayerCrawlAction, firstPlayerCrawlingCount, secondPlayerCrawlAction, player2walkcounter;
//if the player corresponding to its number is punching:
bool firstPlayerPunch, secondPlayerPunch, firstPlayerWasPunching, player2waspunching;
//this var is used for the punch action (oh man):
int firstPlayerPunchAnimCounter, secondPlayerPunchAnimCounter;
//moving the player in case of being punched by the other player:
int firstPlayerCrawlBack, secondPlayerCrawlBack;
//safekeeping the player points:
int firstPlayerPoints, secondPlayerPoints;

void drawString(void * typeface, char *text, float x, float y) {

	//drawing text on the canvas:
	//the raster position for text operations (on pixels where to put the character):
	glRasterPos2f(x, y);
	for (unsigned int i = 0; i < strlen(text); i++) {

		//draw the character on the screen with the specified font:
		glutBitmapCharacter(typeface, text[i]);
	}
}

void drawCircle(float cx, float cy, float r, int numberOfLines) {

	//drawing a circle with given params .
	glBegin(GL_LINE_LOOP);
	//set the color to draw the circle:
	glColor3f(color[0], color[1], color[2]);
	for (int i = 0; i < numberOfLines; i++) {

		//get the current angle
		float theta = 2.0f * 3.1415926f * float(i) / float(numberOfLines);
		//evaluate x:
		float x = r * cosf(theta);
		//evaluate y:
		float y = r * sinf(theta);
		//create the vertex:
		glVertex2f(x + cx, y + cy);
	}
	glEnd();
}

void drawLine(GLfloat beginX, GLfloat beginY, GLfloat endX, GLfloat endY) {

	//draw a line with given params .
	glBegin(GL_LINES);
	glColor3f(color[0], color[1], color[2]);
	glVertex2f(beginX, beginY);
	glVertex2f(endX, endY);
	glEnd();
}

void drawLineStrip(GLfloat beginX, GLfloat beginY, GLfloat middleX, GLfloat middleY, GLfloat endX, GLfloat endY) {

	//draw a line-strip with given params .
	glBegin(GL_LINE_STRIP);
	glColor3f(color[0], color[1], color[2]);
	glVertex2f(beginX, beginY);
	glVertex2f(middleX, middleY);
	glVertex2f(endX, endY);
	glEnd();
}

//overload of above function in order to make it suitable for the other leg of stick.
void drawLineStrip(GLfloat beginX, GLfloat beginY, GLfloat middleOneX, GLfloat middleOneY, GLfloat middleTwoX, GLfloat middleTwoY, GLfloat endX, GLfloat endY) {

	//draw a line-strip with given params .
	glBegin(GL_LINE_STRIP);
	glColor3f(color[0], color[1], color[2]);
	glVertex2f(beginX, beginY);
	glVertex2f(middleOneX, middleOneY);
	glVertex2f(middleTwoX, middleTwoY);
	glVertex2f(endX, endY);
	glEnd();
}

//params:
//dude's x, dude's y, reflected dude flag, 0:standing/1:walk1/2:walk2, dude's punch flag:
void drawDude(int x, int y, bool mirror, int walkaction, bool punch) {

	//draw the stick dudes:
	//load the identity matrix:
	glLoadIdentity();
	glTranslatef(x, y, 0);

	//stick dude 2 (mirror of stick dude 1):
	if (mirror == true) {
	
		//reverse scale the shape:
		glScalef(-1, 1, 1);
	}
	
	//set the line size:
	glLineWidth(2);

	//head of the stick:
	drawCircle(0, -10, 10, 10);

	//the body of stick:
	drawLine(0, 0, 0, 30);

	if (walkaction == 1) {\

		//walk action 1 (open legs)
		//draw the stick's legs:
		//leg 1:
		drawLineStrip(0, 30, 0, 60, 10, 60);
		//leg 2:
		drawLineStrip(0, 30, 15, 45, -3, 48, 2, 57);
	} else if (walkaction == 2) {

		//walk action 2 (close legs):
		//draw the stick's legs:
		//leg 1:
		drawLineStrip(0, 40, -10, 55, -3, 60);
		//leg 2:
		drawLineStrip(0, 30, 10, 60, 20, 55);
	} else {

		//stick man is standing
		//both legs in same row:
		drawLineStrip(0, 30, 0, 60, 10, 60);
	} if (punch == true) {

		//show the punch action (not good for indoor use):
		drawLineStrip(0, 20, 5, 30, 10, 15);
		drawLine(0, 20, 40, 20);
	} else {

		//else no punch action just standing
		drawLineStrip(0, 20, 10, 30, 20, 15);
		drawLineStrip(0, 20, 15, 30, 25, 15);
	}

	//load identity matrix again to refresh the view:
	glLoadIdentity();
}

//update the position of players:
void updategame(int value) {

	//registers a timer callback to be triggered in a specified number of milliseconds.
	//update the game is every 10 milliseconds:
	glutTimerFunc(10, updategame, 0);

	if (gamePauseFlag == false) {

		//check if movement buttons are pressed:
		if (strokes['a'] == true || strokes['d'] == true) {

			//check if first player has hit the left edge if not, then crawl:
			if (strokes['a'] == true && firstPlayerX > 10) {

				//crawl left the first player:
				firstPlayerX = firstPlayerX - 1;

				if (firstPlayerCrawlAction == 0) {

					//do the crawl action if the dude is standing:
					//first crawl action:
					firstPlayerCrawlAction = 1;
					//animate the crawling action:
					firstPlayerCrawlingCount = 25;
				} else if (firstPlayerCrawlAction == 1 && firstPlayerCrawlingCount == 0) {

					firstPlayerCrawlingCount = 25;
					//crawling act two:
					firstPlayerCrawlAction = 2;
				} else if (firstPlayerCrawlAction == 2 && firstPlayerCrawlingCount == 0) {

					//swap the crawling action id:
					firstPlayerCrawlingCount = 25;
					firstPlayerCrawlAction = 1;
				} else {

					//player1walkcounter is used so we can see both walk actions
					firstPlayerCrawlingCount = firstPlayerCrawlingCount - 1;
				}
			}

			//move first player to right if there is enough room .
			//630 is the pixel number of the right edge .
			if (strokes['d'] == true && firstPlayerX < 630) {

				firstPlayerX = firstPlayerX + 1;

				if (secondPlayerX - firstPlayerX < 20) {

					//push the second player when they're face to face.
					secondPlayerX = firstPlayerX + 20;
				}

				if (firstPlayerCrawlAction == 0) {

					//switch between crawling actions and update the counter:
					firstPlayerCrawlAction = 1;
					firstPlayerCrawlingCount = 20;
				} else if (firstPlayerCrawlAction == 1 && firstPlayerCrawlingCount == 0) {

					//switch between crawling actions and update the counter:
					firstPlayerCrawlingCount = 20;
					firstPlayerCrawlAction = 2;
				} else if (firstPlayerCrawlAction == 2 && firstPlayerCrawlingCount == 0) {

					//switch between crawling actions and update the counter:
					firstPlayerCrawlingCount = 20;
					firstPlayerCrawlAction = 1;
				} else {

					//decrement the crawling count:
					firstPlayerCrawlingCount = firstPlayerCrawlingCount - 1;
				}
			}
		}
		
		else {

			//stand if there is no crawling buttons pressed:
			firstPlayerCrawlAction = 0;
		}

		//handle second player's movements:
		if (strokes['4'] == true || strokes['6'] == true) {

			if (strokes['4'] == true && secondPlayerX > 10)	{

				// check if second player has hit the right edge if not, then crawl:
				secondPlayerX = secondPlayerX - 1;

				if (secondPlayerX - firstPlayerX < 20) {

					//push the first player when they're face to face.
					firstPlayerX = secondPlayerX - 20;
				}
				
				if (secondPlayerCrawlAction == 0) {

					//switch the crawling actions and update the counter.
					secondPlayerCrawlAction = 1;
					player2walkcounter = 20;
				} else if (secondPlayerCrawlAction == 1 && player2walkcounter == 0) {

					//switch the crawling actions and update the counter.
					player2walkcounter = 20;
					secondPlayerCrawlAction = 2;
				} else if (secondPlayerCrawlAction == 2 && player2walkcounter == 0) {

					//switch the crawling actions and update the counter.
					player2walkcounter = 20;
					secondPlayerCrawlAction = 1;
				} else {

					//decrease the counter:
					player2walkcounter = player2walkcounter - 1;
				}
			}
			
			//check if the second dude has hit the wall near him:
			if (strokes['6'] == true && secondPlayerX < 630) {

				//if not, then move:
				secondPlayerX = secondPlayerX + 1;

				if (secondPlayerCrawlAction == 0) {

					//switch the crawling actions and update the counter.
					secondPlayerCrawlAction = 1;
					player2walkcounter = 20;
				} else if (secondPlayerCrawlAction == 1 && player2walkcounter == 0) {

					//switch the crawling actions and update the counter.
					player2walkcounter = 20;
					secondPlayerCrawlAction = 2;
				} else if (secondPlayerCrawlAction == 2 && player2walkcounter == 0) {

					//switch the crawling actions and update the counter.
					player2walkcounter = 20;
					secondPlayerCrawlAction = 1;
				} else {

					//decrement the counter:
					player2walkcounter = player2walkcounter - 1;
				}
			}
		} else {

			//if there's no buttons being pressed, then the dude should stand:
			secondPlayerCrawlAction = 0;
		}

		//check if space button is being pressed:
		if (strokes[' '] == true && firstPlayerPunch == false && firstPlayerWasPunching == false) {

			//activate the punch:
			firstPlayerPunch = true;
			//set the counter to keep the punch:
			firstPlayerPunchAnimCounter = 10;
		} else if (firstPlayerPunchAnimCounter > 0) {

			//keep the punch up:
			firstPlayerPunchAnimCounter = firstPlayerPunchAnimCounter - 1;
		} else {
		
			//deactivate the punch! :)) :
			firstPlayerPunch = false;
		}

		//set the punch flag for first dude:
		firstPlayerWasPunching = strokes[' '];

		//check if the enter button is being pressed:
		if (strokes[13] == true && secondPlayerPunch == false && player2waspunching == false) {

			//activate the punch:
			secondPlayerPunch = true;
			//set the counter to keep the punch:
			secondPlayerPunchAnimCounter = 10;
		} else if (secondPlayerPunchAnimCounter > 0) {

			//keep the punch up:
			secondPlayerPunchAnimCounter = secondPlayerPunchAnimCounter - 1;
		} else {

			//deactivate the punch:
			secondPlayerPunch = false;
		}

		//set the punch flag for second dude:
		player2waspunching = strokes[13];

		//if player1 was hit move him back
		firstPlayerX = firstPlayerX - firstPlayerCrawlBack;

		//check if second dude has hit the first dude:
		if (secondPlayerX - firstPlayerX < 40 && secondPlayerX - firstPlayerX > 0 && secondPlayerPunch == true && firstPlayerCrawlBack == 0) {

			//decrease the points of first player:
			firstPlayerPoints = firstPlayerPoints - 10;

			if (firstPlayerPoints <= 0) {
			
				//check if the second dude is won:
				gamePauseFlag = true;
			}

			//if the first dude is still alive, then crawl him back:
			firstPlayerCrawlBack = 10;
		}

		if (firstPlayerCrawlBack > 0) {

			//if still room, then crawl more:
			firstPlayerCrawlBack = firstPlayerCrawlBack - 1;
		}

		//if the second dude was hit, then crawl him back:
		secondPlayerX = secondPlayerX + secondPlayerCrawlBack;


		//check if first dude hit the second:
		if (secondPlayerX - firstPlayerX < 40 && secondPlayerX - firstPlayerX > 0 && firstPlayerPunch == true && secondPlayerCrawlBack == 0) {

			//decrease the second dude's points:
			secondPlayerPoints = secondPlayerPoints - 10;

			if (secondPlayerPoints <= 0) {
			
				//check if the first dude is won:
				gamePauseFlag = true;
			}

			//if the second dude is still alive, then crawl him back:
			secondPlayerCrawlBack = 10;
		}

		if (secondPlayerCrawlBack > 0) {

			//if still room, then crawl more:
			secondPlayerCrawlBack = secondPlayerCrawlBack - 1;
		}
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	//black (landscape):
	glClearColor(0.0, 0.0, 0.0, 1);
	//grey (playground):
	glBegin(GL_QUADS);
	glColor3f(0.1, 0.1, 0.1);
	glVertex2f(0, 360);
	glVertex2f(0, 480);
	glVertex2f(640, 480);
	glVertex2f(640, 360);
	glEnd();

	//display first dude's points:
	glColor3f(0, 1, 0);
	//set fonts, text, x, y to display the text:
	drawString(GLUT_BITMAP_HELVETICA_18, "Player1:", 40, 20);
	//convert firstPlayerPoints to string and save to textView variable:
	itoa(firstPlayerPoints, textView, 10);
	//draw the string on the display by setting its fonts, text, x, y to display the text:
	drawString(GLUT_BITMAP_HELVETICA_18, textView, 150, 20);

	//display second dude's points:
	glColor3f(0, 1, 0);
	drawString(GLUT_BITMAP_HELVETICA_18, "Player2:", 440, 20);
	//convert firstPlayerPoints to string and save to textView variable:
	itoa(secondPlayerPoints, textView, 10);
	//draw the string on the display:
	drawString(GLUT_BITMAP_HELVETICA_18, textView, 550, 20);

	//draw the stick dudes:
	//dude 1:
	drawDude(firstPlayerX, firstPlayerY, false, firstPlayerCrawlAction, firstPlayerPunch);
	//dude 2:
	drawDude(secondPlayerX, secondPlayerY, true, secondPlayerCrawlAction, secondPlayerPunch);

	//if the game is paused:
	if (gamePauseFlag == true) {

		glColor3f(1, 1, 1);

		if (firstPlayerPoints == 0) {
			
			//show the winning dialog for the second dude:
			glColor3f(0, 1, 0);
			drawString(GLUT_BITMAP_HELVETICA_18, "Player2 Wins", 260, 260);
		} else {
		
			//show the winning dialog for the first dude:
			glColor3f(0, 1, 0);
			drawString(GLUT_BITMAP_HELVETICA_18, "Player1 Wins", 260, 260);
		}

		//show the game restart messages:
		glColor3f(0, 1, 0);
		drawString(GLUT_BITMAP_HELVETICA_18, "Game Over", 270, 280);
		drawString(GLUT_BITMAP_HELVETICA_18, "Do you want to continue(y/n)", 200, 300);
	}

	//swap the buffers of the current window because there are two buffers
	//and the display needs to be refreshed more preciesly:
	glutSwapBuffers();
}

void reshape(int x, int y)
{
	//handle the reshapes when the user changes the window size .
	//set the dimentions of the window:
	glViewport(0, 0, x, y);

	glMatrixMode(GL_PROJECTION);
	//load the identity matrix:
	glLoadIdentity();
	//set the screen resoloution:
	glOrtho(0, 640, 480, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
}

//init the game and players:
void initgame()
{
	firstPlayerX = 240;
	secondPlayerX = 400;
	firstPlayerY = 300;
	secondPlayerY = 300;
	firstPlayerPunch = 0;
	secondPlayerPunch = 0;
	firstPlayerCrawlBack = 0;
	secondPlayerCrawlBack = 0;
	firstPlayerCrawlAction = 0;
	secondPlayerCrawlAction = 0;
	firstPlayerPoints = 120;
	secondPlayerPoints = 120;
}

//keyboard callback to handle key release events .
void keyReleaseEvent(unsigned char key, int x, int y) {
	switch (key) {

	//unregister player 1 keys:
	case 'w':
		//up:
		strokes['w'] = false;

		break;

	case 's':
		//down:
		strokes['s'] = false;
		break;

	case 'a':
		//left:
		strokes['a'] = false;
		break;

	case 'd':
		//right:
		strokes['d'] = false;
		break;

	//Unregister player 2 keys:
	case '8':
		//up:
		strokes['8'] = false;
		break;

	case '2':
		//down:
		strokes['2'] = false;
		break;

	case '4':
		//left:
		strokes['4'] = false;
		break;

	case '6':
		//right:
		strokes['6'] = false;
		break;


	case ' ':
		//player 1 punch:
		strokes[' '] = false;
		break;

	case 13:
		//player 2 punch (enter):
		strokes[13] = false;
		break;

	case 'c':
		//continue:
		break;

	case 27:
		//escape:
		exit(0);

	}
}

//keyboard callback to handle key push events .
void keyPushEvent(unsigned char key, int x, int y) {
	switch (key) {
		//handle the key events:

	case 'w':
		//player 1 up key:
		strokes['w'] = true;
		break;

	case 's':
		//player 1 down key:
		strokes['s'] = true;
		break;

	case 'a':
		//player 1 left key:
		strokes['a'] = true;
		break;

	case 'd':
		//player 1 right key:
		strokes['d'] = true;
		break;

	case '8':
		//player 2 up key:
		strokes['8'] = true;
		break;

	case '2':
		//player 2 down key:
		strokes['2'] = true;
		break;

	case '4':
		//player 2 left key:
		strokes['4'] = true;
		break;

	case '6':
		//player 2 right key:
		strokes['6'] = true;
		break;

	case ' ':
		//case of space (player 1 punch):
		strokes[' '] = true;
		break;

	case 13:
		//case of enter (player 2 punch):
		strokes[13] = true;
		break;

	case 'y':
		//restart or continue the game:
		if (gamePauseFlag == true)
		{
			gamePauseFlag = false;
			initgame();
		}
		break;

	case 'n':
		//answer to close the game dialog:
		if (gamePauseFlag == true)
			exit(0);
		break;
	case 27:
		//case of escape button:
		exit(0);

	}

}

void main(int argc, char **argv) {

	initgame(); //call init function to initialize the game
	glutInit(&argc, argv);
	//use rgb color + double buffering:
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	//initial window's position:
	glutInitWindowPosition(100, 100);
	//initial window size:
	glutInitWindowSize(640, 480);
	//create the window with given title:
	glutCreateWindow(title);
	//register the reshape callback:
	glutReshapeFunc(reshape);
	//register the display callback:
	glutDisplayFunc(display);
	//register the keyboard key release event callback:
	glutKeyboardFunc(keyPushEvent);
	//register the keyboard key push event callback:
	glutKeyboardUpFunc(keyReleaseEvent);
	//register the idle event callback:
	glutIdleFunc(display);
	//update the game in each 10 milliseconds:
	glutTimerFunc(10, updategame, 0);
	//start the glut main loop:
	glutMainLoop();
}
