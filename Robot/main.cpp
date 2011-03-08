/***************************************************
Materia: Gráficas Computacionales
Tarea: 5
Fecha: 17 de febrero del 2011
Autor 1: 1162205 Diego Alfonso García Mendiburu
***************************************************/

#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>

float rotationX=0.0;
float rotationY=0.0;
float prevX=0.0;
float prevY=0.0;
bool mouseDown=false;
float viewer[]= {0.0, 0.0, 7.0};
int displayMode=1;

GLfloat anguloTorsoY = 0.0;
GLfloat angulosX[10]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
GLfloat angulosZ[10]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
int segmento=0;


typedef struct treenode
{
    GLfloat m[16];
    void (*f)();
    struct treenode *sibling;
    struct treenode *child;

} treenode;

treenode torso, head, leftUpperArm, leftLowerArm, rightUpperArm, rightLowerArm, leftUpperLeg, leftLowerLeg, rightUpperLeg, rightLowerLeg;


void dibujarTorso(){
     if(segmento==0)
        glColor3f(0.0, 1.0, 0.0);
     glPushMatrix();
         glScalef(1.0,1.5,1.0);
         if (displayMode==1)
            glutWireCube(1);
         else
            glutSolidCube(1);

     glPopMatrix();
     glColor3f(1.0, 0.0, 0.0);
}

void dibujarHead(){
    if(segmento==1)
        glColor3f(0.0, 1.0, 0.0);
     glPushMatrix();
         glScalef(0.5,0.5,0.5);
         if (displayMode==1)
            glutWireCube(1);
         else
            glutSolidCube(1);

     glPopMatrix();
     glColor3f(1.0, 0.0, 0.0);
}

void dibujarLeftUpperArm(){
    if(segmento==2)
        glColor3f(0.0, 1.0, 0.0);
      glPushMatrix();
         glTranslatef(0.0f, -0.5f, 0.0f);
         glScalef(0.3,1.0,0.3);
         if (displayMode==1)
            glutWireCube(1);
         else
            glutSolidCube(1);
     glPopMatrix();
     glColor3f(1.0, 0.0, 0.0);
}

void dibujarLeftLowerArm(){
    if(segmento==3)
        glColor3f(0.0, 1.0, 0.0);
      glPushMatrix();
         glTranslatef(0.0f, -0.35f, 0.0f);
         glScalef(0.3,0.7,0.3);
         if (displayMode==1)
            glutWireCube(1);
         else
            glutSolidCube(1);
     glPopMatrix();
     glColor3f(1.0, 0.0, 0.0);
}

void dibujarLeftUpperLeg(){
    if(segmento==6)
        glColor3f(0.0, 1.0, 0.0);
      glPushMatrix();
         glTranslatef(0.0f, -0.5f, 0.0f);
         glScalef(0.4,1.0,0.4);
         if (displayMode==1)
            glutWireCube(1);
         else
            glutSolidCube(1);
     glPopMatrix();
     glColor3f(1.0, 0.0, 0.0);
}

void dibujarLeftLowerLeg(){
     if(segmento==7)
        glColor3f(0.0, 1.0, 0.0);
      glPushMatrix();
         glTranslatef(0.0f, -0.35f, 0.0f);
         glScalef(0.3,0.7,0.3);
         if (displayMode==1)
            glutWireCube(1);
         else
            glutSolidCube(1);
     glPopMatrix();
     glColor3f(1.0, 0.0, 0.0);
}

void dibujarRightUpperArm(){
    if(segmento==4)
        glColor3f(0.0, 1.0, 0.0);
      glPushMatrix();
         glTranslatef(0.0f, -0.5f, 0.0f);
         glScalef(0.3,1.0,0.3);
         if (displayMode==1)
            glutWireCube(1);
         else
            glutSolidCube(1);
     glPopMatrix();
     glColor3f(1.0, 0.0, 0.0);
}

void dibujarRightLowerArm(){
    if(segmento==5)
        glColor3f(0.0, 1.0, 0.0);
      glPushMatrix();
         glTranslatef(0.0f, -0.35f, 0.0f);
         glScalef(0.3,0.7,0.3);
         if (displayMode==1)
            glutWireCube(1);
         else
            glutSolidCube(1);
     glPopMatrix();
     glColor3f(1.0, 0.0, 0.0);
}

void dibujarRightUpperLeg(){
    if(segmento==8)
        glColor3f(0.0, 1.0, 0.0);
      glPushMatrix();
         glTranslatef(0.0f, -0.5f, 0.0f);
         glScalef(0.4,1.0,0.4);
         if (displayMode==1)
            glutWireCube(1);
         else
            glutSolidCube(1);
     glPopMatrix();
     glColor3f(1.0, 0.0, 0.0);
}

