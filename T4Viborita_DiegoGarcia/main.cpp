/*
 * Snake 2D
 *
 * Written by Diego Alfonso García Mendiburu 1162205
 *
 * Classic Snake 2d Game
 *
 *
 */
#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

GLint delta = 1;
GLint paused = 0;
GLint gameOver = 0;
GLint dir = 1; // North = 0, East = 1, South = 2, West = 3
GLint score = 0;
GLint snakeSize = 1;
GLint canTurn = 1;
GLdouble appleX= 0;
GLdouble appleY= 0;

struct dot{
    GLint x;
    GLint y;
    GLint xPast;
    GLint yPast;
    dot *next;
};

dot *head = new dot;

void print_bitmap_string(void* font, char* s){
   if (s && strlen(s)) {
      while (*s) {
         glutBitmapCharacter(font, *s);
         s++;
      }
   }
}

void addDot(){
    dot *nuevo = new dot;
    nuevo -> next = NULL;
    dot *temp = head;
    while(temp->next != NULL){
        temp = temp->next;
    }
    temp->next = nuevo;
    nuevo -> x = temp->x;
    nuevo -> y = temp->y;
    nuevo -> xPast = temp->x;
    nuevo -> yPast = temp->y;
}

GLint PRNG(){
    // our initial starting seed is 5323
    static GLint nSeed = 5323;

    // Take the current seed and generate a new value from it
    // Due to our use of large constants and overflow, it would be
    // very hard for someone to predict what the next number is
    // going to be from the previous one.
    nSeed = (8253729 * nSeed + 2396403);

    // Take the seed and return a value between 0 and 9
    return nSeed  % 9;

}

void randomApple(){
    GLint ran = PRNG();
    GLint sign2 = ran;
    GLint sign = (double)PRNG();
    if(sign <= 5 && ran!= 0)
        ran *=-1;
    if(sign2 <= 5 && sign!= 0)
        sign *=-1;
    appleX = ran;
    appleY = sign;
}

void printGameOver(){
    void* bitmap_font = GLUT_BITMAP_TIMES_ROMAN_24;//GLUT_BITMAP_HELVETICA_18;
    glColor3f(1.0, 0.0, 0.0);
    char* text = "GAME OVER!";
    glRasterPos2i(-4, 0);
    print_bitmap_string(bitmap_font, text);
}

void snakeCollision(){
    if(head->next != NULL && head->next->next != NULL){
        dot *temp = head->next->next;
        while(temp -> next != NULL){
            if(temp->x == head->next->x && temp->y == head->next->y){
                gameOver=1;
            }
            temp = temp -> next;
        }
    }
    if(head->next != NULL){
        if(head->next->x == appleX && head->next->y == appleY){
            for(int i = 0; i < snakeSize; i++)
                addDot();
            snakeSize*=2;
            score++;
            randomApple();
        }
    }
}


void myTimer(int i) {
    GLint delta = 1;
    if(paused == 0 && gameOver==0){
        if(head->next != NULL){
            head->next->yPast = head->next->y;
            head->next->xPast = head->next->x;
            switch(dir){
                case 0:
                    head->next->y+=delta;
                    if(head->next->y >=10)
                        head->next->y = -9;
                    break;
                case 1:
                    head->next->x+=delta;
                    if(head->next->x >=10)
                        head->next->x = -9;
                    break;
                case 2:
                    head->next->y-=delta;
                    if(head->next->y <=-10)
                        head->next->y = 9;
                    break;
                case 3:
                    head->next->x-=delta;
                    if(head->next->x <=-10)
                        head->next->x = 9;
                    break;
            }
            delta =0;
        }
        if(head->next != NULL && head->next->next != NULL){
            dot *temp = head->next;
            while(temp -> next != NULL){
                temp->next->xPast = temp->next->x;
                temp->next->yPast = temp->next->y;
                temp->next->x = temp->xPast;
                temp->next->y = temp->yPast;
                temp = temp -> next;
            }
        }
        snakeCollision();
        canTurn=1;
    }
	glutPostRedisplay();
    glutTimerFunc(500,myTimer,1);

}

void print(){
    void* bitmap_font = GLUT_BITMAP_HELVETICA_18;
    glColor3f(1.0, 1.0, 1.0);
    char* text = "q - Exit";
    glRasterPos2f(6, 14);
    print_bitmap_string(bitmap_font, text);
    text = "p - Pause";
    glRasterPos2f(6.0, 13.2);
    print_bitmap_string(bitmap_font, text);
    text = "i - Restart";
    glRasterPos2f(6.0, 12.4);
    print_bitmap_string(bitmap_font, text);
    glColor3f(.50, 0.0, 1.0);

    stringstream myString;
    myString << "Score: " << score;

    string toString=myString.str();
    int TempNumOne=toString.length();
    char scoreText[100];
    for (int a=0;a<=TempNumOne;a++)
    {
        scoreText[a]=toString[a];
    }
    glRasterPos2f(-5, 12.4);
    print_bitmap_string(bitmap_font, scoreText);
}

void dibujarFiguras(){

    // Draw skane
    glColor3f(0.0, 0.50, 0.0);
    glPointSize(20);
    glBegin(GL_POINTS);
        dot *temp = head;
        while(temp -> next != NULL){
            glVertex2i(temp->next->x,temp->next->y);
            temp = temp -> next;
        }
    glEnd();

    // Draw Apple
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(20);
    glBegin(GL_POINTS);
        glVertex2f(appleX,appleY);
    glEnd();

    // Draw square
    glColor3f(1.0, 1.0, 1.0);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glBegin(GL_POLYGON);
        glVertex2f(-10.5,-10.5);
        glVertex2f(-10.5,9.5);
        glVertex2f(9.5,9.5);
        glVertex2f(9.5,-10.5);
    glEnd();
    glFlush();
}

void myDisplay(){
     glClear(GL_COLOR_BUFFER_BIT);
     dibujarFiguras();
     print();
     if(gameOver == 1)
        printGameOver();
     glutSwapBuffers();
}

void initValores(){
    glClearColor(0,0,0,1);
    glColor3f(1,1,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-11, 10, -11, 15);
    dot *temp = new dot;
    temp -> x = -10;
    temp -> y = 1;
    temp -> xPast = -10;
    temp -> yPast = 0;
    temp -> next = NULL;
    head -> next = temp;
    paused = 0;
    gameOver = 0;
    dir = 1 ;
    score = 0;
    randomApple();
    snakeSize = 1;
}

void keys(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
        case 'p':
            if(paused == 0)
                paused = 1;
            else
                paused = 0;
            break;

        case 'i':
            initValores();
            break;
    }
}

void specialKeys(int c, int x, int y){
    if(paused == 0 && canTurn==1){
        switch (c)
        {
            case GLUT_KEY_UP:
                if(dir != 2)
                    dir = 0;
                break;
            case GLUT_KEY_DOWN:
                if(dir != 0)
                    dir = 2;
                break;
            case GLUT_KEY_LEFT:
                if(dir != 1)
                    dir = 3;
                break;
            case GLUT_KEY_RIGHT:
                if(dir != 3)
                    dir = 1;
                break;
        }
        canTurn = 0;
    }
}

int main(){
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(0,0);
    glutCreateWindow("T4 Snake - Diego Garcia - 1162205");

        // registra función de deplegado
        glutDisplayFunc(myDisplay);
        // registra función timer
        glutTimerFunc(33,myTimer,1);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keys);
    initValores();
    glutMainLoop();
}
