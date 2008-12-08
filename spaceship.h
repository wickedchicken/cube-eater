/*
 * spaceship.h
 *
 * header definitions for spaceship rendering code
 *
 */

#ifndef SPACESHIP_H
#define SPACESHIP_H
#include <GL/glut.h>

void spaceship(float x, float y, float z,float *theta, GLuint shiphandle);
GLuint buildship();

#endif
