#include <iostream>
#include <GL/glut.h>
using namespace std;


int DELAY = 80;
int N = 20;
int SQUARE_SIZE = 25;
int wh = SQUARE_SIZE*N;
int LEN = 2;
int POINT;

const int POINT_UP = 0;
const int POINT_LEFT = 1;
const int POINT_RIGHT = 2;
const int POINT_DOWN = 3;

class SnakeItem {

public:
	int x;
	int y;

};

class Apple {

public:
	int x;
	int y;

	Apple() {
		this->x = rand() % N; //a random number between 0 and N .
		this->y = rand() % N;
	}

	void showApple() {
		glColor3f(1.0, 0.0, 0.0);
		//draw a rectangle with one single vertex .
		glRectf(x*SQUARE_SIZE,
			y*SQUARE_SIZE,
			(x + 1)*SQUARE_SIZE,
			(y + 1)*SQUARE_SIZE);
	}
};

Apple *apple;   //create a pointer to an Apple object .

SnakeItem snake[100];   //create a SnakeItem array .

void drawBoard() {   //creates the game board .

	glBegin(GL_LINES);  //draws a line between vertices .
	glColor3f(0.3, 0.3, 0.3);   //grey color .

	for (int i = 0; i<wh; i += SQUARE_SIZE) {   //draws the vertical lines . |
		glVertex2f(i, 0);
		glVertex2f(i, wh);
	}

	for (int j = 0; j<wh; j += SQUARE_SIZE) {   //draws the horizontal lines . _
		glVertex2f(0, j);
		glVertex2f(wh, j);
	}

	glEnd();    //finish drawing lines .
}

void putSnake() {    //updates the snake position on the game board .


	for (int i = 0; i<LEN; i++) {
		//drawing snake items on the board .

		//change snake's head color:
		glColor3f(0.0, 0.0, 1.0);   //blue color .

		if (i != 0) { //color of snake (except its head)
			glColor3f(0.0, 1.0, 0.0);   //green color .
		}

		glRectf((snake[i].x + 0.15) * SQUARE_SIZE,  //draw snake items .
			(snake[i].y + 0.15) * SQUARE_SIZE,
			(snake[i].x + 0.85) * SQUARE_SIZE,
			(snake[i].y + 0.85) * SQUARE_SIZE);
	}
}

void moveSnakeItem() {

	for (int i = LEN; i>0; --i) {

		snake[i].x = snake[i - 1].x;
		snake[i].y = snake[i - 1].y;
	}

	/* snake[0] is pointing to head of snake*/

	if (POINT == POINT_UP) {

		snake[0].y += 1;
	}

	if (POINT == POINT_LEFT) {

		snake[0].x -= 1;
	}
	if (POINT == POINT_RIGHT) {

		snake[0].x += 1;
	}
	if (POINT == POINT_DOWN) {

		snake[0].y -= 1;
	}

	if ((snake[0].x == apple->x) && (snake[0].y == apple->y)) {
		LEN++;
		apple = new Apple();    //creates another random apple on the board .
	}
	// change direction if snake goes out
	if (snake[0].x > N) {

		POINT = POINT_LEFT;
	}
	if (snake[0].x < 0) {

		POINT = POINT_RIGHT;
	}
	if (snake[0].y > N) {

		POINT = POINT_DOWN;
	}
	if (snake[0].y < 0) {

		POINT = POINT_UP;
	}

	for (int i = 1; i<LEN; i++) {
		if (snake[0].x == snake[i].x
			&& snake[0].y == snake[i].y) { //in case of the snake bites itself !

			LEN = i; //reduce the length .
		}
	}
}

void display() {

	glClear(GL_COLOR_BUFFER_BIT);

	drawBoard();
	putSnake();
	apple->showApple();

	glFlush();

}

void timer(int) {
	display();    //call display function in timer callback .
	moveSnakeItem();  //move the snake cosidering the POINT .
	glutTimerFunc(DELAY, timer, 0); //re-sign the timer callback .
}

void keyEvent(int key, int a, int b) {   //keyboard callback function .

	switch (key) {   //checks which key has been pressed .

	case GLUT_KEY_UP:
		if (POINT != POINT_DOWN) {

			POINT = POINT_UP;
		}
		break;

	case GLUT_KEY_RIGHT:
		if (POINT != POINT_LEFT) {

			POINT = POINT_RIGHT;
		}
		break;

	case GLUT_KEY_LEFT:
		if (POINT != POINT_RIGHT) {

			POINT = POINT_LEFT;
		}
		break;

	case GLUT_KEY_DOWN:
		if (POINT != POINT_UP) {

			POINT = POINT_DOWN;
		}
		break;
	}
}

void menuRes(int item) { //handles how menu resources respond to click events .

	switch (item) {

	case '0': DELAY = 250;
		break;

	case '1': DELAY = 80;
		break;

	case '2': DELAY = 40;
		break;

	case '3': exit(0);  //close the canvas .
		break;
	}
}

int main(int argc, char** argv)
{
	//init random seed for rand() function .
	//srand(time(NULL));

	glutInit(&argc, argv);  //init before using Display .
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  //init mode .
	glutInitWindowSize(wh, wh);    //assigning widow size .
	glutCreateWindow("Snake");  //window title .
	glClearColor(0.0, 0.0, 0.0, 0.0);  //board color .

	glMatrixMode(GL_PROJECTION);    //set the matrix mode .
	glLoadIdentity();   //load identity matrix .
	gluOrtho2D(0, wh, 0, wh);  //initializing ortho .
	glutDisplayFunc(display);   //registering display callback .
	glutSpecialFunc(keyEvent);  //registering keyboard event callback .

								//menu resource:
	glutCreateMenu(menuRes);    //registering right-click menu resource callback .
	glutAddMenuEntry("Easy", '0');   //menu items .
	glutAddMenuEntry("Hard", '1');
	glutAddMenuEntry("Overkill", '2');
	glutAddMenuEntry("Exit", '3');

	//make menu inflation, sensitive to right click .
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	apple = new Apple();    //create a new apple for the beginning of the game .
	glutTimerFunc(DELAY, timer, 0);  //registering timer callback .
	glutMainLoop(); //main loop .
}

