/***************************************************
Materia: Gr�ficas Computacionales
Tarea 8: Luces
Fecha: 19 de Marzo de 2011
Autor 1: 1162205 Diego Alfonso Garc�a Mendiburu
***************************************************/

#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include "mesh.h"
#include <iostream>

float rotationX=0.0;
float rotationY=0.0;
float prevX=0.0;
float prevY=0.0;
bool mouseDown=false;
float viewer[]= {0.0, 0.0, 500.0};
int displayMode=2;

GLfloat difusse[4], ambient[4], specular[4], position[4], globalAmbient[4];

mesh *object;

void lights(){
	char token[100];
    FILE *scene;
    string file = "bunny69k.view";
	scene = fopen(file.c_str(),"r");

    while(!feof(scene))
	{
		token[0] = NULL;
		fscanf(scene,"%s", token);

        if (!strcmp(token,"light"))
		{
            fscanf(scene,"%f %f %f",&position[0],&position[1],&position[2]);
            glLightfv(GL_LIGHT0, GL_POSITION, position);
            fscanf(scene,"%f %f %f",&ambient[0],&ambient[1],&ambient[2]);
            glLightfv(GL_LIGHT0, GL_AMBIENT,   ambient);
            fscanf(scene,"%f %f %f",&difusse[0],&difusse[1],&difusse[2]);
            glLightfv(GL_LIGHT0, GL_SPECULAR,  difusse);
            fscanf(scene,"%f %f %f",&specular[0],&specular[1],&specular[2]);
            glLightfv(GL_LIGHT0, GL_DIFFUSE,   specular);

 		}
        else if (!strcmp(token,"ambient"))
		{
            fscanf(scene,"%f %f %f",&globalAmbient[0],&globalAmbient[1],&globalAmbient[2]);
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
		}

	}

}


void init(){
	object = new mesh("bunny69k.obj");
	lights();
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(viewer[0],viewer[1],viewer[2],0,0,0,0,1,0);

    glRotatef(rotationX,1,0,0);
    glRotatef(rotationY,0,1,0);
    glColor3f(1.0, 1.0, 1.0);
    if(displayMode==1){
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        //glBegin(GL_LINES);// wire
    }
    else{
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        // solid
    }
    glBegin(GL_TRIANGLES);
    for (int i=0;i < object->fTotal; i++)
    {
        for(int j = 0; j < 3;j++){
            glNormal3fv(object->nList[object->faceList[i][j].n].ptr);
            glVertex3fv(object->vList[object->faceList[i][j].v].ptr);
        }
    }
    glEnd();
    glutSwapBuffers();
}

void reshape(int w, int h){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble)w/(GLdouble)h, 0.0, 500.0);
    glViewport(0,0,w,h);
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
	glutAddMenuEntry("Wireframe", 1);
	glutAddMenuEntry("Solid", 2);
	glutSetMenu(mainMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Segundo Parcial");
    glutInitWindowSize(500,500);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(key);
    gluPerspective(45.0,1.0,0,500);
    addMenu();
    init();
    glutMainLoop();
}
