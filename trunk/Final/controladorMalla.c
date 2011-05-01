/***************************************************
Materia: Gráficas Computacionales
Tarea: 10 Particulas y Proyecto final
Fecha: 11 de Abril del 2011
Autor 1: 1162205 Diego Alfonso García Mendiburu
***************************************************/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <GL/glut.h>
#include <math.h>
#include "struct.h"

Particle *createParticle(GLfloat x, GLfloat y, GLfloat z){
    Particle *nuevo=(Particle *)malloc(sizeof(Particle));
    if(!nuevo){
        printf("Error al alojar memoria");
        exit(1);
    }
    memset(nuevo,0,sizeof(nuevo));
    nuevo->mass = 10;
    nuevo->pos[0] = x;
    nuevo->pos[1] = y;
    nuevo->pos[2] = z;
    nuevo->vel[0] = 0;
    nuevo->vel[1] = 0;
    nuevo->vel[2] = 0; // velocidad
    nuevo->oldPos[0] = x;
    nuevo->oldPos[1] = y;
    nuevo->oldPos[2] = z; // velocidad
    nuevo->forces[0] = 0;
    nuevo->forces[1] = 0;
    nuevo->forces[2] = 0;
    nuevo->acl[0] = 0.0f;
    nuevo->acl[1] = 0.0f;
    nuevo->acl[2] = 0.0f;
    nuevo->damping = .005;
    nuevo->fixed = 0;
    nuevo->selected = 0;
    nuevo->next = NULL;
    nuevo->child = NULL;
    nuevo->father = NULL;
    nuevo->hair = NULL;
    nuevo->firstHair = 0;
    return nuevo;
}
GLfloat Modulo(GLfloat x, GLfloat y, GLfloat z){
     GLfloat len;

     len = x*x + y*y + z*z;
     return (sqrt(len));
}
Spring *createSpring(Particle *p1, Particle *p2){
    //printf("x1:%f  y1:%f   z1:%f    x2:%f   y2:%f    z2:%f\n",p1->pos[0],p1->pos[1],p1->pos[2],p2->pos[0],p2->pos[1],p2->pos[2]);
    Spring *nuevo=(Spring *)malloc(sizeof(Spring));
    if(!nuevo){
        printf("Error al alojar memoria");
        exit(1);
    }
    memset(nuevo,0,sizeof(nuevo));
    nuevo->p1 = p1;
    nuevo->p2 = p2;
    nuevo->tension = 0;
    GLfloat distancia [3];
    GLint i;
    for(i = 0; i < 3; i++){
        distancia[i] = p1->pos[i]-p2->pos[i];
    }
    nuevo->r = Modulo(distancia[0],distancia[1],distancia[2]);
    return nuevo;
}

