/***************************************************
Materia: Gráficas Computacionales
Tarea: 10 Particulas y Proyecto final
Fecha: 11 de Abril del 2011
Autor 1: 1162205 Diego Alfonso García Mendiburu
***************************************************/

#include <windows.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "controladorMalla.c"

GLfloat damping = 0.033f;
GLfloat magGravedad = -0.005f;
GLint gravedad[3] = {0,1,0};
GLfloat springConstant=0.03f;
static GLfloat timeStepSize = 0.5*0.5;
GLint ventanaX = 800;
GLint ventanaY = 600;
static GLint tamanoVista = 100;
static GLint minDiv = 10;
static GLint maxDiv = 40;
GLint divs = 15;
static GLfloat tamanoLadoMalla = 18;
GLfloat xInit = -9.0f;
GLfloat yInit = 9;
GLfloat zInit = 9.0f;
Particle *malla = (Particle *)malloc(sizeof(Particle));
Spring *springs=(Spring*)malloc(sizeof(Spring));
GLuint texName=0;
unsigned char woodtexture[512][512][3];
GLint imageSize = 512;
GLfloat prevX=0.0;
GLfloat prevY=0.0;
bool mouseDown=false;
bool prueba=false;
GLfloat viewer[]= {0.0, 0.0, 25.0};
GLint viewMode = 0;
GLint shadeModel = 0;
GLint mostrarTextura = 0;
GLint mostrarParticulas = 0;
GLint started = 0;
GLint paused = 1;
GLfloat radioEsfera=4.0f;
GLfloat radioParticulas = 0.2f;
GLfloat posicionEsferasCilindro[] = {0.0f,0.0f,3.0f,0.0f,0.0f,-3.0f};
GLint collider = 0;
GLint seleccionada = 0;
GLfloat distParticulaCabello = 0.3f;
GLint numParticulasCabello = 3;
GLdouble mposX, mposY, mposZ; // posicion 3d del mouse
GLint hacerTiras=0;
/** GLUI **/

float xy_aspect;
int   last_x, last_y;
float rotationX = 0.0, rotationY = 0.0;

/** These are the live variables passed into GLUI ***/
int   wireframe = 0;
int   obj_type = 1;
int   segments = 8;
int   segments2 = 8;
int   light0_enabled = 1;
float light0_intensity = 1.0;
int   main_window;
float scale = 1.0;
int   show_sphere=1;
int   show_torus=1;
int   show_axes = 1;
int   show_text = 1;
float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, 0.0 };
float collider_pos[] = { 0.0, 0.0, 0.0 };
char *string_list[] = { "Hello World!", "Foo", "Testing...", "Bounding box: on" };
int   curr_string = 0;

/** Pointers to the windows and some of the controls we'll create **/
GLUI *glui, *glui2;
GLUI_Spinner    *light0_spinner, *spinnerDivs, *tiras;
GLUI_RadioGroup *radio;
GLUI_Panel *obj_panel;
GLUI_Checkbox *fijar;
/********** User IDs for callbacks ********/
#define LIGHT0_ENABLED_ID    200
#define LIGHT0_INTENSITY_ID  250
#define DIVISIONES_ID        101
#define STARTED_ID           102
#define PAUSED_ID            103
#define RESET_ID             104
#define OBJECT_TYPE_ID       105
#define HACER_TIRAS_ID       106
#define FIJAR_ID             107

