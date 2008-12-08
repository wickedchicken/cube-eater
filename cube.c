/* 
 * cube.c
 *
 * colored cube definitions (including display list creation)
 *
 * this is heavily based on cubeview.c from Chapter 5 of Angel
 *
 */


#include <GL/glut.h>

extern unsigned int cube;
extern unsigned int cube_r;

GLfloat vertices[][3] = {{-0.3,-0.3,-0.3},{0.3,-0.3,-0.3},
	{0.3,0.3,-0.3}, {-0.3,0.3,-0.3}, {-0.3,-0.3,0.3}, 
	{0.3,-0.3,0.3}, {0.3,0.3,0.3}, {-0.3,0.3,0.3}};

GLfloat normals[][3] = {{-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},
	{1.0,1.0,-1.0}, {-1.0,1.0,-1.0}, {-1.0,-1.0,1.0}, 
	{1.0,-1.0,1.0}, {1.0,1.0,1.0}, {-1.0,1.0,1.0}};

GLfloat colors[][3] = {{0.0,0.0,0.0},{1.0,0.0,0.0},
	{1.0,1.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0}, 
	{1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0}};

// draw a polygon
void polygon(int a, int b, int c , int d)
{
	glBegin(GL_POLYGON);
		glColor3fv(colors[a]);
		glNormal3fv(normals[a]);
		glVertex3fv(vertices[a]);
		glColor3fv(colors[b]);
		glNormal3fv(normals[b]);
		glVertex3fv(vertices[b]);
		glColor3fv(colors[c]);
		glNormal3fv(normals[c]);
		glVertex3fv(vertices[c]);
		glColor3fv(colors[d]);
		glNormal3fv(normals[d]);
		glVertex3fv(vertices[d]);
	glEnd();
}

// draw a red polygon
void polygon_red(int a, int b, int c , int d)
{
	glBegin(GL_POLYGON);
		glColor3f(0.7,0.0,0.0);
		glNormal3fv(normals[a]);
		glVertex3fv(vertices[a]);
		glColor3f(0.7,0.0,0.0);
		glNormal3fv(normals[b]);
		glVertex3fv(vertices[b]);
		glColor3f(0.7,0.0,0.0);
		glNormal3fv(normals[c]);
		glVertex3fv(vertices[c]);
		glColor3f(0.7,0.0,0.0);
		glNormal3fv(normals[d]);
		glVertex3fv(vertices[d]);
	glEnd();
}


void colorcube_r(){
	glCallList(cube_r);
}

// build the red colorcube display list
GLuint build_colorcube_r()
{
	unsigned int list = glGenLists(1);
	glNewList(list,GL_COMPILE);
	polygon_red(0,3,2,1);
	polygon_red(2,3,7,6);
	polygon_red(0,4,7,3);
	polygon_red(1,2,6,5);
	polygon_red(4,5,6,7);
	polygon_red(0,1,5,4);
	glEndList();
	return list;
}

void colorcube(){
	glCallList(cube);
}

// build the colorcube display list
GLuint build_colorcube()
{
	unsigned int list = glGenLists(1);
	glNewList(list,GL_COMPILE);

	polygon(0,3,2,1);
	polygon(2,3,7,6);
	polygon(0,4,7,3);
	polygon(1,2,6,5);
	polygon(4,5,6,7);
	polygon(0,1,5,4);

	glEndList();

	return list;
}