GLvoid normaliza(GLfloat *x, GLfloat *y, GLfloat *z){
     GLfloat len;

     len = Modulo(*x, *y, *z);
     len = 1.0/len;
     (*x) *= len;
     (*y) *= len;
     (*z) *= len;
}
GLvoid productoVectorial(GLfloat V1[], GLfloat V2[], GLfloat V3[],
                         GLfloat *NormalX,
                         GLfloat *NormalY,
                         GLfloat *NormalZ){
     GLfloat Qx, Qy, Qz, Px, Py, Pz;

     Px = V2[0]-V1[0];
     Py = V2[1]-V1[1];
     Pz = V2[2]-V1[2];
     Qx = V3[0]-V1[0];
     Qy = V3[1]-V1[1];
     Qz = V3[2]-V1[2];
     *NormalX = Py*Qz - Pz*Qy;
     *NormalY = Pz*Qx - Px*Qz;
     *NormalZ = Px*Qy - Py*Qx;
}
int mismaParticula(Particle *p1, Particle *p2){
    if(p1 == p2)
        return 1;
    return 0;
}
void sumarFuerza(Particle *p, GLfloat cx, GLfloat cy, GLfloat cz){
    if(p->fixed == 0){
        p->acl[0]=p->acl[0]+cx;
        p->acl[1]=p->acl[1]+cy;
        p->acl[2]=p->acl[2]+cz;
    }
}
void timeStep(Particle *p, GLfloat timeStepSize, GLfloat damping){
    if(p->fixed == 0){
        GLfloat nuevo;
        //p->selected=1;
        GLfloat temp[3] = {p->pos[0],p->pos[1],p->pos[2]};
        Particle *te = p;
        //printf("x1: %f, y1:%f,  z1: %f  cambio: %f\n",te->pos[0],te->pos[1],te->pos[2],nuevo);
        GLint i = 0;
        for(i = 0; i < 3; i++){
            //nuevo =
            //nuevo = ((p->pos[i]-p->oldPos[i])*(1.0-p->damping));
            p->pos[i] = p->pos[i] + ((p->pos[i]-p->oldPos[i])*(1.0-damping))+p->acl[i]*timeStepSize;;
            p->oldPos[i] = temp[i];
            p->acl[i] = 0;
            //printf("cambio%i: %f  p->pos[i]:%f  p->oldPos[i]:%f     1.0-p->damping:%f   \n",i,nuevo,p->pos[i],p->oldPos[i],1.0-p->damping);
        }
        //printf("x2: %f, y2:%f,  z2: %f  cambio: %f\n",te->pos[0],te->pos[1],te->pos[2],nuevo);

    }
}
void mover(Particle *p, GLfloat x, GLfloat y, GLfloat z){
    if(p->fixed==0){
    //printf("1: Moviendo x: %f  y: %f  z: %f  \n   ant: x: %f  y: %f  z: %f \n",x,y,z,p->pos[0],p->pos[1],p->pos[2]);
        p->pos[0]=p->pos[0]+x;
        p->pos[1]=p->pos[1]+y;
        p->pos[2]=p->pos[2]+z;
    }
    //printf("2: Ant: x: %f  y: %f  z: %f \n",p->pos[0],p->pos[1],p->pos[2]);
}
void colisionPiso(Particle *p){
    if(p->pos[1]<-9.0)
        p->pos[1]= -9.0;
}
void colisionCapsula(Particle *p,
                     GLfloat radioEsfera,
                     GLfloat x,
                     GLfloat y,
                     GLfloat z,
                     GLfloat longitud){
    //if(p->pos[2]<longitud/2+radioEsfera+z && p->pos[2]<z+longitud/2-radioEsfera){}
    GLfloat P1 = 0.0f;
    GLfloat P2 = 0.0f;
    GLfloat distancia[3];
    distancia[0] = p->pos[0]-x;
    distancia[1] = p->pos[1]-y;
    distancia[2] = p->pos[2]-z;

}
void fijarSelecionada(Particle *malla){
    Particle *temp = malla;
    Particle *tira;
    Particle *tempCol = malla->next;
    int cont = 0;
    for(;tempCol->next;tempCol=tempCol->next){
        temp = tempCol;
        for(;temp->child;temp=temp->child){
            if(temp->selected==1)
                temp->fixed=(temp->fixed+1)%2;
        }
        if(temp->selected==1)
            temp->fixed=(temp->fixed+1)%2;
    }
    temp = tempCol;
    for(;temp->child;temp=temp->child){
        if(temp->selected==1)
            temp->fixed=(temp->fixed+1)%2;
    }
    if(temp->selected==1)
        temp->fixed=(temp->fixed+1)%2;
}
int particulaFijada(Particle *malla){
    Particle *temp = malla;
    Particle *tira;
    Particle *tempCol = malla->next;
    int cont = 0;
    for(;tempCol->next;tempCol=tempCol->next){
        temp = tempCol;
        for(;temp->child;temp=temp->child){
            if(temp->fixed == 1 && temp->selected==1)
                return 1;
        }
        if(temp->fixed == 1 && temp->selected==1)
            return 1;
    }
    temp = tempCol;
    for(;temp->child;temp=temp->child){
        if(temp->fixed == 1 && temp->selected==1)
            return 1;
    }
    if(temp->fixed == 1 && temp->selected==1)
        return 1;
    return 0;
}
void colisionMouseParticula(Particle *p, GLfloat x, GLfloat y, GLfloat z, GLfloat radioParticulas){
    GLfloat distancia[3];
    distancia[0] = x-p->pos[0];
    distancia[1] = y-p->pos[1];
    distancia[2] = z-p->pos[2];
    GLfloat longitud = Modulo(distancia[0],distancia[1],distancia[2]);
    if(longitud*longitud<radioParticulas*1.08f*1.08f*radioParticulas){
        p->selected=(p->selected+1)%2;
    }
}
void colisionMouseMalla(Particle *malla, GLfloat x, GLfloat y, GLfloat z, GLfloat radioParticulas){
    Particle *temp = malla;
    Particle *tira;
    Particle *tempCol = malla->next;
    int cont = 0;
    for(;tempCol->next;tempCol=tempCol->next){
        temp = tempCol;
        for(;temp->child;temp=temp->child){
            temp->selected = 0;
            colisionMouseParticula(temp,x,y,z,radioParticulas);
        }
        temp->selected = 0;
        colisionMouseParticula(temp,x,y,z,radioParticulas);
    }
    temp = tempCol;
    for(;temp->child;temp=temp->child){
        temp->selected = 0;
        colisionMouseParticula(temp,x,y,z,radioParticulas);
    }
    temp->selected = 0;
    colisionMouseParticula(temp,x,y,z,radioParticulas);

}
void colisionEsfera(Particle *p, GLfloat radioEsfera, GLfloat x, GLfloat y, GLfloat z){

    GLfloat distancia[3];
    distancia[0] = p->pos[0]-x;
    distancia[1] = p->pos[1]-y;
    distancia[2] = p->pos[2]-z;
    GLfloat longitud = Modulo(distancia[0],distancia[1],distancia[2]);
    if(longitud*longitud<radioEsfera*1.08f*1.08f*radioEsfera){
        //printf("longitud: %f  radio: %f\n",longitud,radioEsfera);
        int i = 0;
        normaliza(&distancia[0],&distancia[1],&distancia[2]);
        for(i = 0; i < 3; i++)
            distancia[i]*=(radioEsfera*1.08-longitud);
        p->pos[0]+=distancia[0];
        p->pos[1]+=distancia[1];
        p->pos[2]+=distancia[2];
    }
    colisionPiso(p);
}
void sumarNormal(Particle *p, GLfloat cx, GLfloat cy, GLfloat cz){
    p->normal[0]= p->normal[0]+cx;
    p->normal[1]= p->normal[1]+cy;
    p->normal[2]= p->normal[2]+cz;
}
void contrae(Spring *s, GLfloat springConstant){
    Spring *temp = s;
    GLfloat distancia[3];
    GLint i;
    for(i = 0; i < 3; i++){
        distancia [i] = s->p1->pos[i]-s->p2->pos[i];
    }
    GLfloat magnitudDist = Modulo(distancia[0],distancia[1],distancia[2]);

    float extension=magnitudDist-s->r;

    s->tension =  springConstant*extension/s->r;
    //printf("s->tension: %f extension %f , s->r: %f  extension/s-> %f:   magnitudDist: %f\n",s->tension,extension,s->r,extension/s->r,magnitudDist);

    // PARA s->p1
    GLfloat tensionDirection[3];
    if(s->p2->firstHair == 0){
        for(i = 0; i < 3; i++){
            tensionDirection [i] = s->p2->pos[i] - s->p1->pos[i];
        }
        normaliza(&tensionDirection[0],&tensionDirection[1],&tensionDirection[2]);
        //printf("%f   %f   %f\n",tensionDirection[0],tensionDirection[1],tensionDirection[2]);
        sumarFuerza(s->p1,s->tension*tensionDirection[0],s->tension*tensionDirection[1],s->tension*tensionDirection[2]);
    }
    // PARA s->p2
    for(i = 0; i < 3; i++){
        tensionDirection [i] = s->p1->pos[i]-s->p2->pos[i];
    }
    normaliza(&tensionDirection[0],&tensionDirection[1],&tensionDirection[2]);
    sumarFuerza(s->p2,s->tension*tensionDirection[0],s->tension*tensionDirection[1],s->tension*tensionDirection[2]);

}
void dibujarTiras(Particle *malla){
    Particle *temp = malla;
    Particle *tempCol = malla->next;
    Particle *tira = malla;
    for(;tempCol->next;tempCol=tempCol->next){
        temp = tempCol;
        for(;temp->child;temp=temp->child){
            if(temp->hair){
                tira = temp;
                for(;tira->hair;tira=tira->hair){
                    glBegin(GL_LINES);
                        glVertex3f(tira->pos[0],tira->pos[1],tira->pos[2]);
                        glVertex3f(tira->hair->pos[0],tira->hair->pos[1],tira->hair->pos[2]);
                    glEnd();
                }
            }
        }
        if(temp->hair){
            tira = temp;
            for(;tira->hair;tira=tira->hair){
                glBegin(GL_LINES);
                    glVertex3f(tira->pos[0],tira->pos[1],tira->pos[2]);
                    glVertex3f(tira->hair->pos[0],tira->hair->pos[1],tira->hair->pos[2]);
                glEnd();
            }
        }
    }
    temp = tempCol;
    for(;temp->child;temp=temp->child){
        if(temp->hair){
            tira = temp;
            for(;tira->hair;tira=tira->hair){
                glBegin(GL_LINES);
                    glVertex3f(tira->pos[0],tira->pos[1],tira->pos[2]);
                    glVertex3f(tira->hair->pos[0],tira->hair->pos[1],tira->hair->pos[2]);
                glEnd();
            }
        }
    }
    if(temp->hair){
        tira = temp;
        for(;tira->hair;tira=tira->hair){
            glBegin(GL_LINES);
                glVertex3f(tira->pos[0],tira->pos[1],tira->pos[2]);
                glVertex3f(tira->hair->pos[0],tira->hair->pos[1],tira->hair->pos[2]);
            glEnd();
        }
    }

}
void sumarFuerzaMalla(Particle *malla, GLfloat cx, GLfloat cy, GLfloat cz){
    Particle *temp = malla;
    Particle *tira;
    Particle *tempCol = malla->next;
    int cont = 0;
    for(;tempCol->next;tempCol=tempCol->next){
        temp = tempCol;
        for(;temp->child;temp=temp->child){
            sumarFuerza(temp,cx,cy,cz);
            if(temp->hair){
                tira = temp->hair;
                for(;tira->hair;tira=tira->hair){
                    sumarFuerza(tira,cx,cy,cz);
                }
                sumarFuerza(tira,cx,cy,cz);
            }
        }
        sumarFuerza(temp,cx,cy,cz);;
        if(temp->hair){
            tira = temp->hair;
            int i= 0;
            for(;tira->hair;tira=tira->hair){
                sumarFuerza(tira,cx,cy,cz);
            }
            sumarFuerza(tira,cx,cy,cz);
        }
    }
    temp = tempCol;
    for(;temp->child;temp=temp->child){
        sumarFuerza(temp,cx,cy,cz);
        if(temp->hair){
            tira = temp->hair;
            for(;tira->hair;tira=tira->hair){
                sumarFuerza(tira,cx,cy,cz);
            }
            sumarFuerza(tira,cx,cy,cz);
        }
    }
    sumarFuerza(temp,cx,cy,cz);
    if(temp->hair){
        tira = temp->hair;
        for(;tira->hair;tira=tira->hair){
            sumarFuerza(tira,cx,cy,cz);
        }
        sumarFuerza(tira,cx,cy,cz);
    }
}
void drawSprings(Spring *s){
    Spring *temp = s->next;
    for(;temp->next;temp=temp->next){
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
            glVertex3f(temp->p1->pos[0],temp->p1->pos[1],temp->p1->pos[2]);
            glVertex3f(temp->p2->pos[0],temp->p2->pos[1],temp->p2->pos[2]);
        glEnd();
        //printf("x1:%f  y1:%f   z1:%f    x2:%f   y2:%f    z2:%f\n",temp->p1->pos[0],temp->p1->pos[1],temp->p1->pos[2],temp->p2->pos[0],temp->p2->pos[1],temp->p2->pos[2]);
    }
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
        glVertex3f(temp->p1->pos[0],temp->p1->pos[1],temp->p1->pos[2]);
        glVertex3f(temp->p2->pos[0],temp->p2->pos[1],temp->p2->pos[2]);
    glEnd();
}
void countSprings(Spring *s){
    Spring *temp = s;
    int i=0;
    for(;temp->next;temp=temp->next,i++)
        ;//printf("%i x1:%f  y1:%f   z1:%f    x2:%f   y2:%f    z2:%f\n",i,temp->p1->pos[0],temp->p1->pos[1],temp->p1->pos[2],temp->p2->pos[0],temp->p2->pos[1],temp->p2->pos[2]);
    //printf("hay %i springs\n",i+1);
}
void dibujarMalla(Particle *malla, GLfloat divs){
    Particle *temp = malla;
    Particle *tempCol = malla->next;
    int i = 0;
    for(;tempCol->next;tempCol=tempCol->next){
        temp = tempCol;
        for(;temp->child;temp=temp->child){
            for(i = 0;i<3;i++)
                temp->normal[i]=0;
        }
        for(i = 0;i<3;i++)
            temp->normal[i]=0;
    }
    temp = tempCol;
    for(;temp->child;temp=temp->child){
        for(i = 0;i<3;i++)
            temp->normal[i]=0;
    }
    for(i = 0;i<3;i++)
        temp->normal[i]=0;

    GLfloat posActualX, posActualY;
 	posActualX = posActualY = 0.0;
 	tempCol = malla->next;
    for(;tempCol->next;tempCol=tempCol->next){
        temp = tempCol;
        for(;temp->child;temp=temp->child){
            GLfloat NormalX, NormalY, NormalZ;
            GLfloat V1[3];
            GLfloat V2[3];
            GLfloat V3[3];
            GLfloat V4[3];
            V1[0] = temp->pos[0];
            V1[1] = temp->pos[1];
            V1[2] = temp->pos[2];
            V2[0] = temp->next->pos[0];
            V2[1] = temp->next->pos[1];
            V2[2] = temp->next->pos[2];
            V3[0] = temp->child->pos[0];
            V3[1] = temp->child->pos[1];
            V3[2] = temp->child->pos[2];
            V4[0] = temp->child->next->pos[0];
            V4[1] = temp->child->next->pos[1];
            V4[2] = temp->child->next->pos[2];

            productoVectorial(V1, V3, V2, &NormalX, &NormalY, &NormalZ);
            normaliza(&NormalX, &NormalY, &NormalZ);
            temp->normal[0]+=NormalX;
            temp->normal[1]+=NormalY;
            temp->normal[2]+=NormalZ;
            temp->next->normal[0]+=NormalX;
            temp->next->normal[1]+=NormalY;
            temp->next->normal[2]+=NormalZ;
            temp->child->normal[0]+=NormalX;
            temp->child->normal[1]+=NormalY;
            temp->child->normal[2]+=NormalZ;


            productoVectorial(V2, V3, V4, &NormalX, &NormalY, &NormalZ);
            normaliza(&NormalX, &NormalY, &NormalZ);
            temp->next->normal[0]+=NormalX;
            temp->next->normal[1]+=NormalY;
            temp->next->normal[2]+=NormalZ;
            temp->child->normal[0]+=NormalX;
            temp->child->normal[1]+=NormalY;
            temp->child->normal[2]+=NormalZ;
            temp->child->next->normal[0]+=NormalX;
            temp->child->next->normal[1]+=NormalY;
            temp->child->next->normal[2]+=NormalZ;
        }
        posActualY = 0;
 	    posActualX += 1/divs;
    }
    tempCol = malla->next;
    for(;tempCol->next;tempCol=tempCol->next){
        temp = tempCol;
        for(;temp->child;temp=temp->child){
            GLfloat NormalX, NormalY, NormalZ;
            glBegin(GL_TRIANGLES);
                normaliza(&temp->normal[0],&temp->normal[1],&temp->normal[2]);
                glNormal3fv(temp->normal);
                glTexCoord2f(posActualX, posActualY);
                glVertex3fv(temp->pos);

                normaliza(&temp->next->normal[0],&temp->next->normal[1],&temp->next->normal[2]);
                glNormal3fv(temp->next->normal);
                posActualX += 1/divs;
                glTexCoord2f(posActualX, posActualY);
                glVertex3fv(temp->next->pos);

                normaliza(&temp->child->normal[0],&temp->child->normal[1],&temp->child->normal[2]);
                glNormal3fv(temp->child->normal);
                posActualX -= 1/divs;
                posActualY += 1/divs;
                glTexCoord2f(posActualX, posActualY);
                glVertex3fv(temp->child->pos);
                //glVertex3f(temp->next->child->x,temp->next->child->y,temp->next->child->z);
            glEnd();

            glBegin(GL_TRIANGLES);
                //glVertex3f(temp->x,temp->y,temp->z);
                normaliza(&temp->next->normal[0],&temp->next->normal[1],&temp->next->normal[2]);
                glNormal3fv(temp->next->normal);
                posActualY -= 1/divs;
                posActualX += 1/divs;
                glTexCoord2f(posActualX, posActualY);
                glVertex3fv(temp->next->pos);

                normaliza(&temp->child->next->normal[0],&temp->child->next->normal[1],&temp->child->next->normal[2]);
                glNormal3fv(temp->child->next->normal);
                posActualY += 1/divs;
                glTexCoord2f(posActualX, posActualY);
                glVertex3fv(temp->child->next->pos);

                normaliza(&temp->child->normal[0],&temp->child->normal[1],&temp->child->normal[2]);
                glNormal3fv(temp->child->normal);
                posActualX -= 1/divs;
                glTexCoord2f(posActualX, posActualY);
                glVertex3fv(temp->child->pos);
                //posActualY -= 1/(GLfloat)divs;
            glEnd();

        }
        posActualY = 0;
 	    posActualX += 1/divs;
    }
}
void dibujarParticulas(Particle *malla, GLfloat radioParticulas){
    Particle *temp = malla;
    Particle *tempCol = malla->next;
    for(;tempCol->next;tempCol=tempCol->next){
        temp = tempCol;
        for(;temp->child;temp=temp->child){
            glPushMatrix();
                glColor3f(0.0f, 1.0f, 0.0f);
                if(temp->fixed == 1)
                    glColor3f(1.0f, 0.0f, 0.0f);
                if(temp->selected == 1)
                    glColor3f(1.0f, 0.0f, 1.0f);
                glTranslatef(temp->pos[0],temp->pos[1],temp->pos[2]);
                glutSolidSphere(radioParticulas,20,20);
            glPopMatrix();
            glColor3f(0.0f, 0.0f, 0.0f);
        }
        glPushMatrix();
            glColor3f(0.0f, 1.0f, 0.0f);
            if(temp->fixed == 1)
                glColor3f(1.0f, 0.0f, 0.0f);
            if(temp->selected == 1)
                glColor3f(1.0f, 0.0f, 1.0f);
            glTranslatef(temp->pos[0],temp->pos[1],temp->pos[2]);
            glutSolidSphere(radioParticulas,20,20);
        glPopMatrix();
        glColor3f(0.0f, 0.0f, 0.0f);
    }
    temp = tempCol;
    for(;temp->child;temp=temp->child){
        glPushMatrix();
            glColor3f(0.0f, 1.0f, 0.0f);
            if(temp->fixed == 1)
                glColor3f(1.0f, 0.0f, 0.0f);
            if(temp->selected == 1)
                glColor3f(1.0f, 0.0f, 1.0f);
            glTranslatef(temp->pos[0],temp->pos[1],temp->pos[2]);
            glutSolidSphere(radioParticulas,20,20);
        glPopMatrix();
        glColor3f(0.0f, 0.0f, 0.0f);
    }
    glPushMatrix();
        glColor3f(0.0f, 1.0f, 0.0f);
        if(temp->fixed == 1)
            glColor3f(1.0f, 0.0f, 0.0f);
        if(temp->selected == 1)
            glColor3f(1.0f, 0.0f, 1.0f);
        glTranslatef(temp->pos[0],temp->pos[1],temp->pos[2]);
        glutSolidSphere(radioParticulas,20,20);
    glPopMatrix();
}
