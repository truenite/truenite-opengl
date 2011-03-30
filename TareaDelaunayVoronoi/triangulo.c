#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <GL/glut.h>

#include "triangulo.h"

Triangulo *crearTriangulo(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat circunX, GLfloat circunY){
     Triangulo *nuevo=(Triangulo *)malloc(sizeof(Triangulo));
     if(!nuevo){
          printf("Error al alojar memoria");
          exit(1);
     }
     memset(nuevo,0,sizeof(nuevo));
     nuevo->x1=x1;
     nuevo->y1=y1;
     nuevo->x2=x2;
     nuevo->y2=y2;
     nuevo->x3=x3;
     nuevo->y3=y3;
     nuevo->circunX=circunX;
     nuevo->circunY=circunY;
     nuevo->next=NULL;
     return nuevo;
}

GLint esVertice(GLfloat x, GLfloat y, Triangulo *triangulo){
    if((x == triangulo->x1 && y == triangulo->y1) || (x == triangulo->x2 && y == triangulo->y2) || (x == triangulo->x3 && y == triangulo->y3))
        return 1;
    return 0;
}

GLint esIgualQue(Triangulo *t1 , Triangulo *t2){
    if(esVertice(t2->x1,t2->y1,t1) == 1 && esVertice(t2->x2,t2->y2,t1) == 1 && esVertice(t2->x3,t2->y3,t1) == 1)
        return 1;
    return 0;
}

GLint esVecino(Triangulo *t1 , Triangulo *t2){
    GLint cont = 0;
    cont += esVertice(t1->x1,t1->y1,t2);
    if(esVertice(t1->x2,t1->y2,t2) == 1)
        cont++;
    if(esVertice(t1->x3,t1->y3,t2))
        cont++;
    if(cont != 2)
        return 0;
    return 1;
}
GLint esVecinoConArista(Triangulo *t1 , Triangulo *t2){
    if(esVertice(t1->x1,t1->y1,t2) == 1 && esVertice(t1->x2,t1->y2,t2) == 1)
        return 0;
    if(esVertice(t1->x2,t1->y2,t2) == 1 && esVertice(t1->x3,t1->y3,t2) == 1)
        return 1;
    if(esVertice(t1->x1,t1->y1,t2) == 1 && esVertice(t1->x3,t1->y3,t2) == 1)
        return 2;
    return -1;
}

void drawTriangulo(GLfloat x1,GLfloat y1,GLfloat x2,GLfloat y2,GLfloat x3,GLfloat y3){
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
        glVertex2f(x1,y1);
        glVertex2f(x2,y2);
        glVertex2f(x2,y2);
        glVertex2f(x3,y3);
        glVertex2f(x3,y3);
        glVertex2f(x1,y1);
    glEnd();
}