void dibujarRightLowerLeg(){
    if(segmento==9)
        glColor3f(0.0, 1.0, 0.0);
      glPushMatrix();
         glTranslatef(0.0f, -0.35f, 0.0f);
         glScalef(0.3,0.7,0.3);
         if (displayMode==1)
            glutWireCube(1);
         else
            glutSolidCube(1);
     glPopMatrix();
     glColor3f(1.0, 0.0, 0.0);
}

void init(){
	glColor3f(1.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

    //TORSO
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, torso.m); // guardar la matriz actual en m
    torso.f = dibujarTorso;                           // torso() – función que dibuja
    torso.sibling = NULL;                      // no hay hermanos
    torso.child = &head;                  // primer hijo

    //HEAD
    glLoadIdentity();
    glTranslatef(0.0,1.1,0.0);   //traslada al lugar que ocupa en el cuerpo
    glGetFloatv(GL_MODELVIEW_MATRIX, head.m);
    head.f = dibujarHead;
    head.sibling = &leftUpperArm;
    head.child = NULL;

    //LEFT UPPER ARM
    glLoadIdentity();
    glTranslatef(-0.8,0.7,0.0);   //traslada al lugar que ocupa en el cuerpo
    glGetFloatv(GL_MODELVIEW_MATRIX, leftUpperArm.m);
    leftUpperArm.f = dibujarLeftUpperArm;
    leftUpperArm.sibling = &rightUpperArm;
    leftUpperArm.child = &leftLowerArm;

    //LEFT LOWER ARM
    glLoadIdentity();
    glTranslatef(0.0,-1.1,0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, leftLowerArm.m);
    leftLowerArm.f = dibujarLeftLowerArm;
    leftLowerArm.sibling = NULL;
    leftLowerArm.child = NULL;

    //RIGHT UPPER ARM
    glLoadIdentity();
    glTranslatef(0.8,0.7,0.0);   //traslada al lugar que ocupa en el cuerpo
    glGetFloatv(GL_MODELVIEW_MATRIX, rightUpperArm.m);
    rightUpperArm.f = dibujarRightUpperArm;
    rightUpperArm.sibling = &leftUpperLeg;
    rightUpperArm.child = &rightLowerArm;

    //RIGHT LOWER ARM
    glLoadIdentity();
    glTranslatef(0.0,-1.1,0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, rightLowerArm.m);
    rightLowerArm.f = dibujarRightLowerArm;
    rightLowerArm.sibling = NULL;
    rightLowerArm.child = NULL;

    //LEFT UPPER LEG
    glLoadIdentity();
    glTranslatef(-0.3 ,-0.8,0.0);   //traslada al lugar que ocupa en el cuerpo
    glGetFloatv(GL_MODELVIEW_MATRIX, leftUpperLeg.m);
    leftUpperLeg.f = dibujarLeftUpperLeg;
    leftUpperLeg.sibling = &rightUpperLeg;
    leftUpperLeg.child = &leftLowerLeg;

    //LEFT LOWER Leg
    glLoadIdentity();
    glTranslatef(0.0,-1.1,0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, leftLowerLeg.m);
    leftLowerLeg.f = dibujarLeftLowerLeg;
    leftLowerLeg.sibling = NULL;
    leftLowerLeg.child = NULL;

    //RIGHT UPPER LEG
    glLoadIdentity();
    glTranslatef(0.3 ,-0.8,0.0);   //traslada al lugar que ocupa en el cuerpo
    glGetFloatv(GL_MODELVIEW_MATRIX, rightUpperLeg.m);
    rightUpperLeg.f = dibujarRightUpperLeg;
    rightUpperLeg.sibling = NULL;
    rightUpperLeg.child = &rightLowerLeg;

    //RIGHT LOWER Leg
    glLoadIdentity();
    glTranslatef(0.0,-1.1,0.0);
    glGetFloatv(GL_MODELVIEW_MATRIX, rightLowerLeg.m);
    rightLowerLeg.f = dibujarRightLowerLeg;
    rightLowerLeg.sibling = NULL;
    rightLowerLeg.child = NULL;
}

// recorrido a profundidad: primero los hijos, luego los hermanos
void traverse (treenode *node)
{
    // guardar la matriz actual porque las transformaciones a realizarse
    // sólo deben afectarle a él y a sus hijos
    glPushMatrix();

        // transformar relativo a su padre
        glMultMatrixf(node->m);

        // dibujar el nodo
        node->f();

        // primer recorrer los hijos (si hay)
        if (node->child != NULL)
        	traverse(node->child);

    glPopMatrix();
    // después recorrer los hermanos (si hay)
    if(node->sibling != NULL)
    	traverse(node->sibling);
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(viewer[0],viewer[1],viewer[2],0,0,0,0,1,0);

    glRotatef(rotationX,1,0,0);
    glRotatef(rotationY,0,1,0);

    glPushMatrix();
        traverse(&torso);
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble)w/(GLdouble)h, 1.0, 100.0);
    glViewport(0,0,w,h);
}

