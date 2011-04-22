/***************************************************
Materia: Gráficas Computacionales
Tarea: 10 Particulas
Fecha: 11 de Abril del 2011
Autor 1: 1162205 Diego Alfonso García Mendiburu
***************************************************/

#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "controladorMalla.c"


GLint ventanaX = 500;
GLint ventanaY = 500;
static GLint tamanoVista = 100;
static GLint minDiv = 10;
static GLint maxDiv = 100;
GLfloat divs = 10;
static GLfloat tamanoLadoMalla = 18;
GLfloat xInit = -9.0f;
GLfloat yInit = 0.0;
GLfloat zInit = 9.0f;
Particle *malla = (Particle *)malloc(sizeof(Particle));
GLuint texName=0;
unsigned char woodtexture[512][512][3];
GLint imageSize = 512;
float LightPos[] = { 1.0f, 0.5f, 1.0f, 0.0f};   // Light Position
float LightAmb[] = { 0.2f, 0.2f, 0.2f, 1.0f};   // Ambient Light Values
float LightDif[] = { 1.0f, 1.0f, 1.0f, 1.0f};   // Diffuse Light Values
float LightSpc[] = { 1.0f, 1.0f, 1.0f, 1.0f};   // Specular Light Values
GLfloat rotationX=0.0;
GLfloat rotationY=0.0;
GLfloat prevX=0.0;
GLfloat prevY=0.0;
bool mouseDown=false;
GLfloat viewer[]= {0.0, 0.0, 25.0};
GLint viewMode = 0;
GLint shadeModel = 0;
GLboolean mostrarTextura = 0;
GLboolean mostrarParticulas = 0;


void crearMalla(){
    GLfloat x = xInit;
    GLfloat y = yInit;
    GLfloat z = zInit;
    Particle *temp = malla;
    Particle *temp2;
    for(GLint i = 0;i<=divs;i++){
        Particle *newOne = createParticle(x,y,z);
        temp->next = newOne;
        temp = newOne;
        temp2 = temp;
        for(GLint j = 1;j<=divs;j++){
            z-=(tamanoLadoMalla/divs);
            Particle *newTwo = createParticle(x,y,z);
            temp2->child = newTwo;
            temp2 = newTwo;
        }
        z = zInit;
        x+=(tamanoLadoMalla/divs);
    }
    // Unimos todos los hijos con sus nexts
    temp = malla->next;
    for(;temp->next;temp=temp->next){
        Particle *temp2 = temp;
        Particle *temp3 = temp->next;
        for(;temp2->child;temp2=temp2->child,temp3=temp3->child)
            temp2->next = temp3;

    }
}
void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(viewer[0],viewer[1],viewer[2],0,0,0,0,1,0);
    glRotatef(rotationX,1,0,0);
    glRotatef(rotationY,0,1,0);
    if(viewMode == 0)
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if(shadeModel == 0)
        glShadeModel(GL_SMOOTH);
    else
        glShadeModel(GL_FLAT);


    // AQUI EMPIEZA DIBUJABLE


    glColor3f(1.0,1.0,1.0);
    dibujarMalla(malla,divs);
    if(mostrarParticulas)
        dibujarParticulas(malla);


    // AQUI TERMINA DIBUJABLE
    glutSwapBuffers();
}

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

void init(){
    //glClearColor(1,1,1,1);
    glColor3f(0,0,0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    /* Activo la fuente de luz */
    glLightfv(GL_LIGHT0, GL_POSITION, LightPos);        // Set Light1 Position
    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmb);         // Set Light1 Ambience
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDif);         // Set Light1 Diffuse
    glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpc);        // Set Light1 Specular
    glEnable(GL_LIGHT0);                                // Enable Light1
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D) ;
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE) ;
    glBindTexture(GL_TEXTURE_2D, texName) ;

    //initTexture();
    crearMalla();
}

void key(unsigned char key, int x, int y){
    switch (key){
        case 27 :
        case 'q':
            exit(0);
            break;
        case '+':
            if(divs < maxDiv){
                divs++;
                crearMalla();
            }
            break;

        case '-':
            if(divs > minDiv){
                divs--;
                crearMalla();
            }
            break;
        case 'x':
            viewer[0]-= 0.1;
            break;
        case 'X':
            viewer[0]+= 0.1;
            break;
        case 'z':
            viewer[2]-= 0.1;
            break;
        case 'Z':
            viewer[2]+= 0.1;
            break;
        case 'y':
            viewer[1]-= 0.1;
            break;
        case 'Y':
            viewer[1]+= 0.1;
            break;
        case 'w':
            viewMode = (viewMode+1)%2;
            break;
        case 's':
            shadeModel = (shadeModel+1)%2;
            break;
        case 't':
            if(mostrarTextura)
                glEnable(GL_TEXTURE_2D);
            else
                glDisable(GL_TEXTURE_2D);
            mostrarTextura = !mostrarTextura;
            break;
        case 'p':
            mostrarParticulas = !mostrarParticulas;
            break;
    }
    glutPostRedisplay();
}
void reshape(int w, int h){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble)w/(GLdouble)h, 1.0, 100.0);
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
int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(ventanaX,ventanaY);
    glutInitWindowPosition (250,250);
    glutCreateWindow("Tarea10_Particulas");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(key);
    glutMainLoop();
}
