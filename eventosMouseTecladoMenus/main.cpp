#include <windows.h>
#include <GL/glut.h>

int figura = 0;
GLsizei anchoI, altoI;

void myReshapeI(GLsizei w, GLsizei h){
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();

     if(w<=h)
             gluOrtho2D(-2.0,2.0,-2.0*(GLfloat)h/(GLfloat)w,2.0*(GLfloat)h/(GLfloat)w);
     else
             gluOrtho2D(-2.0*(GLfloat)w/(GLfloat)h,2.0*(GLfloat)w/(GLfloat)h,-2.0,2.0);

     glMatrixMode(GL_MODELVIEW);

     glViewport(0,0,w,h);
     anchoI=w;
     altoI=h;

}

void displayI(void)
{
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

	switch(figura){
		case 1:
			glutSolidSphere(0.5, 10, 10);
			break;
		case 2:
			glutSolidCube(0.5);
			break;
		case 3:
			glutSolidTorus(0.2, 0.5, 10, 10);
			break;
		case 4:
			glutSolidTeapot(0.5);
			break;
		case 11:
			glutWireSphere(0.5, 10, 10);
			break;
		case 12:
			glutWireCube(0.5);
			break;
		case 13:
			glutWireTorus(0.2, 0.5, 10, 10);
			break;
		case 14:
			glutWireTeapot(0.5);
			break;
	}

	glutSwapBuffers();
}

void procesaMenuI(int val){
	if(figura < 20){
		figura = val;
		glutPostRedisplay();
	} else{
		exit(0);
	}
}

int addMenuesI(){
	int mainMenu, subMenu1, subMenu2;
	mainMenu = glutCreateMenu(procesaMenuI);
	subMenu1 = glutCreateMenu(procesaMenuI);
	subMenu2 = glutCreateMenu(procesaMenuI);
	glutSetMenu(mainMenu);
	glutAddSubMenu("Figura Sólida", subMenu1);
	glutAddSubMenu("Figura en Wireframe", subMenu2);
	glutAddMenuEntry("Salir", 20);
	glutSetMenu(subMenu1);
	glutAddMenuEntry("Esfera",   1);
	glutAddMenuEntry("Cubo",     2);
	glutAddMenuEntry("Toro",     3);
	glutAddMenuEntry("Tetera",   4);
	glutSetMenu(subMenu2);
	glutAddMenuEntry("Esfera",   11);
	glutAddMenuEntry("Cubo",     12);
	glutAddMenuEntry("Toro",     13);
	glutAddMenuEntry("Tetera",   14);
	glutSetMenu(mainMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void keyI(unsigned char c, int x, int y){
	if(c == 'x') exit(0);
	if(c == '\t') {
		figura = (figura/10)*10+(((figura-1)%10+1)%4)+1;
		glutPostRedisplay();
	}
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


int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow("Ejercicio 1");
  glutDisplayFunc(displayI);
  glutReshapeFunc(myReshapeI);
  glutKeyboardFunc(keyI);
  glutMouseFunc(mouseI);
  glutSpecialFunc(specialI);
  addMenuesI();
  glutMainLoop();
  return 0;
}
