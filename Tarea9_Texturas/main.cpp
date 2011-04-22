/***************************************************
Materia: Gráficas Computacionales
Tarea: 9 Texturas
Fecha: 3 de abril de 2011
Autor 1: 1162205 Diego Alfonso Garcia Mendiburu
***************************************************/

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>


#define MINTRIANGULOS 10
#define MAXTRIANGULOS 100
#define IMAGESIZE 512



float MAXALTURA  = .2;

char textura[] ="Madera.ppm";
char altura[] = "espiral.ppm";


float rotationX=0.0;
float rotationY=0.0;
float prevX=0.0;
float prevY=0.0;
bool mouseDown=false;
float viewer[]= {0.0, 0.0, 1.0};


GLuint texName=0;
unsigned char woodtexture[IMAGESIZE][IMAGESIZE][3];
unsigned char woodHeight[IMAGESIZE][IMAGESIZE][3];
int numFilas = 100;
int heightSize;




void calcAltura(){
   int w, h, d;
   char *c;
   FILE *fp;
   int i, j, k;
   fp = fopen(altura,"rb");
   fscanf(fp,"%s ", &c);
   fscanf(fp,"%d %d %d ", &w, &h, &d) ;
   //heightSize = w;
   for (i = 0 ; i < w ; i++)
     for (j = 0 ; j < h ; j++)
       for (k = 0 ; k < 3 ; k++)
	     fscanf(fp,"%c",&(woodHeight[i][j][k])) ;
   fclose(fp);
}

float getAltura(int v){
    if(v == 255)
        return (GLfloat) MAXALTURA;
    else
        return (GLfloat)(v * MAXALTURA) / 255.0;
}

void initTexture(){

   /* First, read this simple ppm file in */
   int w, h, d;
   char *c;
   FILE *fp;
   int i, j, k;
   fp = fopen(textura,"rb");
   fscanf(fp,"%s ", &c);
   fscanf(fp,"%d %d %d ", &w, &h, &d) ;
   for (i = 0 ; i < w ; i++)
     for (j = 0 ; j < h ; j++)
       for (k = 0 ; k < 3 ; k++)
	     fscanf(fp,"%c",&(woodtexture[i][j][k])) ;
   fclose(fp) ;

   /* Now, set up all the stuff for texturing, per page 368 */
   glGenTextures(1, &texName) ;
   //glBindTexture(GL_TEXTURE_2D, texName) ;
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) ;
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) ;
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) ;
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGB, IMAGESIZE, IMAGESIZE, 0, GL_RGB,
	   GL_UNSIGNED_BYTE, woodtexture);


}

void specialKey(int c, int x, int y){


        if(c == GLUT_KEY_UP) {
            if(MAXALTURA < 1)
                MAXALTURA += 0.02;
        }
        if(c == GLUT_KEY_DOWN) {
            if(MAXALTURA > 0)
                MAXALTURA -= 0.02;
        }

   glutPostRedisplay();
}



void key(unsigned char key, int x, int y)
{
   if(key == '+' && numFilas < MAXTRIANGULOS) {

       numFilas += 1;
       //numFilas *= 2;
       printf("Numero de filas %d \n", numFilas);
   }
   if(key == '-' && numFilas > MINTRIANGULOS) {

       numFilas -= 1;
       //numFilas /= 2;
       printf("Numero de filas %d \n", numFilas);
   }
   if(key == 'x') viewer[0]-= 0.1;
   if(key == 'X') viewer[0]+= 0.1;
   if(key == 'y') viewer[1]-= 0.1;
   if(key == 'Y') viewer[1]+= 0.1;
   if(key == 'z') viewer[2]-= 0.1;
   if(key == 'Z') viewer[2]+= 0.1;
   glutPostRedisplay();
}


void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor3f(1,1,1);

    gluLookAt(viewer[0],viewer[1],viewer[2],0,0,0,0,1,0);


    glRotatef(rotationX,1,0,0);
    glRotatef(rotationY,0,1,0);


    //glTranslatef(1.0,1.0,0);
    //glRotatef(90,0,0,1);
    //glTranslatef(.5,.5,0);


 	float posActualX, posActualY, posActualZ;
 	posActualX = posActualY = posActualZ = 0.0;

 	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 	for(int i = 0; i < numFilas; i++){
 	    for(int j = 0; j < numFilas; j++){
 	        //glBegin(GL_POLYGON);
            glBegin(GL_TRIANGLES);

                //Vertice 1
                glTexCoord2f(posActualX, posActualY);
                int im = IMAGESIZE / numFilas;
                glVertex3f(posActualX, posActualY, getAltura(woodHeight[im * i][im * j][0]));

                //printf("[%d,%d,%d]\n", i, j, im * i);

                posActualX += 1/(GLfloat)numFilas;

                //Vertice 2
                glTexCoord2f(posActualX, posActualY);
                glVertex3f(posActualX, posActualY, getAltura(woodHeight[im * i][im * (j+1)][0]));

                posActualY += 1/(GLfloat)numFilas;


                //Vertice 3
                glTexCoord2f(posActualX, posActualY);
                glVertex3f(posActualX, posActualY, getAltura(woodHeight[im * (i+1)][im * (j+1)][0]));


                //Vertice 4
                glTexCoord2f(posActualX, posActualY);
                glVertex3f(posActualX, posActualY, getAltura(woodHeight[im * (i+1)][im * (j+1)][0]));

                posActualX -= 1/(GLfloat)numFilas;

                //Vertice 5
                glTexCoord2f(posActualX, posActualY);
                glVertex3f(posActualX, posActualY, getAltura(woodHeight[im * (i+1)][im * j][0]));

                posActualY -= 1/(GLfloat)numFilas;

                //Vertice 6
                glTexCoord2f(posActualX, posActualY);
                glVertex3f(posActualX, posActualY, getAltura(woodHeight[im * i][im * j][0]));
                posActualX += 1/(GLfloat)numFilas;

            glEnd();
 	    }
 	    posActualY += 1/(GLfloat)numFilas;
 	    posActualX = 0;
 	}
    //
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
   calcAltura();

}

void mouse(int button, int state, int x, int y){
    printf("pica\n");
    if(button == GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        mouseDown = true;
        prevX = x - rotationY;
        prevY = y - rotationX;
    }else{
        mouseDown = false;
    }
}

void mouseMotion(int x, int y){
    printf("moviendo\n");
    if(mouseDown){
        rotationX = y - prevY;
        rotationY = x - prevX;
        glutPostRedisplay();
    }
}


main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Usando una imagen como textura");
    glutKeyboardFunc(key);
    glutSpecialFunc(specialKey);
	glutDisplayFunc(myDisplay);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	initializeGL();
	glutMainLoop();
}


