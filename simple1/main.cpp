#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

float r = 1.0;
float g = 1.0;
float b = 1.0;

void myDisplay(){
    printf("Display");
	glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(r,g,b);
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
	glutMainLoop();
}
