#include <windows.h>

#include <GL/glut.h>

void dibujarFiguras(){

    glBegin(GL_POLYGON);
        glColor3f(1.0, 1.0, 1.0); // blanco
        glVertex2f(-0.5, -0.5);
        glColor3f(1.0, 0.0, 0.0); // rojo
        glVertex2f(-0.5, 0.5);
        glColor3f(0.0, 0.0, 1.0); // azul
        glVertex2f( 0.5, 0.5);
        glColor3f(1.0, 0.0, 1.0); // morado
        glVertex2f( 0.5, -0.5);
    glEnd();


    glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0); // rojo
        glVertex2f( -.8, -.8);
        glColor3f(0.0, 0.0, 1.0); // azul
        glVertex2f( .8, -.8);
    glEnd();

 }

void myDisplay(){
     glClear(GL_COLOR_BUFFER_BIT);

     dibujarFiguras();

     glFlush();
}

void initValores(){
     glClearColor(0,0,0,1);
     glColor3f(1,1,1);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
}


main( ){
      glutInitDisplayMode(GLUT_RGB);
      glutInitWindowSize(500,500);
      glutInitWindowPosition(0,0);
      glutCreateWindow("simple5");
      glutDisplayFunc(myDisplay);

      initValores();

      glutMainLoop();
}
