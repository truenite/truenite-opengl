#include <windows.h>
#include <GL/glut.h>

void dibujarFiguras(){

    glColor3f(1,0,0);
    glPointSize(80);
    glBegin(GL_POINTS);
        glVertex2f(0, .5);
    glEnd();

    glLineWidth(80);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(5, 50);

    glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0); // rojo
        glVertex2f( -.5, -.5);
        glColor3f(0.0, 0.0, 1.0); // azul
        glVertex2f( .5, -.5);
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
      glutCreateWindow("simple6");
      glutDisplayFunc(myDisplay);

      initValores();

      glutMainLoop();
}
