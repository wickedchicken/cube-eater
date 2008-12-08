/*
 * skybox.h
 *
 * header definitions for skybox code
 *
 */

#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glut.h>

GLuint LoadTextureRAW( const char * filename, int wrap );
GLuint buildrendersky(GLuint *skybox);
void rendersky(GLuint sky);

#endif
