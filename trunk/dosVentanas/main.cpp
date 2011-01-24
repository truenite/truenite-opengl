#include <windows.h>
#include <math.h>
#include <GL/glut.h>


int singleB, doubleB;
float thetaSB = 0.0;

void displays(void)
{
     glClear(GL_COLOR_BUFFER_BIT);

     GLfloat angulo = 0.017453*thetaSB;
     GLfloat sinAngulo = sin(angulo);
     GLfloat cosAngulo = cos(angulo);

	 glBegin(GL_POLYGON);
	 	glVertex2f( cosAngulo, sinAngulo );
	 	glVertex2f( -sinAngulo, cosAngulo );
	 	glVertex2f( -cosAngulo, -sinAngulo );
	 	glVertex2f( sinAngulo, -cosAngulo );
	  glEnd();

     glFlush();
}

void displayd(void)
{
     glClear(GL_COLOR_BUFFER_BIT);

     GLfloat angulo = 0.017453*thetaSB;
     GLfloat sinAngulo = sin(angulo);
     GLfloat cosAngulo = cos(angulo);

	 glBegin(GL_POLYGON);
	 	glVertex2f( cosAngulo, sinAngulo );
	 	glVertex2f( -sinAngulo, cosAngulo );
	 	glVertex2f( -cosAngulo, -sinAngulo );
	 	glVertex2f( sinAngulo, -cosAngulo );
	  glEnd();

     glutSwapBuffers();
}

void spinDisplay(){
     thetaSB+=2.0;
     if(thetaSB>360.0)
        thetaSB -= 360.0;

     glutSetWindow(singleB);
     glutPostWindowRedisplay(singleB);

     glutSetWindow(doubleB);
     glutPostWindowRedisplay(doubleB);
}

void mouse(int btn, int state, int x, int y){
     if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
        glutIdleFunc(spinDisplay);
     if(btn==GLUT_MIDDLE_BUTTON && state==GLUT_DOWN)
        glutIdleFunc(NULL);
}

void myReshapeSD(int w, int h){
     glViewport(0,0,w,h);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluOrtho2D(-2.0,2.0,-2.0,2.0);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
}

void mykey(unsigned char key, int x, int y){
     if(key=='Q' || key=='q')
        exit(0);
}

void quit_menu(int id){
     if(id==1)
          exit(0);
}

void myInit()
{
   glClearColor (1.0, 1.0, 1.0, 1.0);
   glColor3f(1.0, 0.0, 0.0);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluOrtho2D(-2.0,2.0,-2.0,2.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	singleB=glutCreateWindow("Single Buffered");
	myInit();
    glutDisplayFunc(displays);
    glutReshapeFunc(myReshapeSD);
    glutIdleFunc(spinDisplay);
    glutMouseFunc(mouse);
    glutKeyboardFunc(mykey);

   	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   	glutInitWindowPosition(310,0);
	doubleB=glutCreateWindow("Double Buffered");
	myInit();
    glutDisplayFunc(displayd);
    glutReshapeFunc(myReshapeSD);
    glutIdleFunc(spinDisplay);
    glutMouseFunc(mouse);
    glutCreateMenu(quit_menu);
    glutAddMenuEntry("quit",1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
}
