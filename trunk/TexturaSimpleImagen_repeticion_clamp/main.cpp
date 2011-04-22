#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

GLuint texName=0;
unsigned char woodtexture[512][512][3];

void initTexture(){

   /* Leer el archivo */
   int w, h, d;
   FILE *fp;
   int i, j, k;
   fp = fopen("madera.ppm","rb");
   fscanf(fp,"%s ");
   fscanf(fp,"%d %d %d ", &w, &h, &d) ;
   for (i = 0 ; i < w ; i++)
     for (j = 0 ; j < h ; j++)
       for (k = 0 ; k < 3 ; k++)
	     fscanf(fp,"%c",&(woodtexture[i][j][k])) ;
   fclose(fp) ;

   /* Especificar caracteristicas */
   glGenTextures(1, &texName) ;

    //descomentar para ver como funciona el GL_CLAMP

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) ;
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP) ;
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP) ;

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) ;
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) ;

   glTexImage2D(GL_TEXTURE_2D,0,GL_RGB, 512, 512, 0, GL_RGB,
	   GL_UNSIGNED_BYTE, woodtexture);
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor3f(1,1,1);

 	glBegin(GL_POLYGON);

		glTexCoord2f(0, 0);
		glVertex3f(0, 0, 0);

		glTexCoord2f(2, 0);
		glVertex3f(1, 0, 0);

		glTexCoord2f(2, 2);
		glVertex3f(1, 1, 0);

		glTexCoord2f(0, 2);
		glVertex3f(0, 1, 0);

	glEnd();

	glutSwapBuffers();
}

void initializeGL(void)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0,0,0,1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(-0.5, 1.5, -0.5, 1.5, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);

    glEnable(GL_TEXTURE_2D) ;
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE) ;
    glBindTexture(GL_TEXTURE_2D, texName) ;

   initTexture();
}

main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Pruebas de GL_REPEAT y GL_CLAMP");
	glutDisplayFunc(myDisplay);
	initializeGL();
	glutMainLoop();
}

