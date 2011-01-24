#include <windows.h>
#include <GL/glut.h>

float t=-1.0;
float delta=0.1;


void myTimer(int i) {
	t += delta;
	glutPostRedisplay();
    glutTimerFunc(33,myTimer,1);

}

void dibujarFiguras(){
    glPointSize(50);
    glBegin(GL_POINTS);
      glVertex2f(t,0);
    glEnd();
}

void myDisplay(){
     glClear(GL_COLOR_BUFFER_BIT);
     dibujarFiguras();
     glutSwapBuffers();
}

void initValores(){
     glClearColor(0,0,0,1);
     glColor3f(1,1,1);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}


main( ){
      glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
      glutInitWindowSize(500,500);
      glutInitWindowPosition(0,0);
      glutCreateWindow("animacion1");

        // registra función de deplegado
        glutDisplayFunc(myDisplay);
        // registra función timer
        glutTimerFunc(33,myTimer,1);

      initValores();

      glutMainLoop();
}
