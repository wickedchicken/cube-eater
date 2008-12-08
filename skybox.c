/*
 * skybox.c
 *
 * code to load, create display list for and display skybox
 *
 * some of this code was used form other sources where noted
 */


#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// current ship heading in cubeview.c
extern float shiphdg;

// this function taken from http://www.nullterminator.net/gltexture.html
// shouldn't this be mmap() for speed?
// load a 2048x2048 RGB .RAW file as a texture
GLuint LoadTextureRAW( const char * filename, int wrap )
{
    GLuint texture;
    int width, height;
    char * data;
    FILE * file;

    // open texture data
    file = fopen( filename, "rb" );
    if ( file == NULL ) return 0;

    // allocate buffer
    width = 2048;
    height = 2048;
    data = malloc( width * height * 3 );

    // read texture data
    fread( data, width * height * 3, 1, file );
    fclose( file );

    // allocate a texture name
    glGenTextures( 1, &texture );

    // select our current texture
    glBindTexture( GL_TEXTURE_2D, texture );

    // select modulate to mix texture with color for shading
    //glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

    // build our texture mipmaps
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,
                       GL_RGB, GL_UNSIGNED_BYTE, data );


    // free buffer
    free( data );

    return texture;
}

// render skybox around camera
void rendersky(GLuint sky){
    glPushMatrix();
 	glRotatef(shiphdg, 0.0, 1.0, 0.0);
	glCallList(sky);
    glPopMatrix();
}

// most of this is taken from
// http://sidvind.com/wiki?title=Skybox_tutorial&oldid=2196
// build a skybox from the six textures loaded
GLuint buildrendersky(GLuint *skybox){
	unsigned int sky = glGenLists(1);
	glNewList(sky,GL_COMPILE);

	assert(NULL != skybox);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


	// Enable/Disable features
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	// Just in case we set all vertices to white.
	glColor4f(1,1,1,1);

	// Render the front quad
	glBindTexture(GL_TEXTURE_2D, skybox[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3d(  300.5f, -300.5f, -300.5f );
	glTexCoord2f(1, 1); glVertex3d( -300.5f, -300.5f, -300.5f );
	glTexCoord2f(1, 0); glVertex3d( -300.5f,  300.5f, -300.5f );
	glTexCoord2f(0, 0); glVertex3d(  300.5f,  300.5f, -300.5f );
	glEnd();

	// Render the left quad
	glBindTexture(GL_TEXTURE_2D, skybox[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3d(  300.5f, -300.5f,  300.5f );
	glTexCoord2f(1, 1); glVertex3d(  300.5f, -300.5f, -300.5f );
	glTexCoord2f(1, 0); glVertex3d(  300.5f,  300.5f, -300.5f );
	glTexCoord2f(0, 0); glVertex3d(  300.5f,  300.5f,  300.5f );
	glEnd();

	// Render the back quad
	glBindTexture(GL_TEXTURE_2D, skybox[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3d( -300.5f, -300.5f,  300.5f );
	glTexCoord2f(1, 1); glVertex3d(  300.5f, -300.5f,  300.5f );
	glTexCoord2f(1, 0); glVertex3d(  300.5f,  300.5f,  300.5f );
	glTexCoord2f(0, 0); glVertex3d( -300.5f,  300.5f,  300.5f );

	glEnd();

	// Render the right quad
	glBindTexture(GL_TEXTURE_2D, skybox[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3d( -300.5f, -300.5f, -300.5f );
	glTexCoord2f(1, 1); glVertex3d( -300.5f, -300.5f,  300.5f );
	glTexCoord2f(1, 0); glVertex3d( -300.5f,  300.5f,  300.5f );
	glTexCoord2f(0, 0); glVertex3d( -300.5f,  300.5f, -300.5f );
	glEnd();

	// Render the top quad
	glBindTexture(GL_TEXTURE_2D, skybox[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1); glVertex3d( -300.5f,  300.5f, -300.5f );
	glTexCoord2f(1, 0); glVertex3d( -300.5f,  300.5f,  300.5f );
	glTexCoord2f(0, 0); glVertex3d(  300.5f,  300.5f,  300.5f );
	glTexCoord2f(0, 1); glVertex3d(  300.5f,  300.5f, -300.5f );
	glEnd();

	// Render the bottom quad
	glBindTexture(GL_TEXTURE_2D, skybox[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0); glVertex3d( -300.5f, -300.5f, -300.5f );
	glTexCoord2f(1, 1); glVertex3d( -300.5f, -300.5f,  300.5f );
	glTexCoord2f(0, 1); glVertex3d(  300.5f, -300.5f,  300.5f );
	glTexCoord2f(0, 0); glVertex3d(  300.5f, -300.5f, -300.5f );
	glEnd();

	// Restore enable bits and matrix
	glPopAttrib();
	glEndList();
	return sky;
}
