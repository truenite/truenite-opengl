/*
--------------------------------
Gráficas Computacionales
Examen Primer Parical
3 de febrero del 2011

Matrícula: 1162205
Nombre:    Diego Alfonso García Mendiburu
---------------------------------
*/

#include <windows.h>
#include <GL/glut.h>

float rotationX=0.0;
float rotationY=0.0;
float prevX=0.0;
float prevY=0.0;
bool mouseDown=false;
float viewer[]= {0.0, 0.0, 10.0};

float size=0.5;
//AGREGAR LAS VARIABLES QUE CONSIDEREN NECESARIAS AQUÍ

float angle = 0.0;
float speed = .01;
int closed=0;

//Dibujo de una cara del cubo
//la cara está centrada en el origen y acostada en el piso (plano XZ)
//cada lado mide 2*size
void drawSquare(){
	glBegin(GL_QUADS);
    	glVertex3f( size, 0,  size);
    	glVertex3f(-size, 0,  size);
    	glVertex3f(-size, 0, -size);
    	glVertex3f( size, 0, -size);
	glEnd();
}

//Dibujo del cubo por caras y en jerarquía para poderse animar
void drawCube(){
	// Dibujar base en rojo
	glPushMatrix();
        glColor3f(1.0,0.0,0.0);
        drawSquare();
    glPopMatrix();

    //blanco
    glPushMatrix();
        glColor3f(1.0,1.0,1.0);
        glTranslatef(.5,0,0);
        glRotatef(angle,0,0,1);
        glTranslatef(.5,0,0);
        drawSquare();
    glPopMatrix();
    // azul
    glPushMatrix();
        glColor3f(0.0,0.0,1.0);
        glTranslatef(-.5,0,0);
        glRotatef(angle,0,0,-1);
        glTranslatef(-.5,0,0);
        drawSquare();
        // verde

        glColor3f(0.0,1.0,0.0);
        glTranslatef(-.5,0,0);
        glRotatef(angle,0,0,-1);
        glTranslatef(-.5,0,0);
        drawSquare();
    glPopMatrix();

    //morado
    glPushMatrix();
        glColor3f(1.0,0.0,1.0);
        glTranslatef(0,0,.5);
        glRotatef(angle,-1,0,0);
        glTranslatef(0,0,.5);
        drawSquare();
    glPopMatrix();

    //amarillo
    glPushMatrix();
        glColor3f(1.0,1.0,0.0);
        glTranslatef(0,0,-.5);
        glRotatef(angle,1,0,0);
        glTranslatef(0,0,-.5);
        drawSquare();
    glPopMatrix();


}


void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(viewer[0],viewer[1],viewer[2],0,0,0,0,1,0);

    glRotatef(rotationX,1,0,0);
    glRotatef(rotationY,0,1,0);

    drawCube();

    glutSwapBuffers();
}

void reshape(int w, int h){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble)w/(GLdouble)h, 1.0, 100.0);
    glViewport(0,0,w,h);
}

void idle(){
    //AGREGAR CÓDIGO AQUÍ
    angle += speed;
    if(angle > 90)
        speed = 0;
    if(angle < 0)
        speed = 0;
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
    if(button == GLUT_RIGHT_BUTTON && state==GLUT_DOWN){
        if(angle < 0)
            speed = .01;
        else
            speed = -.01;
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

void init(){
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Examen 1 -- Diego Alfonso García Mendiburu 1162205");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle) ;
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(key);
    init();
    glutMainLoop();
}
