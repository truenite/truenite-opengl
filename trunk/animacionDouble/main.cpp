#include <windows.h>
#include <GL/glut.h>

float t=-1.0;
float delta=0.1;


void myTimer(int i) {
	t += delta;
	glutPostRedisplay();
    glutTimerFunc(1000,myTimer,1);

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

void specialI(int c, int x, int y){
	if(c == GLUT_KEY_RIGHT) {
		figura = (figura/10)*10+(((figura-1)%10+1)%4)+1;
		glutPostRedisplay();
	}
}

void mouseI(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
			figura = (figura/10)*10+(((figura-1)%10+1)%4)+1;
			glutPostRedisplay();
		}
}

main( ){
      glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
      glutInitWindowSize(500,500);
      glutInitWindowPosition(0,0);
      glutCreateWindow("T4 Viborita - Diego Garcia - 1162205");

        // registra función de deplegado
        glutDisplayFunc(myDisplay);
        // registra función timer
        glutTimerFunc(33,myTimer,1);

      initValores();

      glutMainLoop();
}
