// main.cpp

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <time.h>

#include "glut.h"
#include "Game.h"

using namespace std;

const int BLOCKSIZE = 40;
const int VPWIDTH = COLS * BLOCKSIZE;
const int VPHEIGHT = ROWS * BLOCKSIZE;
int display_flag = 0; // to display the starting screens.

Game game;
GLvoid* font_style = GLUT_BITMAP_TIMES_ROMAN_24;

// Displaying text on the screen
void BitmapText(char *str, int wcx, int wcy)
{
	glRasterPos2i(wcx, wcy);
	for (int i=0; str[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
	}
}

// Getting a random number in the interval [0, 1]
double random()
{ 
	int r = rand();
	return double(r) / RAND_MAX;
}

// Callback function, timer
void timer(int id)
{
	if (game.killed) {	// If the game is over

		game.paused = true;
		//glutTimerFunc(200, timer, id);
		game.clearMainGrid();
		game.clearNextPieceGrid();
		glutPostRedisplay();

	} else if (!game.paused) {	// If the game continues and is not paused
		game.update();		
		if (game.killed) {
			glutTimerFunc(10, timer, 1);
		} else {
			glutPostRedisplay();
			//game.timer = game.timer * 0.95;
			glutTimerFunc(game.timer, timer, 0);
		}
	}
}

// Callback function, handling keyboard actions
void keyboard(unsigned char key, int x, int y)
{
	if (display_flag == 0) {
		if (key == 13) {
			display_flag++;
		}
	}
	else if (display_flag == 1) {
		if (key == 13) {
			display_flag++;
			game.paused = false;
		}
	}
	else {
		if (game.paused && game.killed) {
			if (key == 13) { // 13 == ENTER
				game.killed = false;
				game.restart();
				glutTimerFunc(game.timer, timer, 0);
			}
		}
		else {
			if (key == 'p' || key == 27) { // 27 == ESCAPE
				game.paused = !game.paused;
				if (!game.paused)
					glutTimerFunc(game.timer, timer, 0);
			}
			else if (!game.paused && !game.killed && key == ' ') { // ' ' == SPACE
				game.update();
			}
		}
	}

	glutPostRedisplay();
}

// Callback function, handling arrow actions
void special(int key, int x, int y)
{
	if (!game.paused && !game.killed) {
		if (key == GLUT_KEY_LEFT) {
			game.move(-1);
			glutPostRedisplay();
		} else if (key == GLUT_KEY_RIGHT) {
			game.move(1);
			glutPostRedisplay();
		} else if (key == GLUT_KEY_UP) {
			game.rotateShape(1);
			glutPostRedisplay();
		} else if (key == GLUT_KEY_DOWN) {
			game.rotateShape(-1);
			glutPostRedisplay();
		}
	}
}

//mouse function:-
void mouse(int btn,int state,int xmouse, int ymouse) {
	if (display_flag == 0) {
		if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			display_flag++;
			glutPostRedisplay();
		}
	}
	else if (display_flag == 1) {
		if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			display_flag++;
			game.paused = false;
			//game.paused = !game.paused;
			glutPostRedisplay();
		}
	}
}

