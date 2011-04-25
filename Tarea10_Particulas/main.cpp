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

GLfloat gravedad[3] = {0.0f,-0.001f, 0.0f};
GLfloat springConstant=0.05f;
static GLfloat timeStepSize = .5*.5*1;
GLint ventanaX = 500;
GLint ventanaY = 500;
static GLint tamanoVista = 100;
static GLint minDiv = 10;
static GLint maxDiv = 40;
GLfloat divs = 15;
static GLfloat tamanoLadoMalla = 18;
GLfloat xInit = -9.0f;
GLfloat yInit = 9;
GLfloat zInit = 9.0f;
Particle *malla = (Particle *)malloc(sizeof(Particle));
Spring *springs=(Spring*)malloc(sizeof(Spring));
GLuint texName=0;
unsigned char woodtexture[512][512][3];
GLint imageSize = 512;
float LightPos[] = { 1.0f, 1.0f, 1.0f, 1.0f};   // Light Position
float LightAmb[] = { 0.30f, 0.30f, 0.30f, 0.0f};   // Ambient Light Values
float LightDif[] = { 1.0f, 1.0f, 1.0f, 1.0f};   // Diffuse Light Values
float LightSpc[] = { 1.0f, 1.0f, 1.0f, 1.0f};   // Specular Light Values
GLfloat rotationX=0.0;
GLfloat rotationY=0.0;
GLfloat prevX=0.0;
GLfloat prevY=0.0;
bool mouseDown=false;
bool prueba=false;
GLfloat viewer[]= {0.0, 0.0, 25.0};
GLint viewMode = 0;
GLint shadeModel = 0;
GLboolean mostrarTextura = 0;
GLboolean mostrarParticulas = 0;
bool started = false;
float radioEsfera=4.0f;

void idle(){
    if (started) {
        Particle *te = malla->next->child;
        //printf("x1: %f, y1:%f,  z1: %f\n",te->pos[0],te->pos[1],te->pos[2]);
        sumarFuerzaMalla(malla,gravedad[0]*timeStepSize,gravedad[1]*timeStepSize,gravedad[2]*timeStepSize);
        Spring *temp = springs;
        for(;temp->next;temp=temp->next)
            contrae(temp);
        contrae(temp);
        Particle *tempCol = malla->next;
        Particle *tempM = malla;
        for(;tempCol->next;tempCol=tempCol->next){
            tempM = tempCol;
            for(;tempM->child;tempM=tempM->child){
                timeStep(tempM,timeStepSize,radioEsfera);
            }
            timeStep(tempM,timeStepSize,radioEsfera);
        }
        tempM = tempCol;
        for(;tempM->child;tempM=tempM->child){
            timeStep(tempM,timeStepSize,radioEsfera);
        }
        timeStep(tempM,timeStepSize,radioEsfera);
        //printf("x2: %f, y2:%f,  z2: %f\n",te->pos[0],te->pos[1],te->pos[2]);
        glutPostRedisplay();
        //started=!started;
    }
}

void crearResortes(){
    Particle *tempCol = malla->next;
    Particle *temp;
    Spring *tempSpring = springs;
    for(;tempCol->next;tempCol=tempCol->next){
        temp = tempCol;
        for(;temp->child;temp=temp->child){
            Spring *newSpring;
            newSpring = createSpring(temp,temp->next,springConstant);
            tempSpring->next = newSpring;
            tempSpring = newSpring;
            newSpring = createSpring(temp,temp->next->child,springConstant);
            tempSpring->next = newSpring;
            tempSpring = newSpring;
            newSpring = createSpring(temp,temp->child,springConstant);
            tempSpring->next = newSpring;
            tempSpring = newSpring;
            //temp->selected=1;
            if(temp->father){
                newSpring = createSpring(temp,temp->father->next,springConstant);
                tempSpring->next = newSpring;
                tempSpring = newSpring;
            }
        }
        Spring *newSpring = createSpring(temp,temp->next,springConstant);
        tempSpring->next = newSpring;
        tempSpring = newSpring;
        if(temp->father->next){
            Spring *newSpring = createSpring(temp,temp->father->next,springConstant);
            tempSpring->next = newSpring;
            tempSpring = newSpring;
        }
    }
    temp = tempCol;
    for(;temp->child;temp=temp->child){
        Spring *newSpring = createSpring(temp,temp->child,springConstant);
        tempSpring->next = newSpring;
        tempSpring = newSpring;
        //temp->selected=1;
        //printf("x:%f  y:%f z:%f\n",temp->child->pos[0],temp->child->pos[1],temp->child->pos[2]);
    }
    countSprings(springs);
}

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
        if(prueba)
            newOne->fixed=1;
        for(GLint j = 1;j<=divs;j++){
            z-=(tamanoLadoMalla/divs);
            Particle *newTwo = createParticle(x,y,z);//18*(j/divs)*2,z);
            temp2->child = newTwo;
            newTwo->father = temp2;
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
        temp2->next = temp3;
    }
    crearResortes();
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
    if(mostrarParticulas){
        dibujarParticulas(malla);
    }
    glColor3f(0.0,1.0,0.0);
    static GLUquadricObj * sphere=gluNewQuadric();
	gluSphere(sphere, radioEsfera, 48, 24);
    //drawSprings(springs);
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
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D) ;
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE) ;
    //glColorMaterial(GL_AMBIENT_AND_DIFFUSE);
    glBindTexture(GL_TEXTURE_2D, texName) ;
    glFrontFace(GL_CW);
    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);

    initTexture();
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
        case 32:
            started = !started;
            //lastUpdate = glutGet(GLUT_ELAPSED_TIME);
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
    glutIdleFunc(idle);
    glutMainLoop();
}
