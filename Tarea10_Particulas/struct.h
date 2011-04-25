/***************************************************
Materia: Gr�ficas Computacionales
Tarea: 10 Particulas
Fecha: 11 de Abril del 2011
Autor 1: 1162205 Diego Alfonso Garc�a Mendiburu
***************************************************/
#include <windows.h>
#include <GL/glut.h>

typedef struct particle{
    GLfloat mass;       // masa
    GLfloat pos[3];    // posici�n
    GLfloat vel[3]; // velocidad
    GLfloat oldPos[3]; // velocidad
    GLfloat acl[3]; // aceleracion
    GLfloat forces[3]; // acumulador de fuerzas
    GLfloat normal[3];
    GLint fixed;       // part�cula fija?
    GLint selected;    // part�cula seleccionada?
    GLfloat damping;
    struct particle *next;
    struct particle *child;
    struct particle *father;
}Particle;
typedef struct springForce{
    Particle *p1, *p2;      // part�culas unidas por el resorte
    GLfloat k;       // coeficientes del resorte (Hooke y elasticidad)
    GLfloat tension; // tension
    GLfloat r;            // distancia de reposo
    struct springForce *next;
}Spring;
typedef struct plane{
    GLfloat nx, ny, nz; // normal del plano
    GLfloat d;          // distancia al origen
}Plane;
typedef struct particleContact{
    Particle *p;
    Plane *plane;
}ParticleContact;
typedef struct particleSystem{
    struct particle *particles; // arreglo de part�culas
    //Force forces[100];       // arreglo de fuerzas
    Plane *planes;        // arreglo de planos
    ParticleContact *contacts; // lista de contactos por resolver
}ParticleSystem;
typedef struct unaryForce{
    ParticleSystem *ps; // sistema de part�culas sobre el que ejerce fuerza
    GLfloat forces[3];   // magnitud de la fuerza
}UnaryForce;