// Callback function, displaying the gameplay window
void display(void)
{
	const int N = 100;
	char msg[N + 1];

	glClearColor(0.0f, 0.0f, 0.0f, 0.72);
	glClear(GL_COLOR_BUFFER_BIT);

	// Grid
	glViewport(0, 0, VPWIDTH, VPHEIGHT);
	glMatrixMode(GL_PROJECTION);

	if (display_flag == 0) {
		game.paused = true;
		glLoadIdentity();
		gluOrtho2D(0, VPWIDTH*2, 0, VPHEIGHT);
		glColor3f(0, 1, 1);
		sprintf_s(msg, N, "tetris");
		BitmapText(msg, VPWIDTH + 370, VPHEIGHT / 2 + 20);
		sprintf_s(msg, N, "opengl - mini project");
		BitmapText(msg, VPWIDTH  + 250, VPHEIGHT / 2 - 20);

	}
	else if (display_flag == 1) {
		glLoadIdentity();
		gluOrtho2D(0, VPWIDTH*2, 0, VPHEIGHT);
		glColor3f(1, 1, 0);
		sprintf_s(msg, N, "press [ENTER] to start");
		BitmapText(msg, VPWIDTH + 250, VPHEIGHT/2 - 10);
	}
	else {
		if (!game.paused) {	// If the game is active, the movement of the pieces is carried out

			glLoadIdentity();
			gluOrtho2D(0, COLS, ROWS, 0);

			for (int r = 0; r < ROWS; r++) {
				for (int c = 0; c < COLS; c++) {
					Square& square = game.mainGrid[r][c];
					if (square.isFilled) {
						glColor3f(square.red, square.green, square.blue);
						glRectd(c + .1, r + .1, c + .9, r + .9);
					}
					else {
						glColor3f(0.1, 0.1, 0.1);
						glRectd(c + .1, r + .1, c + .9, r + .9);
					}
				}
			}
		}
		else {

			glLoadIdentity();
			gluOrtho2D(0, VPWIDTH, 0, VPHEIGHT);

			if (game.paused && !game.killed) {		// If the game is paused, the pause menu is displayed
				glColor3f(1, 1, 1);
				sprintf_s(msg, N, "GAME PAUSED");
				BitmapText(msg, 140, VPHEIGHT / 2);
			}
			if (game.paused && game.killed) {		// If the game is over, the restart menu is displayed
				glColor3f(1, 1, 1);
				sprintf_s(msg, N, "GAME OVER");
				BitmapText(msg, 155, VPHEIGHT / 2 + 50);
				sprintf_s(msg, N, "YOUR SCORE: %d", game.linesCleared);
				BitmapText(msg, 140, VPHEIGHT / 2);
				sprintf_s(msg, N, "Press [ENTER] to restart ...");
				BitmapText(msg, 75, VPHEIGHT / 2 - 100);
			}
		}

		// Vertical dividing strip
		glViewport(VPWIDTH, 0, VPWIDTH, VPHEIGHT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, VPWIDTH, 0, VPHEIGHT);

		glBegin(GL_LINES);
		glColor3f(1.0, 1.0, 1.0);
		glVertex2d(1, 0);
		glVertex2d(1, glutGet(GLUT_WINDOW_HEIGHT));
		glEnd();

		// Messages on the right side of the screen
		glLoadIdentity();
		gluOrtho2D(0, VPWIDTH, 0, VPHEIGHT);

		glColor3f(1, 1, 0);
		sprintf_s(msg, N, "options:-");
		BitmapText(msg, 50, 450);
		sprintf_s(msg, N, "p or 'ESC' to pause/play");
		BitmapText(msg, 50, 400);
		sprintf_s(msg, N, "<left> arrow to move left");
		BitmapText(msg, 50, 370);
		sprintf_s(msg, N, "<right> arrow to move right");
		BitmapText(msg, 50, 340);
		sprintf_s(msg, N, "<up> arrow to rotate clockwise");
		BitmapText(msg, 50, 310);
		sprintf_s(msg, N, "<down> arrow to rotate anti-clockwise");
		BitmapText(msg, 50, 280);
		glColor3f(0, 1, 1);
		sprintf_s(msg, N, "lines Cleared: %d", game.linesCleared);
		BitmapText(msg, 50, 100);
		sprintf_s(msg, N, "shapes Encountered: %d", game.shapesCount);
		BitmapText(msg, 50, 50);
		sprintf_s(msg, N, "next Shape:");
		BitmapText(msg, 50, VPHEIGHT - 50);

		// Vertical dividing strip
		glBegin(GL_LINES);
		glColor3f(1.0, 1.0, 1.0);
		glVertex2d(1, 0);
		glVertex2d(1, glutGet(GLUT_WINDOW_HEIGHT));
		glEnd();

		// Grid displaying the next piece
		glViewport(VPWIDTH + 50, -50, VPWIDTH, VPHEIGHT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, COLS, ROWS, 0);

		for (int r = 1; r < 5; r++) {
			for (int c = 0; c < 2; c++) {
				Square& square = game.nextPieceGrid[r][c];
				if (square.isFilled) {
					glColor3f(square.red, square.green, square.blue);
					glRectd(c + .1, r + .1, c + .9, r + .9);
				}
				else {
					glColor3f(0.1, 0.1, 0.1);
					glRectd(c + .1, r + .1, c + .9, r + .9);
				}
			}
		}

	}
	glutSwapBuffers();
}

void main(int argc, char *argv[])
{
	srand(time(0));
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);	

	//glutInitWindowPosition(100, 100);
	glutInitWindowSize(VPWIDTH*2, VPHEIGHT);

	glutCreateWindow("Tetris");

	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutTimerFunc(game.timer, timer, 0);

	glutMainLoop();
}
