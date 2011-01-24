#include <windows.h>
#include <GL/glut.h>

void dibujarFiguras(){

     glBegin(GL_POINTS);
        glVertex2f(0,.8);
     glEnd();

     glBegin(GL_LINES);
        glVertex2f(-.5,.5);
        glVertex2f(.5,.5);
     glEnd();

     glBegin(GL_LINE_STRIP);
        glVertex2f(-.5,.2);
        glVertex2f(0,0);
        glVertex2f(.5,.2);
     glEnd();

     glBegin(GL_LINE_LOOP);
        glVertex2f(-.5,-.2);
        glVertex2f(0,-.5);
        glVertex2f(.5,-.2);
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
      glutCreateWindow("simple3");
      glutDisplayFunc(myDisplay);

      initValores();

      glutMainLoop();
}
