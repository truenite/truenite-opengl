#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

int angle = 0;

void display (void)
{
  glClear (GL_COLOR_BUFFER_BIT);
  glColor3f(0.4f, 1.0f, 1.0f);
  glPushMatrix();
    glRotatef(-20,1,0,0);
    glRotatef(angle,0,1,0);
    glutWireCube(1);
  glPopMatrix();
  glutSwapBuffers();
}

void animate1 (void)
{
  for(long i=0; i<=1000000; i++);  //truco sucio para que no vaya tan rápido
  angle += 1;
  glutPostRedisplay();
}

void animate2 (int incremento)
{
  angle += incremento;
  glutPostRedisplay();
  glutTimerFunc(10,animate2,1);
}

int main (int argc, char **argv)
{
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE);
  glutCreateWindow ("Cubo rotador");
  glutDisplayFunc(display);
  glutIdleFunc(animate1);
  //glutTimerFunc(10,animate2,1);
  glutMainLoop ();
}
