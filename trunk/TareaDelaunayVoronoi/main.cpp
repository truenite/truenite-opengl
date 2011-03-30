/***************************************************
Materia: Gráficas Computacionales
Tarea: 7 Delaunay y Voronoi
Fecha: 12 de marzo del 2011
Autor 1: 1162205 Diego Alfonso García Mendiburu
***************************************************/

#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "triangulo.c"
#include "vertice.c"
#include "circulo.c"

bool mouseDown=false;
GLint agregados = 0;
GLfloat pos[] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
GLint ventanaX = 500;
GLint ventanaY = 500;
GLint puntoSeleccionado = -1;
GLint dibujarTriangulo = 1;
GLint dibujarCirculo = 1;
GLint dibujarVertices = 1;
GLint dibujarVerticesVoronoi = 1;
GLint dibujarVoronoi = 1;
Triangulo *headT;
Vertice *headV;
GLfloat radioVertice = .02;

Circulo *calcularCircuncentro(Vertice *v1, Vertice *v2, Vertice *v3){

    GLfloat ABx = (v1->x + v2->x)/2;
    GLfloat ABy = (v1->y + v2->y)/2;
    GLfloat BCx = (v3->x + v2->x)/2;
    GLfloat BCy = (v3->y + v2->y)/2;
    GLfloat ABm = (v1->y - v2->y)/(v1->x - v2->x);
    GLfloat ABmPrime = 0;
    if(ABm != 0)
        ABmPrime = -1/ABm;
    else ABmPrime = 0;
    GLfloat BCm = (v3->y - v2->y)/(v3->x - v2->x);
    GLfloat BCmPrime = 0;
    if(BCm != 0)
        BCmPrime = -1/BCm;
    else BCmPrime = 0;

    GLfloat circunX = ((-BCx * BCmPrime)+BCy+(ABx*ABmPrime)-ABy)/(ABmPrime-BCmPrime);
    GLfloat circunY = (circunX*ABmPrime)-(ABx*ABmPrime)+(ABy);
    glColor3f(.9f, .9f, .9f);
    GLfloat radio = calcularRadio(v1->x,v1->y,circunX,circunY);

    Vertice *temp = headV;
    Triangulo *triangulo = crearTriangulo(v1->x,v1->y,v2->x,v2->y,v3->x,v3->y,circunX,circunY);

    for(;temp->next;temp=temp->next){
        if(esVertice(temp->x,temp->y,triangulo) == 0 && estaDentroP(temp->x,temp->y,circunX,circunY,radio) == 1){
            free(triangulo);
            return NULL;
        }
    }
    if(esVertice(temp->x,temp->y,triangulo) == 0 && estaDentroP(temp->x,temp->y,circunX,circunY,radio) == 1){
        free(triangulo);
        return NULL;
    }
    if(headT == NULL){
        headT = triangulo;
    }else{
        Triangulo *temp2 = headT;
        GLint existe = 0;
        for(;temp2->next;temp2=temp2->next)
            if(esIgualQue(triangulo,temp2))
                existe = 1;
        if(esIgualQue(triangulo,temp2))
            existe = 1;
        if(existe == 0)
            temp2->next = triangulo;
    }
    Circulo *circ = crearCirculo(circunX,circunY,radio);
    /*if(dibujarCirculo == 1){
        glCircle3f(circunX,circunY,radio);
    }*/
    return circ;
}

void drawTriangulo(Vertice *v1, Vertice *v2, Vertice *v3){
    if(dibujarTriangulo == 1){
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
            glVertex2f(v1->x,v1->y);
            glVertex2f(v2->x,v2->y);
            glVertex2f(v2->x,v2->y);
            glVertex2f(v3->x,v3->y);
            glVertex2f(v3->x,v3->y);
            glVertex2f(v1->x,v1->y);
        glEnd();
    }

}

