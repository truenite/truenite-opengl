#include <windows.h>
#include <GL/glut.h>
typedef struct circulo{
    GLfloat x;
    GLfloat y;
    GLfloat radio;
}Circulo;

Circulo *crearCirculo(GLfloat x, GLfloat y, GLfloat radio);
void glCircle3f(GLfloat x, GLfloat y, GLfloat radius);
GLfloat calcularRadio(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
int estaDentroC(GLfloat x, GLfloat y, Circulo *circulo);
int estaDentroP(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat radio);