void specialKey(int c, int x, int y){
	if(c == GLUT_KEY_PAGE_UP || c== GLUT_KEY_PAGE_DOWN) {
	    if(c == GLUT_KEY_PAGE_DOWN){
            segmento=(segmento+1)%10 ;
            glutPostRedisplay();
	    }
	    else{
            segmento=(segmento-1)%10 ;
            if(segmento < 0)
                segmento = 9;
            glutPostRedisplay();
	    }
	}else{
        if(segmento == 0){
            if(c == GLUT_KEY_LEFT) {
                anguloTorsoY=+5.0;
            }
            if(c == GLUT_KEY_RIGHT) {
                anguloTorsoY=-5.0;
            }
        }
        if(c == GLUT_KEY_UP) {
            angulosX[segmento]=+5.0;
        }
        if(c == GLUT_KEY_DOWN) {
            angulosX[segmento]=-5.0;
        }
        if(c == GLUT_KEY_LEFT) {
            angulosZ[segmento]=+5.0;
        }
        if(c == GLUT_KEY_RIGHT) {
            angulosZ[segmento]=-5.0;
        }
        glPushMatrix();
        glLoadIdentity();

           switch(segmento){
              case 0:
                  glMultMatrixf(torso.m);
                  glRotatef(anguloTorsoY,0.0,1.0,0.0);
                  glGetFloatv(GL_MODELVIEW_MATRIX, torso.m);
                  break;
              case 1:
                  glMultMatrixf(head.m);
                  glRotatef(angulosX[segmento],1.0,0.0,0.0);
                  glRotatef(angulosZ[segmento],0.0,0.0,1.0);
                  glGetFloatv(GL_MODELVIEW_MATRIX, head.m);
                  break;
              case 2:
                  glMultMatrixf(leftUpperArm.m);
                  glRotatef(angulosX[segmento],1.0,0.0,0.0);
                  glRotatef(angulosZ[segmento],0.0,0.0,1.0);
                  glGetFloatv(GL_MODELVIEW_MATRIX, leftUpperArm.m);
                  break;
              case 3:
                  glMultMatrixf(leftLowerArm.m);
                  glRotatef(angulosX[segmento],1.0,0.0,0.0);
                  glRotatef(angulosZ[segmento],0.0,0.0,1.0);
                  glGetFloatv(GL_MODELVIEW_MATRIX, leftLowerArm.m);
                  break;
              case 4:
                  glMultMatrixf(rightUpperArm.m);
                  glRotatef(angulosX[segmento],1.0,0.0,0.0);
                  glRotatef(angulosZ[segmento],0.0,0.0,1.0);
                  glGetFloatv(GL_MODELVIEW_MATRIX, rightUpperArm.m);
                  break;
              case 5:
                  glMultMatrixf(rightLowerArm.m);
                  glRotatef(angulosX[segmento],1.0,0.0,0.0);
                  glRotatef(angulosZ[segmento],0.0,0.0,1.0);
                  glGetFloatv(GL_MODELVIEW_MATRIX, rightLowerArm.m);
                  break;
              case 6:
                  glMultMatrixf(leftUpperLeg.m);
                  glRotatef(angulosX[segmento],1.0,0.0,0.0);
                  glRotatef(angulosZ[segmento],0.0,0.0,1.0);
                  glGetFloatv(GL_MODELVIEW_MATRIX, leftUpperLeg.m);
                  break;
              case 7:
                  glMultMatrixf(leftLowerLeg.m);
                  glRotatef(angulosX[segmento],1.0,0.0,0.0);
                  glRotatef(angulosZ[segmento],0.0,0.0,1.0);
                  glGetFloatv(GL_MODELVIEW_MATRIX, leftLowerLeg.m);
                  break;
              case 8:
                  glMultMatrixf(rightUpperLeg.m);
                  glRotatef(angulosX[segmento],1.0,0.0,0.0);
                  glRotatef(angulosZ[segmento],0.0,0.0,1.0);
                  glGetFloatv(GL_MODELVIEW_MATRIX, rightUpperLeg.m);
                  break;
              case 9:
                  glMultMatrixf(rightLowerLeg.m);
                  glRotatef(angulosX[segmento],1.0,0.0,0.0);
                  glRotatef(angulosZ[segmento],0.0,0.0,1.0);
                  glGetFloatv(GL_MODELVIEW_MATRIX, rightLowerLeg.m);
                  break;
           }

       glPopMatrix();
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
   if(key == '0') segmento=0;
   if(key == '1') segmento=1;
   if(key == '2') segmento=2;
   if(key == '3') segmento=3;
   if(key == '4') segmento=4;
   if(key == '5') segmento=5;
   if(key == '6') segmento=6;
   if(key == '7') segmento=7;
   if(key == '8') segmento=8;
   if(key == '9') segmento=9;
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
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Robot");
    glutInitWindowSize(500,500);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(key);
    glutSpecialFunc(specialKey);
    addMenu();
    init();
    glutMainLoop();
}