void drawTriangulos(){
    if(headT != NULL && dibujarTriangulo == 1){
        Triangulo *temp = headT;
        for(;temp->next;temp=temp->next)
            drawTriangulo(temp->x1,temp->y1,temp->x2,temp->y2,temp->x3,temp->y3);
        drawTriangulo(temp->x1,temp->y1,temp->x2,temp->y2,temp->x3,temp->y3);
    }
}
void drawVertices(){
    if(dibujarVertices == 1){
        GLint i = 0;
        Vertice *temp=headV;
        if(temp!=NULL){
            for(;temp->next;temp=temp->next){
                if(i == puntoSeleccionado)
                    glColor3f(0.0f, 1.0f, 0.0f);
                else
                    glColor3f(0.0f, 0.0f, 0.0f);
                glPushMatrix();
                    glTranslatef(temp->x,temp->y,0);
                    glutSolidSphere(radioVertice,20,20);
                glPopMatrix();
                i++;
            }
            i++;
            if(i == puntoSeleccionado)
                glColor3f(0.0f, 1.0f, 0.0f);
            else
                glColor3f(0.0f, 0.0f, 0.0f);
            glPushMatrix();
                glTranslatef(temp->x,temp->y,0);
                glutSolidSphere(radioVertice,20,20);
            glPopMatrix();
            i++;
        }
        glColor3f(0.0f, 0.0f, 0.0f);
    }
}
void calcularTriangulos(){
    Vertice *temp = headV;
    Vertice *temp2 = temp->next;
    Vertice *temp3 = temp2->next;
    while(temp->next != NULL){
        temp2 = temp->next;
        while(temp2->next != NULL){
            temp3 = temp2->next;
            while(temp3->next != NULL){
                calcularCircuncentro(temp,temp2,temp3);
                Circulo *circulo = calcularCircuncentro(temp,temp2,temp3);
                if(circulo != NULL){
                    if(dibujarCirculo == 1)
                        glCircle3f(circulo->x,circulo->y,circulo->radio);
                }
                temp3=temp3->next;
            }
            calcularCircuncentro(temp,temp2,temp3);
            Circulo *circulo = calcularCircuncentro(temp,temp2,temp3);
            if(circulo != NULL){
                if(dibujarCirculo == 1)
                    glCircle3f(circulo->x,circulo->y,circulo->radio);
            }
            temp2 = temp2->next;
        }
        calcularCircuncentro(temp,temp2,temp3);
        Circulo *circulo = calcularCircuncentro(temp,temp2,temp3);
        if(circulo != NULL){
            if(dibujarCirculo == 1)
                glCircle3f(circulo->x,circulo->y,circulo->radio);
        }
        temp=temp->next;
    }
}

void drawVoronoiVertex(){
    if(dibujarVerticesVoronoi == 1 && headT != NULL){
        glColor3f(1.0f, 0.0f, 0.0f);
        Triangulo *temp = headT;
        for(;temp->next;temp=temp->next){
            glPushMatrix();
                glTranslatef(temp->circunX,temp->circunY,0);
                glutSolidSphere(radioVertice,20,20);
            glPopMatrix();
        }
        glPushMatrix();
            glTranslatef(temp->circunX,temp->circunY,0);
            glutSolidSphere(radioVertice,20,20);
        glPopMatrix();
        glColor3f(0.0f, 0.0f, 0.0f);
    }
}

