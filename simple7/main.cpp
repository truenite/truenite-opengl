#include <windows.h>
#include <GL/glut.h>

void dibujarFiguras(){

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(10);
    glBegin(GL_LINES);
        glColor4f(0, 1.0, 0, 1.0);
        glVertex2f(-1.0, 0.0);
        glColor4f(0, 1.0, 0, 0.0);
        glVertex2f(1.0, 0.0);
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
      glutCreateWindow("simple7");
      glutDisplayFunc(myDisplay);

      initValores();

      glutMainLoop();
}
