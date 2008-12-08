/*
 * simulation.h
 *
 * header definitions for simulation code
 *
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include <GL/glut.h>
void pid(GLfloat * source,const GLfloat * dest,const float p, const float i, const float d,float *preverr,float *integral);
void xycrds(GLdouble *matx, double * x, double * y);
GLdouble * fillgrid(const long x, const long y);
unsigned long int hash32shiftmult(unsigned long key);

#endif