/*** LIGHTS ***/
GLfloat lights_rotation[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float light_position[] = {.5f, .5f, 1.0f, 0.0f};   // Light Position
float light_ambient[] = {0.1f, 0.1f, 0.3f, 1.0f};   // Ambient Light Values
float light_diffuse[] = {.6f, .6f, 1.0f, 1.0f};   // Diffuse Light Values

/**************************************** control_cb() *******************/
/* GLUI control callback                                                 */

void idle(void){
    /*if(collider){
        for(int i = 0; i <3 ; i++)
            collider_pos[i] = sqrt(posicionEsferasCilindro[i]*posicionEsferasCilindro[i])-sqrt(posicionEsferasCilindro[i+3]*posicionEsferasCilindro[i+3]);
    }*/
    if(glutGetWindow() != main_window)
        glutSetWindow(main_window);
    if (started && !paused) {
        //printf("x1: %f, y1:%f,  z1: %f\n",te->pos[0],te->pos[1],te->pos[2]);
        sumarFuerzaMalla(malla,magGravedad*(GLfloat)gravedad[0]*timeStepSize,magGravedad*(GLfloat)gravedad[1]*timeStepSize,magGravedad*(GLfloat)gravedad[2]*timeStepSize);
        Particle *tira;
        Spring *temp = springs->next;
        for(;temp->next;temp=temp->next)
            contrae(temp, springConstant);
        contrae(temp,springConstant);
        Particle *tempCol = malla->next;
        Particle *tempM = malla;
        for(;tempCol->next;tempCol=tempCol->next){
            tempM = tempCol;
            for(;tempM->child;tempM=tempM->child){
                timeStep(tempM,timeStepSize,damping);
                if(collider)
                    ;//colisionCapsula(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2],longitudCapsula);
                else
                    colisionEsfera(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2]);
                if(tempM->hair){
                    tira = tempM->hair;
                    for(;tira->hair;tira=tira->hair){
                        timeStep(tira,timeStepSize,damping);
                        if(collider)
                            ;//colisionCapsula(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2],longitudCapsula);
                        else
                            colisionEsfera(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2]);
                    }
                    timeStep(tira,timeStepSize,damping);
                    //printf("2posx: %f  posy: %f  posz: %f\n",tira->pos[0],tira->pos[1],tira->pos[2]);
                    if(collider)
                        ;//colisionCapsula(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2],longitudCapsula);
                    else
                        colisionEsfera(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2]);
                }
            }
            timeStep(tempM,timeStepSize,damping);
            if(collider)
                ;//colisionCapsula(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2],longitudCapsula);
            else
                colisionEsfera(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2]);
            if(tempM->hair){
                tira = tempM->hair;
                for(;tira->hair;tira=tira->hair){
                    timeStep(tira,timeStepSize,damping);
                    if(collider)
                        ;//colisionCapsula(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2],longitudCapsula);
                    else
                        colisionEsfera(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2]);
                }
                timeStep(tira,timeStepSize,damping);
                //printf("2posx: %f  posy: %f  posz: %f\n",tira->pos[0],tira->pos[1],tira->pos[2]);
                if(collider)
                    ;//colisionCapsula(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2],longitudCapsula);
                else
                    colisionEsfera(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2]);
            }
        }
        tempM = tempCol;
        for(;tempM->child;tempM=tempM->child){
            timeStep(tempM,timeStepSize,damping);
            if(collider)
                ;//colisionCapsula(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2],longitudCapsula);
            else
                colisionEsfera(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2]);
            if(tempM->hair){
                tira = tempM->hair;
                for(;tira->hair;tira=tira->hair){
                    //printf("1posx: %f  posy: %f  posz: %f\n",tira->pos[0],tira->pos[1],tira->pos[2]);
                    timeStep(tira,timeStepSize,damping);
                    //printf("2posx: %f  posy: %f  posz: %f\n",tira->pos[0],tira->pos[1],tira->pos[2]);
                    if(collider)
                        ;//colisionCapsula(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2],longitudCapsula);
                    else
                        colisionEsfera(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2]);
                }
                timeStep(tira,timeStepSize,damping);
                //printf("2posx: %f  posy: %f  posz: %f\n",tira->pos[0],tira->pos[1],tira->pos[2]);
                if(collider)
                    ;//colisionCapsula(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2],longitudCapsula);
                else
                    colisionEsfera(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2]);
            }
        }
        timeStep(tempM,timeStepSize,damping);
        if(collider)
            ;//colisionCapsula(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2],longitudCapsula);
        else
            colisionEsfera(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2]);
        if(tempM->hair){
            tira = tempM->hair;
            for(;tira->hair;tira=tira->hair){
                timeStep(tira,timeStepSize,damping);
                if(collider)
                    ;//colisionCapsula(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2],longitudCapsula);
                else
                    colisionEsfera(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2]);
            }
            timeStep(tira,timeStepSize,damping);
            //printf("2posx: %f  posy: %f  posz: %f\n",tira->pos[0],tira->pos[1],tira->pos[2]);
            if(collider)
                ;//colisionCapsula(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2],longitudCapsula);
            else
                colisionEsfera(tempM,radioEsfera,collider_pos[0],collider_pos[1],collider_pos[2]);
        }
        //printf("x2: %f, y2:%f,  z2: %f\n",te->pos[0],te->pos[1],te->pos[2]);
        //started=!started;
    }
    glutPostRedisplay();
}

