/*

Diego Alfonso García Mendiburu
1162205

Tarea3, introducción a OpenGL

24/01/2011


Nota, el zoom funciona pero medio raro, si se le pica mucho a una flecha al querer cambiar de flecha como que el otro zoom tiene un delay.


*/#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;




//coordenadas del triángulo que se dividirá
GLfloat verticesTriangulo[3][2]={{-0.5,-0.5},{0.0,0.5},{0.5,-0.5}};
static GLint ite = 1;
static GLint shade = 0;
static GLfloat r1,r2,r3 = 0;
static GLfloat zoom = 1;
GLsizei ancho,alto;

void dibujarTriangulo(GLfloat *a, GLfloat *b, GLfloat *c){
    if(shade == 0)
        glShadeModel(GL_SMOOTH);
    else
        glShadeModel(GL_FLAT);
    glBegin(GL_TRIANGLES);
        //en este caso los vértices están dados en el sentido de las manecillas del reloj
        r1 = (float)rand()/(float)RAND_MAX;
        r2 = (float)rand()/(float)RAND_MAX;
        r3 = (float)rand()/(float)RAND_MAX;
        glColor3f(r1,r2,r3);
        glVertex2fv(a);
        r1 = (float)rand()/(float)RAND_MAX;
        r2 = (float)rand()/(float)RAND_MAX;
        r3 = (float)rand()/(float)RAND_MAX;
        glColor3f(r1,r2,r3);
        glVertex2fv(b);
        r1 = (float)rand()/(float)RAND_MAX;
        r2 = (float)rand()/(float)RAND_MAX;
        r3 = (float)rand()/(float)RAND_MAX;
        glColor3f(r1,r2,r3);
        glVertex2fv(c);
    glEnd();
}

void dividirTriangulo(GLfloat *a, GLfloat *b, GLfloat *c, int iteraciones){
     GLfloat v[3][2];
     int j;
     if(iteraciones>0){

        //encontrar los puntos medios de cada arista del triángulo
        for(j=0; j<2; j++)
                 v[0][j]=(a[j]+b[j])/2;
        for(j=0; j<2; j++)
                 v[1][j]=(a[j]+c[j])/2;
        for(j=0; j<2; j++)
                 v[2][j]=(b[j]+c[j])/2;

        //por cada triángulo que entra, se crean 4 triángulos más pequeños y recursivamente estos se dividen a su vez

        glColor4f(0, 0, 1, 1);
        dividirTriangulo( a  , v[0], v[1], iteraciones-1);
        glColor4f(1, 0, 0, 1);
        dividirTriangulo(v[0],  b  , v[2], iteraciones-1);
        glColor4f(0.7, 0.7, 0.7, 1);
        dividirTriangulo(v[1], v[2],  c  , iteraciones-1);
        //dividirTriangulo(v[0], v[1], v[2], iteraciones-1);

     }else{
        //dibujar el triángulo de la iteración 0
        dibujarTriangulo(a,b,c);
     }
}

void desplegar(void)
{
     glClear(GL_COLOR_BUFFER_BIT);
     //al llamar la función dividirTriangulo se indica como cuarto parámetro el número de iteraciones de subdivisión que se quieren
     dividirTriangulo(verticesTriangulo[0], verticesTriangulo[1], verticesTriangulo[2],ite);
     glFlush();
}

void mouseButton(int button, int state, int x, int y)
{
	if((button==GLUT_RIGHT_BUTTON || button == GLUT_LEFT_BUTTON )&& state==GLUT_DOWN)
		glutPostRedisplay();
}

void teclado(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
        case '+':
            ite++;
            glutPostRedisplay();
            break;

        case '-':
            if(ite > 0) ite--;
            glutPostRedisplay();
            break;
    }
}

void reshape(GLsizei w, GLsizei h){
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();

     if(w<=h)
             gluOrtho2D(-2.0,2.0,-2.0*(GLfloat)h/(GLfloat)w,2.0*(GLfloat)h/(GLfloat)w);
     else
             gluOrtho2D(-2.0*(GLfloat)w/(GLfloat)h,2.0*(GLfloat)w/(GLfloat)h,-2.0,2.0);

     glMatrixMode(GL_MODELVIEW);

     glViewport(0,0,w,h);
     ancho=w;
     alto=h;

}

void specialKeys(int c, int x, int y){
    switch (c)
    {
        case GLUT_KEY_UP:
            gluOrtho2D(-zoom+.05,zoom-.05,-zoom+.05,zoom-.05);
            glutPostRedisplay();
            break;

        case GLUT_KEY_DOWN:
            gluOrtho2D(-zoom-.05,zoom+.05,-zoom-.05,zoom+.05);
            glutPostRedisplay();
            break;
    }
}
void menuBotonDerecho(int entryID)
{
    shade = entryID;

    glutPostRedisplay();
}

void crearMenus(){
    glutCreateMenu(menuBotonDerecho);
    glutAddMenuEntry("Smooth", 0);
    glutAddMenuEntry("Flat",1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}



void inicializar()
{
   glClearColor (1.0, 1.0, 1.0, 1.0);
   glColor3f(0.0, 0.0, 0.0);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}


int main(int argc, char** argv)
{
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Ejemplo Subdivisiones");
    inicializar();
    glutDisplayFunc(desplegar);
    glutKeyboardFunc(teclado);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouseButton);
    crearMenus();
	glutMainLoop();
}
