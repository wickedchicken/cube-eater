/*
 * spaceship.c
 *
 * code to render the spaceship with appropriate tilt and camera angle
 *
 */

#include <GL/glut.h>
#include <assert.h>
#include "vecops.h"

// the vertices of the ship
static GLfloat vertices[][3] =
	{{2.5,-4.0,0.0},	// 0
	{0.5,1.0,-0.25},	// 1
	{0.5,1.0,0.25},	// 2
	{0.5,-1.0,0.0},		// 3
	{-0.5,1.0,-0.25},	// 4
	{-0.5,1.0,0.25},	// 5
	{-0.5,-1.0,0.0},	// 6
	{-2.5,-4.0,0.0}};	// 7

// perform transform and rotate operations, then display ship
void spaceship(float x, float y, float z,float *theta, GLuint shiphandle){
	assert(NULL != theta);
	
	glPushMatrix();
	glTranslatef(-x,-y,-z);
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0);

	glCallList(shiphandle);

	glPopMatrix();
}

// build the ship displaylist
// no surface normals as of yet
GLuint buildship(){
	GLuint ship;

	ship = glGenLists(1);

	glNewList(ship,GL_COMPILE);

	glBegin(GL_TRIANGLE_STRIP);

	glColor3f(0.00,0.0,0.0);
	glVertex3fv(vertices[0]);
	glColor3f(0.70,0,0);
	glVertex3fv(vertices[2]);
	glVertex3fv(vertices[3]);
	glVertex3fv(vertices[5]);
	glVertex3fv(vertices[6]);
	glColor3f(0.00,0.0,0.0);
	glVertex3fv(vertices[7]);
	glColor3f(0.70,0,0);
	glVertex3fv(vertices[4]);
	glVertex3fv(vertices[6]);
	glVertex3fv(vertices[3]);
	glVertex3fv(vertices[1]);
	glColor3f(0.00,0.0,0.0);
	glVertex3fv(vertices[0]);
	glColor3f(0.70,0,0);
	glVertex3fv(vertices[2]);

	glEnd();

	glBegin(GL_QUADS);

	glVertex3fv(vertices[5]);
	glVertex3fv(vertices[2]);
	glVertex3fv(vertices[1]);
	glVertex3fv(vertices[4]);
	glEnd();

	glEndList();

	return ship;
}