void lineasInfinito(GLint vecinos[], Triangulo *temp){
    for(int i = 0; i <3; i++){
        if(vecinos[i]==0){
            GLfloat x = 0;//(temp->x1+temp->x2)/2;
            GLfloat y = 0;//(temp->y1+temp->y2)/2;
            GLfloat x3erPunto = 0;
            GLfloat y3erPunto = 0;
            GLfloat limX = 0;
            GLfloat limY = 0;
            switch(i){
                case 0: // de 1 a 2
                    x = (temp->x1+temp->x2)/2;
                    y = (temp->y1+temp->y2)/2;
                    if(temp->circunX > temp->x3)
                        limX = 1;
                    else
                        limX = -1;
                    if(temp->circunY > temp->y3)
                        limY = 1;
                    else
                        if(temp->circunY < temp->y3)
                            limY = -1;
                        else limY = temp->circunY;
                    break;
                case 1: // de 2 a 3
                    x = (temp->x2+temp->x3)/2;
                    y = (temp->y2+temp->y3)/2;
                    x3erPunto = temp->y1;
                    y3erPunto = temp->y1;
                    if(temp->circunX > temp->x1)
                        limX = 1;
                    else
                        limX = -1;
                    if(temp->circunY > temp->y1)
                        limY = 1;
                    else
                        if(temp->circunY < temp->y1)
                            limY = -1;
                        else limY = temp->circunY;
                    break;
                case 2: // de 1 a 3
                    x = (temp->x1+temp->x3)/2;
                    y = (temp->y1+temp->y3)/2;
                    x3erPunto = temp->y2;
                    y3erPunto = temp->y2;
                    if(temp->circunX > temp->x2)
                        limX = 1;
                    else
                        limX = -1;
                    if(temp->circunY > temp->y2)
                        limY = 1;
                    else
                        if(temp->circunY < temp->y2)
                            limY = -1;
                        else limY = temp->circunY;
                    break;
                default:
                    break;
            }
            GLfloat m = (temp->circunY-y)/(temp->circunX-x);
            GLfloat b = y - (m*x);
            GLfloat x1 = 0;
            GLfloat y1 = 0;
            GLfloat x2 = 0;
            GLfloat y2 = 0;

            y1 = (m*limX)+b;
            x1 = (limY-b)/m;

            glColor3f(1.0f, 1.0f, 0.0f);
            //printf("x %f  y %f   cirx %f  ciry %f\n",x,y,temp->circunX,temp->circunY);
            glBegin(GL_LINES);
                glVertex2f(temp->circunX,temp->circunY);
                glVertex2f(x1,limY);
            glEnd();
        }
    }

}

void drawVoronoi(){
    if(dibujarVoronoi==1 && headT != NULL){
        Triangulo *temp = headT;
        Triangulo *temp2;
        GLint j = 0;
        GLint numVecinos = 0;
        GLint vecinos[] = {0,0,0};
        for(;temp->next;temp=temp->next,j++){
            vecinos = {0,0,0};
            numVecinos = 0;
            temp2 = headT;
            GLint k = 0;
            for(;temp2->next;temp2=temp2->next,k++){
                if(esVecino(temp,temp2) > 0){
                    vecinos[esVecinoConArista(temp,temp2)]=1;
                    numVecinos++;
                    glColor3f(1.0f, 0.0f, 0.0f);
                    glBegin(GL_LINES);
                        glVertex2f(temp->circunX,temp->circunY);
                        glVertex2f(temp2->circunX,temp2->circunY);
                    glEnd();
                }
            }
            if(esVecino(temp,temp2) > 0 ){
                vecinos[esVecinoConArista(temp,temp2)]=1;
                numVecinos++;
                glColor3f(1.0f, 0.0f, 0.0f);
                glBegin(GL_LINES);
                    glVertex2f(temp->circunX,temp->circunY);
                    glVertex2f(temp2->circunX,temp2->circunY);
                glEnd();
            }
            if(numVecinos >0 && numVecinos <3){
                lineasInfinito(vecinos,temp);
            }
        }


        vecinos = {0,0,0};
        numVecinos = 0;
        temp2 = headT;
        GLint k = 0;
        for(;temp2->next;temp2=temp2->next,k++){
            if(esVecino(temp,temp2) > 0){
                vecinos[esVecinoConArista(temp,temp2)]=1;
                numVecinos++;
                glColor3f(1.0f, 0.0f, 0.0f);
                glBegin(GL_LINES);
                    glVertex2f(temp->circunX,temp->circunY);
                    glVertex2f(temp2->circunX,temp2->circunY);
                glEnd();
            }
        }
        if(esVecino(temp,temp2) > 0 ){
            vecinos[esVecinoConArista(temp,temp2)]=1;
            numVecinos++;
            glColor3f(1.0f, 0.0f, 0.0f);
            glBegin(GL_LINES);
                glVertex2f(temp->circunX,temp->circunY);
                glVertex2f(temp2->circunX,temp2->circunY);
            glEnd();
        }
        if(numVecinos >0 && numVecinos <3){
            lineasInfinito(vecinos,temp);
        }







        glColor3f(0.0f, 0.0f, 0.0f);
    }
}

