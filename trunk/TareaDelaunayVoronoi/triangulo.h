#include <windows.h>
#include <GL/glut.h>
typedef struct triangulo{
    GLfloat x1;
    GLfloat y1;
    GLfloat x2;
    GLfloat y2;
    GLfloat x3;
    GLfloat y3;
    struct triangulo *next;
}Triangulo;

Triangulo *crearTriangulo(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3);
