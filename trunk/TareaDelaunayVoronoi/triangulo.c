#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <GL/glut.h>

#include "triangulo.h"

Triangulo *crearTriangulo(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3){
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
     nuevo->next=NULL;
     return nuevo;
}
