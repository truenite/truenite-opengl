/***************************************************
Materia: Gráficas Computacionales
Tarea: 10 Particulas y Proyecto final
Fecha: 11 de Abril del 2011
Autor 1: 1162205 Diego Alfonso García Mendiburu
***************************************************/
#include <windows.h>
#include <GL/glut.h>

typedef struct particle{
    GLfloat mass;       // masa
    GLfloat pos[3];    // posición
    GLfloat vel[3]; // velocidad
    GLfloat oldPos[3]; // velocidad
    GLfloat acl[3]; // aceleracion
    GLfloat forces[3]; // acumulador de fuerzas
    GLfloat normal[3];
    GLint fixed;       // partícula fija?
    GLint selected;    // partícula seleccionada?
    GLfloat damping;
    GLint firstHair;
    struct particle *next;
    struct particle *child;
    struct particle *father;
    struct particle *hair;
}Particle;
typedef struct springForce{
    Particle *p1, *p2;      // partículas unidas por el resorte
    GLfloat tension; // tension
    GLfloat r;            // distancia de reposo
    struct springForce *next;
}Spring;

