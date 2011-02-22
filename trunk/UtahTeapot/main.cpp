#include <windows.h>
#include <GL/glut.h>

float rotationX=0.0;
float rotationY=0.0;
float prevX=0.0;
float prevY=0.0;
bool mouseDown=false;
float viewer[]= {0.0, 0.0, 3.0};
float angle=0.0;
int displayMode=1;


void teapot(void){
    glPushMatrix();
        glRotatef(angle,0,1,0);
        if(displayMode==1)
            glutWireTeapot(0.5);
        else
            glutSolidTeapot(0.5);
    glPopMatrix();
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(viewer[0],viewer[1],viewer[2],0,0,0,0,1,0);

    glRotatef(rotationX,1,0,0);
    glRotatef(rotationY,0,1,0);

    teapot();

    glutSwapBuffers();
}

void reshape(int w, int h){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble)w/(GLdouble)h, 1.0, 100.0);
    glViewport(0,0,w,h);
}

void idle(){
    angle += 1;
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        mouseDown = true;
        prevX = x - rotationY;
        prevY = y - rotationX;
    }else{
        mouseDown = false;
    }
}

void mouseMotion(int x, int y){
    if(mouseDown){
        rotationX = y - prevY;
        rotationY = x - prevX;
        glutPostRedisplay();
    }
}

void key(unsigned char key, int x, int y)
{
   if(key == 'x') viewer[0]-= 0.1;
   if(key == 'X') viewer[0]+= 0.1;
   if(key == 'y') viewer[1]-= 0.1;
   if(key == 'Y') viewer[1]+= 0.1;
   if(key == 'z') viewer[2]-= 0.1;
   if(key == 'Z') viewer[2]+= 0.1;
   glutPostRedisplay();
}

void menu(int val){
	displayMode=val;
	glutPostRedisplay();
}

int addMenu(){
	int mainMenu, subMenu1;
	mainMenu = glutCreateMenu(menu);
	subMenu1 = glutCreateMenu(menu);
	glutSetMenu(mainMenu);
	glutAddSubMenu("Display mode", subMenu1);
	glutSetMenu(subMenu1);
	glutAddMenuEntry("Wireframe",   1);
	glutAddMenuEntry("Solid",     2);
	glutSetMenu(mainMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("Utah Teapot");
    glutInitWindowSize(500,500);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle) ;
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(key);
    addMenu();
    glutMainLoop();
}