void display(){
    glColor3f(0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    drawVertices();

    if(agregados > 2){
        calcularTriangulos();
        drawTriangulos();
        drawVoronoiVertex();
        drawVoronoi();
    }
    glFlush();
}

void initValores(){
    glClearColor(1,1,1,1);
    glColor3f(0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-250,250,-250,250);
    headV = NULL;
    headT = NULL;
}

void vaciarListaTriangulos(){
    Triangulo *temp=headT;
    Triangulo *del=headT;
    if(temp){
      if(!temp->next){
             free(del);
      }
      else{
          temp=temp->next;
          for(;temp;temp=temp->next){
             free(del);
             del=temp;
          }
          free(del);
      }
    }
    headT=NULL;

}

void mouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        vaciarListaTriangulos();
        mouseDown = true;
        GLint i = 0;
        GLfloat xMouse = (float)(ventanaX/2 - x)/ventanaX*-2;
        GLfloat yMouse = (float)(ventanaY/2 - y)/ventanaY*2;
        Vertice *temp=headV;
        if(temp==NULL){
            headV=crearVertice((float)(ventanaX/2 - x)/ventanaX*-2,(float)(ventanaY/2 - y)/ventanaY*2);
            agregados++;
        }
        else{
            for(;temp->next;temp=temp->next){
                if(estaDentroP(xMouse,yMouse,temp->x,temp->y,radioVertice) == 1)
                    puntoSeleccionado = i;
                i++;
            }
            i++;
            if(estaDentroP(xMouse,yMouse,temp->x,temp->y,radioVertice) == 1)
                puntoSeleccionado = i;
            if(puntoSeleccionado == -1){
                Vertice *nuevo = crearVertice((float)(ventanaX/2 - x)/ventanaX*-2,(float)(ventanaY/2 - y)/ventanaY*2);
                temp->next=nuevo;
                agregados++;
            }
        }
    }else{
        mouseDown = false;
        puntoSeleccionado = -1;
    }
    glutPostRedisplay();
}

void mouseMotion(int x, int y){
    Vertice *temp=headV;
    if(mouseDown && puntoSeleccionado >= 0 && temp!=NULL){
        vaciarListaTriangulos();
        for(int i = 0; i < puntoSeleccionado; i++)
            if(temp->next != NULL)
                temp=temp->next;
        temp->x = (float)(ventanaX/2 - x)/ventanaX*-2;
        temp->y = (float)(ventanaY/2 - y)/ventanaY*2;
        glutPostRedisplay();
    }
}


void reshape(int w, int h){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    ventanaX = w;
    ventanaY = h;
    glViewport(0,0,w,h);
}

void vaciarListas(){
    vaciarListaTriangulos();
    Vertice *temp2= headV;
    Vertice *del2 = headV;
    if(temp2){
      if(!temp2->next){
             free(del2);
      }
      else{
          temp2=temp2->next;
          for(;temp2;temp2=temp2->next){
             free(del2);
             del2=temp2;
          }
          free(del2);
      }
    }
    headV=NULL;
    agregados = 0;
}

void key(unsigned char key, int x, int y){
    if(key == 'c') dibujarCirculo = (dibujarCirculo+1)%2;
    if(key == 'C') dibujarCirculo = (dibujarCirculo+1)%2;
    if(key == 't') dibujarTriangulo = (dibujarTriangulo+1)%2;
    if(key == 'T') dibujarTriangulo = (dibujarTriangulo+1)%2;
    if(key == 'p') dibujarVertices = (dibujarVertices+1)%2;
    if(key == 'P') dibujarVertices = (dibujarVertices+1)%2;
    if(key == 'v') dibujarVoronoi = (dibujarVoronoi+1)%2;
    if(key == 'V') dibujarVoronoi = (dibujarVoronoi+1)%2;
    if(key == 'o') dibujarVerticesVoronoi = (dibujarVerticesVoronoi+1)%2;
    if(key == 'O') dibujarVerticesVoronoi = (dibujarVerticesVoronoi+1)%2;
    if(key == 'i') {agregados = 0; vaciarListas();}
    if(key == 'I') {agregados = 0; vaciarListas();}
    glutPostRedisplay();
}

main(){
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(ventanaX,ventanaY);
    glutInitWindowPosition(0,0);

    glutCreateWindow("Parte1");

    glutDisplayFunc(display);

    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);

    glutKeyboardFunc(key);
    initValores();

    glutMainLoop();
}
