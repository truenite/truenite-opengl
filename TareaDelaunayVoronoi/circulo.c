#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <GL/glut.h>
#include <math.h>
#define M_PI 3.14159265358979323846f

#include "circulo.h"

Circulo *crearCirculo(GLfloat x, GLfloat y, GLfloat radio){
     Circulo *nuevo=(Circulo *)malloc(sizeof(Circulo));
     if(!nuevo){
          printf("Error al alojar memoria");
          exit(1);
     }
     memset(nuevo,0,sizeof(nuevo));
     nuevo->x=x;
     nuevo->y=y;
     nuevo->radio=radio;
     return nuevo;
}

void glCircle3f(GLfloat x, GLfloat y, GLfloat radius) {
    float angle;
    glPushMatrix();
    glTranslatef(x,y,0);
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    glLineWidth(5.0f);
    glBegin(GL_LINE_LOOP);
    int i;
    for(i = 0; i < 100; i++) {
        angle = i*2*M_PI/100;
        glVertex2f(x + (cos(angle) * radius), y + (sin(angle) * radius));
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

GLfloat calcularRadio(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2){
    return sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}

int estaDentroC(GLfloat x, GLfloat y, Circulo *circulo){
    if(calcularRadio(x,y,circulo->x,circulo->y)>circulo->radio)
        return 0;
    return 1;
}

int estaDentroP(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat radio){
    if(calcularRadio(x1,y1,x2,y2)>radio)
        return 0;
    return 1;
}

