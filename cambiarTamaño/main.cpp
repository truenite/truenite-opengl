#include <windows.h>
#include <GL/glut.h>

GLsizei ancho, alto;

void displayReshape(void)
{
     glClear(GL_COLOR_BUFFER_BIT);

	 glBegin(GL_POLYGON);
	 	glVertex2f(-0.5, -0.5);
	 	glVertex2f(-0.5, 0.5);
	 	glVertex2f(0.5, 0.5);
	 	glVertex2f(0.5, -0.5);
	  glEnd();

     glFlush();
}

void initReshape()
{
   glClearColor (1.0, 1.0, 1.0, 1.0);
   glColor3f(1.0, 0.0, 0.0);
}

void myReshape(GLsizei w, GLsizei h){
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

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400,400);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Ejemplo Reshape");
    glutDisplayFunc(displayReshape);
    glutReshapeFunc(myReshape);
    initReshape();
	glutMainLoop();
}
