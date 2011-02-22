#include <windows.h>
#include <GL/glut.h>

float rotationX=0.0;
float rotationY=0.0;
float prevX=0.0;
float prevY=0.0;
bool mouseDown=false;
float viewer[]= {0.0, 0.0, 7.0};
int displayMode=1;

typedef struct treenode{
    GLfloat m[16];
    void (*f)();
    struct treenode *sibling;
    struct treenode *child;
}treenode;

treenode torso;
// Declarar todas!

void paintTorso(){
    glPushMatrix();
    glScalef(1.0,1.5,1.0);
    // Display mode
    if(displayMode==1)
        glutWireCube(1);
    else
        glutSolidCube(1);
    glPopMatrix();
}

void init(){
	glColor3f(1.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	// Torso
	glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, torso.m);
    torso.f = paintTorso;
    torso.sibling = NULL;
    torso.child = NULL;

}

void traverse(treenode *node){
    glPushMatrix();
        glMultMatrixf(node->m);
        node->f();
        if(node->child != NULL)
            traverse(node->child);
    glPopMatrix();

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

void specialKey(int c, int x, int y){
	if(c == GLUT_KEY_RIGHT) {

	}

    if(c == GLUT_KEY_UP) {

	}
	if(c == GLUT_KEY_DOWN) {

	}

   glutPostRedisplay();
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
