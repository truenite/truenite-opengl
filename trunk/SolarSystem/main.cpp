#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

float rotationX=0.0;
float rotationY=0.0;
float prevX=0.0;
float prevY=0.0;
bool mouseDown=false;
float viewer[]= {0.0, 0.0, 8.0};
float angle[]={0.0,0.0,0.0,0.0,0.0,0.0};
float speed[]={0.01,0.03,0.04,0.05,0.06,0.07};
int displayMode=1;


void drawSolarSystem(void){
    //sol
    glPushMatrix();
        glColor3f(1,1,0);
        glRotatef(angle[0],0,0,1);
        if(displayMode==1)
            glutWireSphere(0.5,10,10);
        else
            glutSolidSphere(0.5,10,10);
    glPopMatrix();

    //tierra
    glPushMatrix();
        glColor3f(0,0,1);
        glRotatef(angle[1],0,0,1);
        glTranslatef(1.4,0,0);
        glRotatef(angle[0],0,0,1);
        if(displayMode==1)
            glutWireSphere(0.3,10,10);
        else
            glutSolidSphere(0.3,10,10);
        //luna1 dependiente de la tierra (adentro del popmatrix)
        glPushMatrix();
        glColor3f(0.7,0.7,0.7);
        glRotatef(angle[2],0,0,1);
        glTranslatef(.5,0,0);
        glRotatef(angle[0],0,0,1);
        if(displayMode==1)
            glutWireSphere(0.05,10,10);
        else
            glutSolidSphere(0.05,10,10);


        glPopMatrix();

        //luna2 dependiente de la tierra (adentro del popmatrix)
        glPushMatrix();
        glColor3f(0.5,0.5,0.5);
        glRotatef(angle[3],0,0,1);
        glTranslatef(.6,0,0);
        glRotatef(angle[0],0,0,1);
        if(displayMode==1)
            glutWireSphere(0.03,10,10);
        else
            glutSolidSphere(0.03,10,10);


        glPopMatrix();
    glPopMatrix();

    //mercurio
    glPushMatrix();
        glColor3f(1,0,1);
        glRotatef(angle[4],0,0,1);
        glTranslatef(.7,0,0);
        glRotatef(angle[0],0,0,1);
        if(displayMode==1)
            glutWireSphere(0.1,10,10);
        else
            glutSolidSphere(0.1,10,10);
    glPopMatrix();

    //marte
    glPushMatrix();
        glColor3f(1,0,0);
        glRotatef(angle[5],0,0,1);
        glTranslatef(2.3,0,0);
        glRotatef(angle[0],0,0,1);
        if(displayMode==1)
            glutWireSphere(0.25,10,10);
        else
            glutSolidSphere(0.25,10,10);
    glPopMatrix();
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(viewer[0],viewer[1],viewer[2],0,0,0,0,1,0);

    glRotatef(rotationX,1,0,0);
    glRotatef(rotationY,0,1,0);

    drawSolarSystem();

    glutSwapBuffers();
}

void reshape(int w, int h){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble)w/(GLdouble)h, 1.0, 100.0);
    glViewport(0,0,w,h);
}

void idle(){
    for(int i = 0; i < 6;i++){
        angle[i] += speed[i];
    }
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

void init(void){
    glEnable(GL_DEPTH_TEST);
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Solar System");
    glutInitWindowSize(900,900);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle) ;
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(key);
    addMenu();
    init();
    glutMainLoop();
}
