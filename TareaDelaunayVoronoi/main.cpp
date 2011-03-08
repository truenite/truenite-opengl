/***************************************************
Materia: Gráficas Computacionales
Tarea: 7 Delaunay y Voronoi
Fecha: 12 de marzo del 2011
Autor 1: 1162205 Diego Alfonso García Mendiburu
***************************************************/

#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "triangulo.c"
#include "circulo.c"

float rotationX=0.0;
float rotationY=0.0;
float prevX=0.0;
float prevY=0.0;
bool mouseDown=false;
float viewer[]= {0.0, 0.0, 7.0};
int displayMode=1;
int agregados = 0;
float pos[] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
int ventanaX = 500;
int ventanaY = 500;
int puntoSeleccionado = -1;
int dibujarTriangulo = 1;
int dibujarCirculo = 1;
int dibujarVertices = 1;


void calcularCircuncentro(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3){

    GLfloat ABx = (x1 + x2)/2;
    GLfloat ABy = (y1 + y2)/2;
    GLfloat BCx = (x3 + x2)/2;
    GLfloat BCy = (y3 + y2)/2;
    GLfloat ABm = (y1 - y2)/(x1 - x2);
    GLfloat ABmPrime = 0;
    if(ABm != 0)
        ABmPrime = -1/ABm;
    else ABmPrime = 0;
    GLfloat BCm = (y3 - y2)/(x3 - x2);
    GLfloat BCmPrime = 0;
    if(BCm != 0)
        BCmPrime = -1/BCm;
    else BCmPrime = 0;

    GLfloat circunX = ((-BCx * BCmPrime)+BCy+(ABx*ABmPrime)-ABy)/(ABmPrime-BCmPrime);
    GLfloat circunY = (circunX*ABmPrime)-(ABx*ABmPrime)+(ABy);
    glColor3f(1.0f, 1.0f, 0.0f);
    /*glPushMatrix();
        glTranslatef(circunX,circunY,0);
        glutSolidSphere(0.03,20,20);
    glPopMatrix();*/
    GLfloat radio = calcularRadio(x1,y1,circunX,circunY);
    if(dibujarCirculo == 1)
        glCircle3f(circunX,circunY,radio);
}

void drawVertices(){
    if(dibujarVertices == 1){
        if(agregados > 0){
            if(puntoSeleccionado == 0)
                glColor3f(0.0f, 1.0f, 0.0f);
            else
                glColor3f(0.0f, 0.0f, 0.0f);
            glPushMatrix();
                glTranslatef(pos[0],pos[1],0);
                glutSolidSphere(0.03,20,20);
            glPopMatrix();
        }
        if(agregados > 1){
            if(puntoSeleccionado == 1)
                glColor3f(0.0f, 1.0f, 0.0f);
            else
                glColor3f(0.0f, 0.0f, 0.0f);
            glPushMatrix();
                glTranslatef(pos[2],pos[3],0);
                glutSolidSphere(0.03,20,20);
            glPopMatrix();
        }
        if(agregados > 2){
            if(puntoSeleccionado == 2)
                glColor3f(0.0f, 1.0f, 0.0f);
            else
                glColor3f(0.0f, 0.0f, 0.0f);
            glPushMatrix();
                glTranslatef(pos[4],pos[5],0);
                glutSolidSphere(0.03,20,20);
            glPopMatrix();

            glColor3f(0.0f, 0.0f, 0.0f);
        }
    }
}

void display(){
    glColor3f(0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    drawVertices();

    if(agregados > 2){
        if(dibujarTriangulo == 1){
            glBegin(GL_LINES);
                glVertex2f(pos[0],pos[1]);
                glVertex2f(pos[2],pos[3]);
                glVertex2f(pos[2],pos[3]);
                glVertex2f(pos[4],pos[5]);
                glVertex2f(pos[4],pos[5]);
                glVertex2f(pos[0],pos[1]);
            glEnd();
        }
        calcularCircuncentro(pos[0],pos[1],pos[2],pos[3],pos[4],pos[5]);
    }
    glFlush();
}

void initValores(){

     glClearColor(1,1,1,1);
     glColor3f(0,0,0);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluOrtho2D(-250,250,-250,250);
}

void mouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        mouseDown = true;
        if(agregados==0){
            pos[0] = (float)(ventanaX/2 - x)/ventanaX*-2;
            pos[1] = (float)(ventanaY/2 - y)/ventanaY*2;
            agregados++;
            //printf("agregados : %i  x= %f y= %f\n",agregados,pos[0],pos[1]);
        }
        else if(agregados==1){
            pos[2] = (float)(ventanaX/2 - x)/ventanaX*-2;
            pos[3] = (float)(ventanaY/2 - y)/ventanaY*2;
            agregados++;
        }
        else if(agregados==2){
            pos[4] = (float)(ventanaX/2 - x)/ventanaX*-2;
            pos[5] = (float)(ventanaY/2 - y)/ventanaY*2;
            agregados++;
        }
        GLfloat xMouse = (float)(ventanaX/2 - x)/ventanaX*-2;
        GLfloat yMouse = (float)(ventanaY/2 - y)/ventanaY*2;

        if(estaDentroP(xMouse,yMouse,pos[0],pos[1],.03)){
            puntoSeleccionado = 0;
        }
        else if(estaDentroP(xMouse,yMouse,pos[2],pos[3],.03)){
            puntoSeleccionado = 1;
        }
        else if(estaDentroP(xMouse,yMouse,pos[4],pos[5],.03)){
            puntoSeleccionado = 2;
        }else puntoSeleccionado = -1;
    }else{
        mouseDown = false;
        puntoSeleccionado = -1;
    }
    glutPostRedisplay();
}

void mouseMotion(int x, int y){
    //printf("ASa\n");
    if(mouseDown && puntoSeleccionado >= 0){
        pos[2*puntoSeleccionado] = (float)(ventanaX/2 - x)/ventanaX*-2;
        pos[(2*puntoSeleccionado)+1] = (float)(ventanaY/2 - y)/ventanaY*2;
        glutPostRedisplay();
    }
}


void reshape(int w, int h){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0,0,w,h);
}


void key(unsigned char key, int x, int y){
    printf("%c  dibujarCirculo = %i\n",key,dibujarCirculo);
    if(key == 'c') dibujarCirculo = (dibujarCirculo+1)%2;
    if(key == 'C') dibujarCirculo = (dibujarCirculo+1)%2;
    if(key == 't') dibujarTriangulo = (dibujarTriangulo+1)%2;
    if(key == 'T') dibujarTriangulo = (dibujarTriangulo+1)%2;
    if(key == 'p') dibujarVertices = (dibujarVertices+1)%2;
    if(key == 'P') dibujarVertices = (dibujarVertices+1)%2;
    if(key == 'i') agregados = 0;
    if(key == 'I') agregados = 0;
    glutPostRedisplay();
}

main(){
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(0,0);

    glutCreateWindow("Parte1");

    glutDisplayFunc(display);

    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);

    glutKeyboardFunc(key);
    initValores();

    glutMainLoop();
}
