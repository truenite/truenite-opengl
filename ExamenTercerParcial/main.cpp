/***************************************************
Materia: Gráficas Computacionales
Examen:  Tercer Parcial -- Examen Practico
Fecha:   7 de abril del 2011
Autores: 1162205 Diego Alfonso García Mendiburu
        1163990  Juan Andrés Rocha
***************************************************/

#include <windows.h>
#include "mesh.h"
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <time.h>

GLboolean mostrarTextura = 1;

float rotationX=0.0;
float rotationY=0.0;
float prevX=0.0;
float prevY=0.0;
bool mouseDown=false;
mesh *object;
float pos[3], ambient[3], difusse[3], specular[3];
float env[3], eye[3], vat[3], vup[3];
float fovy, dnear, dfar;
float viewport[4];
GLuint texName=0;
//Glfloat position[] = {1.0, 2.0, 0.0, 1.0};
unsigned char woodtexture[512][512][3];

void initTexture(){
   int w, h, d;
   FILE *fp;
   int i, j, k;
   fp = fopen("Madera.ppm","rb");
   fscanf(fp,"%*s ");
   fscanf(fp,"%d %d %d ", &w, &h, &d) ;
   for (i = 0 ; i < w ; i++)
     for (j = 0 ; j < h ; j++)
       for (k = 0 ; k < 3 ; k++)
	     fscanf(fp,"%c",&(woodtexture[i][j][k])) ;
   fclose(fp) ;

   glGenTextures(1, &texName) ;
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) ;
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) ;
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) ;
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGB, 512, 512, 0, GL_RGB,
	   GL_UNSIGNED_BYTE, woodtexture);
}


void loadView(string file){

	char token[100];

	FILE *scene;
	scene = fopen(file.c_str(),"r");

    if (!scene)
	{
		cout<< string("Can not open light File \"") << file << "\" !" << endl;
		return;
	}

	cout<<endl<<file<<endl;

	while(!feof(scene))
	{
		token[0] = NULL;
		fscanf(scene,"%s", token);

        if (!strcmp(token,"light"))
		{
            fscanf(scene,"%f %f %f",&pos[0],&pos[1],&pos[2]);
            fscanf(scene,"%f %f %f",&ambient[0],&ambient[1],&ambient[2]);
            fscanf(scene,"%f %f %f",&difusse[0],&difusse[1],&difusse[2]);
            fscanf(scene,"%f %f %f",&specular[0],&specular[1],&specular[2]);
            glLightfv(GL_LIGHT0, GL_POSITION, pos);
            glLightfv(GL_LIGHT0, GL_AMBIENT,   ambient);
            glLightfv(GL_LIGHT0, GL_SPECULAR,  difusse);
            glLightfv(GL_LIGHT0, GL_DIFFUSE,   specular);

 		}
        if (!strcmp(token,"ambient"))
		{
            fscanf(scene,"%f %f %f",&env[0],&env[1],&env[2]);
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, env);
		}
        if (!strcmp(token,"eye"))
		{
            fscanf(scene,"%f %f %f",&eye[0],&eye[1],&eye[2]);
		}
        if (!strcmp(token,"vat"))
		{
            fscanf(scene,"%f %f %f",&vat[0],&vat[1],&vat[2]);
		}
		if (!strcmp(token,"vup"))
		{
            fscanf(scene,"%f %f %f",&vup[0],&vup[1],&vup[2]);
		}
		if (!strcmp(token,"fovy"))
		{
            fscanf(scene,"%f",&fovy);
		}
		if (!strcmp(token,"dnear"))
		{
            fscanf(scene,"%f",&dnear);
		}
		if (!strcmp(token,"dfar"))
		{
            fscanf(scene,"%f",&dfar);
		}
		if (!strcmp(token,"viewport"))
		{
            fscanf(scene,"%f %f %f %f",&viewport[0],&viewport[1],&viewport[2],&viewport[3]);
		}
	}

}

void dibujarMalla(void){
    float u=0.0,v=0.0;
    int numCuadrado=-1;
    int renglon=0;
    int columna=-1;
    int dimension=sqrt(object->fTotal/2);
    float deltaUV=1.0/dimension;

    for (int i=0;i<object->fTotal;i++){
        glBegin(GL_TRIANGLES);
        if(i%2==0){
            numCuadrado++;
            columna++;
            if(columna>0 && columna%dimension==0){
                renglon++;
                columna=0;
            }
        }
        for (int j=0;j<3;j++){
            u=deltaUV*columna;
            v=deltaUV*renglon;
            if(i%2==0){
                if(j==1) u=u+deltaUV;
                if(j==2) v=v+deltaUV;
            }else{
                if(j==0) v=v+deltaUV;
                if(j==1) u=u+deltaUV;
                if(j==2){
                    u=u+deltaUV;
                    v=v+deltaUV;
                }
            }
            //glNormal3fv(object->nList[object->faceList[i][j].n].ptr);
            glTexCoord2f(u, v);
            glVertex3f(object->vList[object->faceList[i][j].v].ptr[0],object->vList[object->faceList[i][j].v].ptr[1],object->vList[object->faceList[i][j].v].ptr[2]);
        }
         glEnd();
    }
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0],eye[1],eye[2],vat[0],vat[1],vat[2],vup[0],vup[1],vup[2]);
    glRotatef(rotationX, 1, 0, 0);
    glRotatef(rotationY, 0, 1, 0);
    dibujarMalla();
    glutSwapBuffers();
}

void reshape(int w, int h){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, (GLdouble)w/(GLdouble)h, dnear, dfar);
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
   if(key == 'x') eye[0]-= 0.2;
   if(key == 'X') eye[0]+= 0.2;
   if(key == 'y') eye[1]-= 0.2;
   if(key == 'Y') eye[1]+= 0.2;
   if(key == 'z') eye[2]-= 0.2;
   if(key == 'Z') eye[2]+= 0.2;
   if(key == 't' || key == 'T'){
        if(mostrarTextura)
            glEnable(GL_TEXTURE_2D);
        else
            glDisable(GL_TEXTURE_2D);
        mostrarTextura = !mostrarTextura;
   }
   if(key == 'j' || key == 'J'){
        pos[0]+=.011;
        printf("a");
   }

   glutPostRedisplay();
}

void menu(int val){
    if(val == 1)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

void init(){
    loadView("malla.view");
	object = new mesh("malla.obj");

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D) ;
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE) ;
    glBindTexture(GL_TEXTURE_2D, texName) ;

    initTexture();
}

main(int argc, char **argv){
    srand(time(NULL));
//    glLight3f(GL_LIGHT0, GL_POSITION, pos);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Examen Tercer Parcial Practico");
    glutInitWindowSize(viewport[2],viewport[3]);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(key);
    addMenu();
    init();
    glutMainLoop();
    delete object;
}




