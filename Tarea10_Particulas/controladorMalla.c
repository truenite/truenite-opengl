#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <GL/glut.h>

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
    nuevo->forces[0] = 0;
    nuevo->forces[1] = 0;
    nuevo->forces[2] = 0;
    nuevo->acl[0] = 0;
    nuevo->acl[1] = 0;
    nuevo->acl[2] = 0;
    nuevo->fixed = 0;
    nuevo->selected = 0;
    nuevo->next = NULL;
    nuevo->child = NULL;
    return nuevo;
}
Spring *createSpringForce(Particle *p1, Particle *p2, GLfloat Ks, GLfloat Kd, GLfloat r){
    Spring *nuevo=(Spring *)malloc(sizeof(Spring));
    if(!nuevo){
        printf("Error al alojar memoria");
        exit(1);
    }
    memset(nuevo,0,sizeof(nuevo));
    nuevo->p1 = p1;
    nuevo->p2 = p2;
    nuevo->Ks = Ks;
    nuevo->Kd = Kd;
    nuevo->r = r;
    return nuevo;
}
ParticleSystem *createParticleSystem(){
    ParticleSystem *nuevo=(ParticleSystem *)malloc(sizeof(ParticleSystem));
    if(!nuevo){
        printf("Error al alojar memoria");
        exit(1);
    }
    memset(nuevo,0,sizeof(nuevo));
    Particle particles[100] = {};
    Plane planes[10] = {};
    ParticleContact contacts[100] = {};
    nuevo->particles = particles;
    nuevo->planes = planes;
    nuevo->contacts = contacts;
    return nuevo;
}

GLfloat Modulo(GLfloat x, GLfloat y, GLfloat z){
     GLfloat len;

     len = x*x + y*y + z*z;
     return (sqrt(len));
}

GLvoid Normaliza(GLfloat *x, GLfloat *y, GLfloat *z){
     GLfloat len;

     len = Modulo(*x, *y, *z);
     len = 1.0/len;
     (*x) *= len;
     (*y) *= len;
     (*z) *= len;
}

GLvoid ProductoVectorial(GLfloat V1[], GLfloat V2[], GLfloat V3[],
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

void dibujarMalla(Particle *malla, GLfloat divs){
    Particle *temp = malla;
    Particle *tempCol = malla->next;
    GLfloat posActualX, posActualY;
 	posActualX = posActualY = 0.0;

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
            V4[0] = temp->next->child->pos[0];
            V4[1] = temp->next->child->pos[1];
            V4[2] = temp->next->child->pos[2];

            ProductoVectorial(V1, V3, V2, &NormalX, &NormalY, &NormalZ);
            Normaliza(&NormalX, &NormalY, &NormalZ);

            glBegin(GL_TRIANGLES);
                glNormal3f(NormalX,NormalY,NormalZ);
                glTexCoord2f(posActualX, posActualY);
                glVertex3f(V1[0],V1[1],V1[2]);

                posActualX += 1/divs;
                glTexCoord2f(posActualX, posActualY);
                glVertex3f(V2[0],V2[1],V2[2]);

                posActualX -= 1/divs;
                posActualY += 1/divs;
                glTexCoord2f(posActualX, posActualY);
                glVertex3f(V3[0],V3[1],V3[2]);
                //glVertex3f(temp->next->child->x,temp->next->child->y,temp->next->child->z);
            glEnd();

            ProductoVectorial(V2, V3, V4, &NormalX, &NormalY, &NormalZ);
            Normaliza(&NormalX, &NormalY, &NormalZ);

            glBegin(GL_TRIANGLES);
                //glVertex3f(temp->x,temp->y,temp->z);
                glNormal3f(NormalX,NormalY,NormalZ);

                posActualY -= 1/divs;
                posActualX += 1/divs;
                glTexCoord2f(posActualX, posActualY);
                glVertex3f(V2[0],V2[1],V2[2]);

                posActualY += 1/divs;
                glTexCoord2f(posActualX, posActualY);
                glVertex3f(V4[0],V4[1],V4[2]);

                posActualX -= 1/divs;
                glTexCoord2f(posActualX, posActualY);
                glVertex3f(V3[0],V3[1],V3[2]);
                //posActualY -= 1/(GLfloat)divs;
            glEnd();

        }
        posActualY = 0;
 	    posActualX += 1/divs;
    }
}


void dibujarParticulas(Particle *malla){
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
                glutSolidSphere(.2,20,20);
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
            glutSolidSphere(.2,20,20);
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
            glutSolidSphere(.2,20,20);
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
        glutSolidSphere(.2,20,20);
    glPopMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
}

