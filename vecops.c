/*
 * vecops.c
 *
 * common vector operations
 *
 */

#include <GL/glut.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// this is veclen(vecdiff())
// calculate the euclidean distance betweent two vectors
float vecdist(const GLfloat *one, const GLfloat *two){
	float xdiff,ydiff,zdiff;
	assert(NULL != one);
	assert(NULL != two);

	xdiff=two[0] - one[0];
	ydiff=two[1] - one[1];
	zdiff=two[2] - one[2];

	return sqrt(xdiff*xdiff + ydiff*ydiff + zdiff*zdiff);
}

// calculate the length of a vector
float veclen(const GLfloat *vec){
	assert(NULL != vec);
	return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

// valculate the difference between two vectors
void vecdiff(const GLfloat *from, const GLfloat *to, float *result){
	int i=0;
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != result);

	for(;i<3;i++){
		result[i] = to[i] - from[i];
	}
}

// calculate a normalized vector pointint from one vector to another
void pointat(const GLfloat *from, const GLfloat *to, float *result){
	int i=0;
	float dist;
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != result);

	vecdiff(from,to,result);
	dist = veclen(result);
	assert(dist > 0);
	for(;i<3;i++){
		result[i] /= dist;
	}
}

// output a vector to the screen
void printvec(const GLfloat *vec,const char name[]){
	assert(NULL != vec);

	printf("%s: %f %f %f\n",name,vec[0],vec[1],vec[2]);
}
