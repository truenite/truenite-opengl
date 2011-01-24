#include <windows.h>
#include <GL/glut.h>

GLsizei ancho, alto;

float r = 0.0;
float g = 0.0;
float b = 0.0;

void init()
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

void menuColores(int entryID)
{
    if (entryID == 1){
    	r=1.0;
    	g=0.0;
    	b=0.0;
     }

    if (entryID == 2){
    	r=0.0;
    	g=1.0;
    	b=0.0;
     }

    if (entryID == 3){
    	r=0.0;
    	g=0.0;
    	b=1.0;
     }


    glutPostRedisplay();
}

void crearMenus(){
    glutCreateMenu(menuColores);
    glutAddMenuEntry("Rojo", 1 );
    glutAddMenuEntry("Verde",2 );
    glutAddMenuEntry("Azul", 3 );
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void myDisplay(){
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
	glutCreateWindow("Menu Boton Derecho");
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	init();
	crearMenus();
	glutMainLoop();
}