void crearResortes(){
    Particle *tempCol = malla->next;
    Particle *temp;
    Spring *tempSpring = springs;
    Particle *tira;
    Spring *newSpring;
    for(;tempCol->next;tempCol=tempCol->next){
        temp = tempCol;
        for(;temp->child;temp=temp->child){
            newSpring = createSpring(temp,temp->next);
            tempSpring->next = newSpring;
            tempSpring = newSpring;
            newSpring = createSpring(temp,temp->next->child);
            tempSpring->next = newSpring;
            tempSpring = newSpring;
            newSpring = createSpring(temp,temp->child);
            tempSpring->next = newSpring;
            tempSpring = newSpring;
            //temp->selected=1;
            tira = temp;
            for(;tira->hair;tira=tira->hair){
                newSpring = createSpring(tira,tira->hair);
                tempSpring->next = newSpring;
                tempSpring = newSpring;
            }
            if(temp->father){
                newSpring = createSpring(temp,temp->father->next);
                tempSpring->next = newSpring;
                tempSpring = newSpring;
            }
        }
        newSpring = createSpring(temp,temp->next);
        tempSpring->next = newSpring;
        tempSpring = newSpring;
        tira = temp;
        for(;tira->hair;tira=tira->hair){
            newSpring = createSpring(tira,tira->hair);
            tempSpring->next = newSpring;
            tempSpring = newSpring;
        }
        if(temp->father->next){
            newSpring = createSpring(temp,temp->father->next);
            tempSpring->next = newSpring;
            tempSpring = newSpring;
        }
    }
    temp = tempCol;
    for(;temp->child;temp=temp->child){
        newSpring = createSpring(temp,temp->child);
        tempSpring->next = newSpring;
        tempSpring = newSpring;
        tira = temp;
        for(;tira->hair;tira=tira->hair){
            newSpring = createSpring(tira,tira->hair);
            tempSpring->next = newSpring;
            tempSpring = newSpring;
        }
        //temp->selected=1;
        //printf("x:%f  y:%f z:%f\n",temp->child->pos[0],temp->child->pos[1],temp->child->pos[2]);
    }
    tira = temp;
    for(;tira->hair;tira=tira->hair){
        newSpring = createSpring(tira,tira->hair);
        tempSpring->next = newSpring;
        tempSpring = newSpring;
    }
    countSprings(springs);
}

