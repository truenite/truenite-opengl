#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <GL/glut.h>

#include "vertice.h"

Vertice *crearVertice(GLfloat x, GLfloat y){
    Vertice *nuevo=(Vertice *)malloc(sizeof(Vertice));
    if(!nuevo){
        printf("Error al alojar memoria");
        exit(1);
    }
    memset(nuevo,0,sizeof(nuevo));
    nuevo->x=x;
    nuevo->y=y;
    nuevo->next=NULL;
    return nuevo;
}
