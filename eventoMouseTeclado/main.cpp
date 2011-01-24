#include <windows.h>
#include <GL/glut.h>

void myMouseButton(int button, int state, int x, int y)
{
	if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
		exit(0);
}

void myKey(unsigned char key, int x, int y)
{
if(key == 'Q' || key == 'q')
	exit(0);
}





void myDisplay(){
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON);
		glVertex2f(-0.5, -0.5);
		glVertex2f(-0.5, 0.5);
		glVertex2f(0.5, 0.5);
		glVertex2f(0.5, -0.5);
	glEnd();
	glFlush();
}
int main(){
	glutCreateWindow("simple1");

	glutDisplayFunc(myDisplay);

	glutMouseFunc(myMouseButton);
	glutKeyboardFunc(myKey);

	glutMainLoop();
}
