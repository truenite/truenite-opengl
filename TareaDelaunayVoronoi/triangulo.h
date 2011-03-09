#include <windows.h>
#include <GL/glut.h>
typedef struct triangulo{
    GLfloat x1;
    GLfloat y1;
    GLfloat x2;
    GLfloat y2;
    GLfloat x3;
    GLfloat y3;
    GLfloat circunX;
    GLfloat circunY;
    struct triangulo *next;
}Triangulo;

Triangulo *crearTriangulo(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat circunX, GLfloat circunY);
GLint esVertice(GLfloat x, GLfloat y, Triangulo *triangulo);
GLint esIqualQue(Triangulo *t1 , Triangulo *t2);
GLint esVecino(Triangulo *t1 , Triangulo *t2);
