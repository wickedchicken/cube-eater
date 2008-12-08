/*
 * vecops.h
 *
 * header definitions for vector operations
 *
 */

#ifndef VECOPS_H
#define VECOPS_H

#include <GL/glut.h>
float vecdist(const GLfloat *one, const GLfloat *two);
void vecdiff(const GLfloat *from, const GLfloat *to, float *result);
void pointat(const GLfloat *from, const GLfloat *to, float *result);
float veclen(const GLfloat *vec);
void printvec(const GLfloat *vec,const char name[]);

#endif
