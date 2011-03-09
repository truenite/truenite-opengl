#include <windows.h>
#include <GL/glut.h>
typedef struct vertice{
    GLfloat x;
    GLfloat y;
    struct vertice *next;
}Vertice;

Vertice *crearVertice(GLfloat x, GLfloat y);
