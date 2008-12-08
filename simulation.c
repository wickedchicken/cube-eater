/*
 * simulation.c
 *
 * this file has code for the simulation aspect of the program
 *
 * currently this means camera and player rotation (PID)
 * and playing field generation
 *
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include "vecops.h"
#include <string.h>
#include <error.h>
#include "simulation.h"

// simulation step size defined in cubeview.c
extern const unsigned long granularity;

// there is some inefficiency here with regard to calculating vecdist and pointat
// they are effectively computing vecdiff twice
// adjust a vector based on PID controller output
void pid(GLfloat * source,const GLfloat * dest,const float p, const float i,const float d,float * preverr,float * integral){
	GLfloat err,derivative,out; 
	GLfloat dir[3];
	int j =0 ;

	assert(NULL != preverr);
	assert(NULL != integral);
	assert(NULL != source);
	assert(NULL != dest);
	assert(p >= 0);
	assert(i >= 0);
	assert(d >= 0);

	err = vecdist(dest,source);
	if(err < 0.001){
		return;
	}
	(*integral) += err*(float)granularity;
	derivative = (err - *preverr)/(float)granularity;
	//out = p*err + i*(*integral) + d * derivative;
	out = p*err;
	*preverr = err;

	pointat(source,dest,dir);

	for(;j<3;j++){
		source[j] += out*dir[j];
	}
}

// seek through the current playing field
// and return an array of cube hits
GLdouble * fillgrid(const long x, const long y){
	const long gridx = 200;
	const long gridy = 200;
	GLdouble *grid = malloc((sizeof(long) * gridx * gridy * 3)+1);
	unsigned long count = 0;
	long seekx = -(gridx/2);
	long seeky = -(gridy/2);
	unsigned long hash;
	unsigned long hashx;

	if(NULL == grid){
		perror("error calling malloc()");
		exit(1);
	}

	// in a large square playing surface,
	// for each x,y pair calculate a hash function
	// if that hash is > 65500, spawn a cube
	for(;seekx<(gridx/2);seekx++){
		hashx = hash32shiftmult(x+seekx);
		seeky = -(gridy/2);
		for(;seeky<(gridy/2);seeky++){
			hash = (hashx ^ hash32shiftmult(y+seeky)) & 0xFFFF;
			if(hash > 65500){
				grid[1+(count*3)] = x+seekx;
				grid[2+(count*3)] = y+seeky;
				grid[3+(count*3)] = 1; // value for cube
				count++;
			}
		}
	}

	grid[0] = count;
	return grid;
}

void xycrds(GLdouble *matx, double *x, double *y){
	*x = matx[12]/matx[15];
	*y = matx[14]/matx[15];
}

// from http://www.concentric.net/~Ttwang/tech/inthash.htm
unsigned long int hash32shiftmult(unsigned long key)
{
  unsigned long c2=0x27d4eb2d; // a prime or an odd constant
  key = (key ^ 61) ^ (key >> 16);
  key = key + (key << 3);
  key = key ^ (key >> 4);
  key = key * c2;
  key = key ^ (key >> 15);
  return key;
}