void crearMalla(){
    GLfloat x = xInit;
    GLfloat y = yInit;
    GLfloat z = zInit;
    Particle *temp = malla;
    Particle *temp2;
    Particle *temp3;
    for(GLint i = 0;i<=divs;i++){
        Particle *newOne = createParticle(x,y,z);
        temp->next = newOne;
        temp = newOne;
        temp2 = temp;
        // CODIGO PARA CREAR TIRAS
        if(hacerTiras == 1){
            temp3 = newOne;
            for(GLint parts = 1; parts<numParticulasCabello+1;parts++){
                Particle *newOne = createParticle(x,y+distParticulaCabello*parts,z);
                temp3->hair = newOne;
                temp3 = newOne;
                if(parts == 1)
                    temp3->firstHair = 1;
            }
        }
        if(prueba)
            newOne->fixed=1;
        for(GLint j = 1;j<=divs;j++){
            z-=(tamanoLadoMalla/divs);
            Particle *newTwo = createParticle(x,y,z);//18*(j/divs)*2,z);
            temp2->child = newTwo;
            newTwo->father = temp2;
            temp2 = newTwo;
            if(hacerTiras == 1){
                // CODIGO PARA CREAR TIRAS
                temp3 = newTwo;
                for(GLint parts = 1; parts<numParticulasCabello+1;parts++){
                    Particle *newOne = createParticle(x,y+distParticulaCabello*parts,z);
                    temp3->hair = newOne;
                    temp3 = newOne;
                    if(parts == 1)
                         temp3->firstHair = 1;
                }
            }
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
    glMultMatrixf( lights_rotation );
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);


    glLoadIdentity();
    gluLookAt(viewer[0],viewer[1],viewer[2],0,0,0,0,1,0);
    glRotatef(rotationX,1,0,0);
    glRotatef(rotationY,0,1,0);

    glMultMatrixf( view_rotate );

    glScalef( scale, scale, scale );

    if(viewMode == 0)
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if(shadeModel == 0)
        glShadeModel(GL_SMOOTH);
    else
        glShadeModel(GL_FLAT);
    if(mostrarTextura)
        glEnable(GL_TEXTURE_2D);
    else
        glDisable(GL_TEXTURE_2D);

    // AQUI EMPIEZA DIBUJABLE

    glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2] );
    glColor3f(1.0,1.0,1.0);
    dibujarMalla(malla,divs);
    glDisable(GL_TEXTURE_2D);
    if(mostrarParticulas){
        dibujarParticulas(malla, radioParticulas);
    }
    glEnable(GL_TEXTURE_2D);
    dibujarTiras(malla);
    glColor3f(1.0,1.0,1.0);
    glDisable(GL_TEXTURE_2D);
    if(collider){
        glTranslatef( collider_pos[0], collider_pos[1], -collider_pos[2]);
        //glColor3f(0.0,1.0,0.0);
        static GLUquadricObj * cylindro=gluNewQuadric();
        /*GLfloat longit[3];
        for(int ite = 0; ite<3; ite++)
            longit[ite] = posicionEsferasCilindro[ite]-posicionEsferasCilindro[ite+3];*/
        GLfloat longitudCapsula = 6.0f;//sqrt(longit[0]*longit[0]+longit[1]*longit[1]+longit[2]*longit[2]);
        //longitudCapsula = 0;
        //printf("long %f  x:%f   y:%f  z:%f\n",longitudCapsula,collider_pos[0],collider_pos[1],collider_pos[2]);
        gluCylinder(cylindro, radioEsfera,radioEsfera,longitudCapsula,48,24);
        glTranslatef( posicionEsferasCilindro[0],posicionEsferasCilindro[1],-posicionEsferasCilindro[2]);
        static GLUquadricObj * sphere1=gluNewQuadric();
        gluSphere(sphere1, radioEsfera, 48, 24);
        glTranslatef( posicionEsferasCilindro[3],posicionEsferasCilindro[4],-posicionEsferasCilindro[2]);
        static GLUquadricObj * sphere2=gluNewQuadric();
        gluSphere(sphere2, radioEsfera, 48, 24);
    }
    else{
        glTranslatef( collider_pos[0], collider_pos[1], -collider_pos[2]);
        static GLUquadricObj * sphere=gluNewQuadric();
        gluSphere(sphere, radioEsfera, 48, 24);
    }
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
    //initTexture();
    crearMalla();
}
void reset(){
    crearMalla();
    started = 0;
    paused = 1;
    radio->enable();
    tiras->enable();
    spinnerDivs->enable();
    glutPostRedisplay();
}
void disables(){
    crearMalla();
    started = 0;
    paused = 1;
    tiras->enable();
    radio->enable();
    spinnerDivs->enable();
    glutPostRedisplay();
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
            viewer[0]-= 0.3;
            break;
        case 'X':
            viewer[0]+= 0.3;
            break;
        case 'z':
            viewer[2]-= 0.3;
            break;
        case 'Z':
            viewer[2]+= 0.3;
            break;
        case 'y':
            viewer[1]-= 0.3;
            break;
        case 'Y':
            viewer[1]+= 0.3;
            break;
        case 'w':
            viewMode = (viewMode+1)%2;
            break;
        case 's':
            shadeModel = (shadeModel+1)%2;
            break;
        case 't':
            mostrarTextura = !mostrarTextura;
            break;
        case 'p':
            mostrarParticulas = !mostrarParticulas;
            break;
        case 'F':
            fijarSelecionada(malla);
            seleccionada = particulaFijada(malla);
            fijar->set_int_val(seleccionada);
            break;
        case 'f':
            fijarSelecionada(malla);
            seleccionada = particulaFijada(malla);
            fijar->set_int_val(seleccionada);
            break;
        case 32:
            if(!started){
                started = !started;
                spinnerDivs->disable();
                radio->disable();
                tiras->disable();
            }
            paused = !paused;
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
void colisionParticula(Particle *p, GLfloat x, GLfloat y, GLfloat z){

    GLfloat distancia[3];
    distancia[0] = p->pos[0]-x;
    distancia[1] = p->pos[1]-y;
    distancia[2] = p->pos[2]-z;
    GLfloat longitud = Modulo(distancia[0],distancia[1],distancia[2]);
    if(longitud*longitud<radioParticulas*1.1f*1.1f*radioParticulas){
        //printf("longitud: %f  radio: %f\n",longitud,radioEsfera);
        int i = 0;
        normaliza(&distancia[0],&distancia[1],&distancia[2]);
        for(i = 0; i < 3; i++)
            distancia[i]*=(radioParticulas*1.08-longitud);
        p->pos[0]+=distancia[0];
        p->pos[1]+=distancia[1];
        p->pos[2]+=distancia[2];
    }
    colisionPiso(p);
}
void GetOGLPos(int x, int y)
{
    GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;


	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &mposX, &mposY, &mposZ);
	//printf("x:%f   y:%f   z:%f\n", mposX, mposY, mposZ);
}

void mouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        mouseDown = true;
        prevX = x - rotationY;
        prevY = y - rotationX;
    }else{
        mouseDown = false;
    }
    if(button == GLUT_RIGHT_BUTTON && state==GLUT_DOWN){
        //printf("\nx:%i   y:%i \n", mposX, mposY);
        GetOGLPos(x,y);
        colisionMouseMalla(malla, mposX, mposY, mposZ, radioParticulas);
        seleccionada = particulaFijada(malla);
        fijar->set_int_val(seleccionada);
    }
    glutPostRedisplay();
}
void mouseMotion(int x, int y){
    if(mouseDown){
        rotationX = y - prevY;
        rotationY = x - prevX;
        glutPostRedisplay();
    }
}
void control_cb( int control )
{
    if(control == FIJAR_ID){
        fijarSelecionada(malla);
    }
    else if(control == HACER_TIRAS_ID){
        reset();
    }
    else if(control == DIVISIONES_ID){
        reset();
    }
    else if ( control == LIGHT0_ENABLED_ID ) {
        if ( light0_enabled ) {
            glEnable( GL_LIGHT0 );
            light0_spinner->enable();
        }
        else {
            glDisable( GL_LIGHT0 );
            light0_spinner->disable();
        }
    }
    else if ( control == LIGHT0_INTENSITY_ID ) {
        float v[] = {
        light_diffuse[0],  light_diffuse[1],
        light_diffuse[2],  light_diffuse[3] };

        v[0] *= light0_intensity;
        v[1] *= light0_intensity;
        v[2] *= light0_intensity;

        glLightfv(GL_LIGHT0, GL_DIFFUSE, v );
    }
    else if ( control == STARTED_ID )
    {
        if(!started){
            started = !started;
            spinnerDivs->disable();
            paused = !paused;
            radio->disable();
            tiras->disable();
        }
    }
    else if ( control == PAUSED_ID )
    {
        if(started){
            paused=!paused;
        }
    }
    else if ( control == RESET_ID )
    {
        reset();
    }
    else if( control == OBJECT_TYPE_ID){
        collider_pos = {0.0,0.0,0.0};
    }
}
int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(ventanaX,ventanaY);
    glutInitWindowPosition (250,90);
    main_window = glutCreateWindow("Proyecto final");
    glutDisplayFunc(display);
    GLUI_Master.set_glutReshapeFunc(reshape);
    GLUI_Master.set_glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    GLUI_Master.set_glutKeyboardFunc(key);
    //glClearColor(1,1,1,1);
    glColor3f(0,0,0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH);
    glShadeModel(GL_SMOOTH);
    /* Activo la fuente de luz */
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);        // Set Light1 Position
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);         // Set Light1 Ambience
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);         // Set Light1 Diffuse
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
    /****************************************/
    /*         Here's the GLUI code         */
    /****************************************/

    /*** Create the side subwindow ***/
    glui = GLUI_Master.create_glui_subwindow( main_window,
                    GLUI_SUBWINDOW_RIGHT );

    //obj_panel = new GLUI_Rollout(glui, "Properties", false );

    /***** Control for object params *****/

    spinnerDivs =
    new GLUI_Spinner( glui, "Segments:", &divs, DIVISIONES_ID, control_cb);
    //spinner->data_type=GLUI_SPINNER_INT;
    spinnerDivs->set_int_limits( minDiv, maxDiv , GLUI_LIMIT_CLAMP);
    spinnerDivs->set_alignment( GLUI_ALIGN_RIGHT );

    /******** Add some controls for lights ********/

    GLUI_Rollout *roll_lights = new GLUI_Rollout(glui, "Lights", false );

    GLUI_Panel *light0 = new GLUI_Panel( roll_lights, "Light" );

    new GLUI_Checkbox( light0, "Enabled", &light0_enabled,
                     LIGHT0_ENABLED_ID, control_cb );
    light0_spinner =
    new GLUI_Spinner( light0, "Intensity:",
                      &light0_intensity, LIGHT0_INTENSITY_ID,
                      control_cb );
    light0_spinner->set_float_limits( 0.0, 1.0 );
    GLUI_Scrollbar *sb;
    sb = new GLUI_Scrollbar( light0, "Red",GLUI_SCROLL_HORIZONTAL,
                           &light_diffuse[0],LIGHT0_INTENSITY_ID,control_cb);
    sb->set_float_limits(0,1);
    sb = new GLUI_Scrollbar( light0, "Green",GLUI_SCROLL_HORIZONTAL,
                           &light_diffuse[1],LIGHT0_INTENSITY_ID,control_cb);
    sb->set_float_limits(0,1);
    sb = new GLUI_Scrollbar( light0, "Blue",GLUI_SCROLL_HORIZONTAL,
                           &light_diffuse[2],LIGHT0_INTENSITY_ID,control_cb);
    sb->set_float_limits(0,1);

    /*** Add another rollout ***/
    GLUI_Rollout *options = new GLUI_Rollout(glui, "Options", false);
    new GLUI_Checkbox( options, "Mostrar textura", &mostrarTextura);
    new GLUI_Checkbox( options, "Alambre/Wireframe", &viewMode);
    new GLUI_Checkbox( options, "Shading", &shadeModel);
    new GLUI_Checkbox( options, "Mostrar Particulas", &mostrarParticulas);
    fijar = new GLUI_Checkbox( options, "Fijar", &seleccionada, FIJAR_ID, control_cb);
    new GLUI_Checkbox( options, "Hacer Tiras", &hacerTiras, HACER_TIRAS_ID, control_cb);
    tiras= new GLUI_Spinner( options, "Longitud tiras:", &numParticulasCabello , HACER_TIRAS_ID, control_cb);
    tiras->set_int_limits( 1, 6 );

    GLUI_Rollout *physics = new GLUI_Rollout(glui, "Physics", false);

    GLUI_Spinner *magnitudGravedad= new GLUI_Spinner( physics, "Gravity:", &magGravedad);
    magnitudGravedad->set_float_limits( -.05,-.0005 );
    magnitudGravedad->set_alignment( GLUI_ALIGN_RIGHT );
    GLUI_Spinner *gravedadX= new GLUI_Spinner( physics, "x:", &gravedad[0]);
    gravedadX->set_int_limits( -1,1 );
    gravedadX->set_alignment( GLUI_ALIGN_RIGHT );
    GLUI_Spinner *gravedadY= new GLUI_Spinner( physics, "y:", &gravedad[1]);
    gravedadY->set_int_limits( -1, 1 );
    gravedadY->set_alignment( GLUI_ALIGN_RIGHT );
    GLUI_Spinner *gravedadZ= new GLUI_Spinner( physics, "z:", &gravedad[2]);
    gravedadZ->set_int_limits( -1, 1 );
    gravedadZ->set_alignment( GLUI_ALIGN_RIGHT );


    GLUI_Spinner *spinnerK = new GLUI_Spinner( physics, "Ks:", &springConstant);
    spinnerK->set_float_limits( 0.001f, 0.09f , GLUI_LIMIT_CLAMP);
    spinnerK->set_alignment( GLUI_ALIGN_RIGHT );
    GLUI_Spinner *kD = new GLUI_Spinner( physics, "Kd:", &damping);
    kD->set_float_limits( 0.001f, 0.1f , GLUI_LIMIT_CLAMP);
    kD->set_alignment( GLUI_ALIGN_RIGHT );
    GLUI_Spinner *spinnerPartRadius = new GLUI_Spinner( physics, "Particles radius:", &radioParticulas);
    spinnerPartRadius->set_float_limits( 0.09f , 1.0f, GLUI_LIMIT_CLAMP);
    spinnerPartRadius->set_alignment( GLUI_ALIGN_RIGHT );

    obj_panel = new GLUI_Panel( glui, "Object Type" );
    radio = new GLUI_RadioGroup( obj_panel,&collider,OBJECT_TYPE_ID,control_cb );
    new GLUI_RadioButton( radio, "Sphere" );
    new GLUI_RadioButton( radio, "Capsule" );

    /*** Disable/Enable buttons ***/
    new GLUI_Button( glui, "Start", STARTED_ID, control_cb );
    new GLUI_Button( glui, "Pause", PAUSED_ID, control_cb );
    new GLUI_Button( glui, "Reset", RESET_ID, control_cb );

    /****** A 'quit' button *****/
    new GLUI_Button( glui, "Quit", 0,(GLUI_Update_CB)exit );


    /**** Link windows to GLUI, and register idle callback ******/

    glui->set_main_gfx_window( main_window );


    /*** Create the bottom subwindow ***/
    glui2 = GLUI_Master.create_glui_subwindow( main_window,
                                             GLUI_SUBWINDOW_BOTTOM );
    glui2->set_main_gfx_window( main_window );

    GLUI_Rotation *view_rot = new GLUI_Rotation(glui2, "View", view_rotate );
    view_rot->set_spin( 1.0 );
    new GLUI_Column( glui2, false );
    GLUI_Rotation *lights_rot = new GLUI_Rotation(glui2, "Light", lights_rotation );
    lights_rot->set_spin( .82 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_xy =
    new GLUI_Translation(glui2, "Scene XY", GLUI_TRANSLATION_XY, obj_pos );
    trans_xy->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_z =
    new GLUI_Translation( glui2, "Scene Z", GLUI_TRANSLATION_Z, &obj_pos[2] );
    trans_z->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_xyCol =
    new GLUI_Translation(glui2, "Collider XY", GLUI_TRANSLATION_XY, collider_pos );
    trans_xyCol->set_speed( .005 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_zCol =
    new GLUI_Translation( glui2, "Collider Z", GLUI_TRANSLATION_Z, &collider_pos[2] );
    trans_zCol->set_speed( .005 );
    init();
    /**** We register the idle callback with GLUI, *not* with GLUT ****/
    GLUI_Master.set_glutIdleFunc(idle);
    /**** Regular GLUT main loop ****/
    glutMainLoop();
}
