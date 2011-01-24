#include <windows.h>
#include <GL/glut.h>

void dibujarFiguras(){

     glBegin(GL_TRIANGLES);
        glVertex2f(-.5,.8);
        glVertex2f(.5,.8);
        glVertex2f(0,.6);
     glEnd();

     glBegin(GL_TRIANGLE_STRIP);
        glVertex2f(-.5,.5);
        glVertex2f(.5,.5);
        glVertex2f(-.5,.2);
        glVertex2f(.5,.2);
        glVertex2f(.5,0);
     glEnd();

     glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0,0);
        glVertex2f(.5,-.5);
        glVertex2f(0,-.8);
        glVertex2f(-.5,-.5);
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
      glutCreateWindow("simple4");
      glutDisplayFunc(myDisplay);

      initValores();

      glutMainLoop();
}
