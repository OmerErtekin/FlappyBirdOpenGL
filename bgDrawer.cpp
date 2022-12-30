/*
 * TextureBMP.cpp		- Version 1.1, Revised January 2004
 *
 * Read images from four bitmap (.bmp) files, and draw them as
 * texture maps on a quad.  A cpp class RgbImage is used to read
 * a texture map from a bitmap (.bmp) file.  Illustrates how to manage
 * multiple texture maps in OpenGL.
 *
 * Author: Samuel R. Buss
 *
 * Software accompanying the book.
 *		3D Computer Graphics: A Mathematical Introduction with OpenGL,
 *		by S. Buss, Cambridge University Press, 2003.
 *
 * Software is "as-is" and carries no warranty.  It may be used without
 *   restriction, but if you modify it, please change the filenames to
 *   prevent confusion between different versions.
 * Bug reports: Sam Buss, sbuss@ucsd.edu.
 * Web page: http://math.ucsd.edu/~sbuss/MathCG
 *
 */
#ifndef BACKGROUNDDRAWER_H
#define BACKGROUNDDRAWER_H

#include <stdlib.h>
#include <GL/freeglut.h>
#include "RgbImage.h"

static GLuint textureName[4];

/*
 * Read a texture map from a BMP bitmap file.
 */

char* filenameArray[4] = {
		"background.bmp",
		"LightningTexture.bmp",
		"IvyTexture.bmp",
		"RedLeavesTexture.bmp"
};

void loadTextureFromFile(char *filename)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	RgbImage theTexMap( filename );

	// Pixel alignment: each row is word aligned.  Word alignment is the default.
	// glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	// Set the interpolation settings to best quality.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB,
					 theTexMap.GetNumCols(), theTexMap.GetNumRows(),
					 GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData() );

}

/*
 * Load the four textures, by repeatedly called loadTextureFromFile().
 */
void initFour()
{
	//glGenTextures( 0, textureName );
    glBindTexture(GL_TEXTURE_2D, textureName[0]);
    loadTextureFromFile(filenameArray[0]);
}

/*
 * Display the i-th texture.
 */
void drawTextureQuad() {
   glBindTexture(GL_TEXTURE_2D, textureName[0]);

   glBegin(GL_QUADS);
   glTexCoord2f(0.0, 0.0); glVertex3f(-5, -5.0, 0.0);
   glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, 5.0, 0.0);
   glTexCoord2f(1.0, 1.0); glVertex3f(5.0, 5.0, 0.0);
   glTexCoord2f(1.0, 0.0); glVertex3f(5.0,-5.0, 0.0);
   glEnd();

}

/*
 * Draw the four textures in the OpenGL graphics window
 */
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

   glPushMatrix();
   drawTextureQuad ();
   glPopMatrix();

   glFlush();
   glDisable(GL_TEXTURE_2D);
}

void resizeWindow(int w, int h)
{
	float viewWidth = 2.2;
	float viewHeight = 2.2;
	glViewport(0, 0, w, h);
	h = (h==0) ? 1 : h;
	w = (w==0) ? 1 : w;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if ( h < w ) {
		viewWidth *= (float)w/(float)h;
	}
	else {
		viewHeight *= (float)h/(float)w;
	}
	glOrtho( -viewWidth, viewWidth, -viewHeight, viewHeight, -1.0, 1.0 );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard (unsigned char key,int,int)
{
	switch (key) {
		case 27:
			exit(0);
			break;
		default:
			break;
   }
}
