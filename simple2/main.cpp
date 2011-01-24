#include <windows.h>
#include <GL/glut.h>

void miFunc(){

     glClear(GL_COLOR_BUFFER_BIT);
     glBegin(GL_POLYGON);
        glVertex2f(-.5,-.5);
        glVertex2f(-.5,.5);
        glVertex2f(.5,.5);
        glVertex2f(.5,-.5);
     glEnd();
     glFlush();
}

void initValores(){

     glClearColor(0,0,0,1);
     glColor3f(1,1,1);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluOrtho2D(-1,1,-1,1);
}

main( ){

      glutInitDisplayMode(GLUT_RGB);
      glutInitWindowSize(300,300);
      glutInitWindowPosition(0,0);

      glutCreateWindow("simple2");

      glutDisplayFunc(miFunc);

      initValores();

      glutMainLoop();
}
