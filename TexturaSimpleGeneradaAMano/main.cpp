#include <windows.h>
#include <stdlib.h>
#include <GL/glut.h>

#define textureWidth  64
#define textureHeight 64

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor3f(1,1,1);
 	glBegin(GL_POLYGON);

		glTexCoord2f(0, 0);
		glVertex3f(0, 0, 0);

		glTexCoord2f(1, 0);
		glVertex3f(1, 0, 0);

		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 0);

		glTexCoord2f(0, 1);
		glVertex3f(0, 1, 0);

	glEnd();

	glutSwapBuffers();
}

void createTexture()
{

	GLubyte myTexture[textureWidth][textureHeight][3];
	int u, v;

	glEnable(GL_TEXTURE_2D);

	for(u = 0; u < textureWidth; u++)
	{
		for(v = 0; v < textureHeight; v++)
		{
			myTexture[u][v][0] = (GLubyte)(255 * rand()/RAND_MAX);
			myTexture[u][v][1] = (GLubyte)(255 * rand()/RAND_MAX);
			myTexture[u][v][2] = (GLubyte)(255 * rand()/RAND_MAX);
		}
	}

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, textureWidth, textureHeight, 0,
				 GL_RGB, GL_UNSIGNED_BYTE, myTexture);

}

void initializeGL(void)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0,0,0,1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(-0.5, 1.5, -0.5, 1.5, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
}

main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Generando una textura a mano");
	glutDisplayFunc(myDisplay);
	initializeGL();
	createTexture();
	glutMainLoop();
}

