//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////
#include "glRenderer.h"
#include "Shared.h"
#include <GL/openglut.h>
#include <stdlib.h>
#include "Foundation.h"

SINGLETON_DEFINITION(glRender);

glRender::glRender()
{
    
}

glRender::~glRender()
{
}

void glRender::glPrint(const char *str, void *font, float r, float g, float b,
              float x, float y) {
  /* font: font to use, e.g., GLUT_BITMAP_HELVETICA_10
     r, g, b: text colour
     x, y: text position in window: range [0,0] (bottom left of window)
           to [1,1] (top right of window). */

  const char *ch;
  GLint matrixMode;
  GLboolean lightingOn;

 lightingOn= glIsEnabled(GL_LIGHTING);        /* lighting on? */
 if (lightingOn) glDisable(GL_LIGHTING);

 glGetIntegerv(GL_MATRIX_MODE, &matrixMode);  /* matrix mode? */

 glMatrixMode(GL_PROJECTION);
 glPushMatrix();
   glLoadIdentity();
   gluOrtho2D(0.0, Foundation::GetInstance()->WindowSize[X], 0.0, Foundation::GetInstance()->WindowSize[X]);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
     glLoadIdentity();
     glPushAttrib(GL_COLOR_BUFFER_BIT);       /* save current colour */
       glColor3f(r, g, b);
       glRasterPos3f(x, y, 0.0);
       for(ch= str; *ch; ch++) {
          glutBitmapCharacter(font, (int)*ch);
       }
     glPopAttrib();
   glPopMatrix();
   glMatrixMode(GL_PROJECTION);
 glPopMatrix();
 glMatrixMode(matrixMode);
 if (lightingOn) glEnable(GL_LIGHTING);
}

void glRender::glPrint(const char* ch, void* font, vec4_f color, vec2_f pos)
{
    glPrint( ch, font, color[R], color[G], color[B], pos[0], pos[1] );
}

void glRender::glProjectionState( ProjectionState state)
{
    if(CurrentProjectionState != state && state == Ortho)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();   
        gluOrtho2D(0, 1024, 0, 768);
        glMatrixMode(GL_MODELVIEW);
        CurrentProjectionState = Ortho;
    }
    else if (CurrentProjectionState != state && state == Perspective)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, (GLfloat) (WINDOW_WIDTH / WINDOW_HEIGHT), 1.0, 100.0);
        glMatrixMode(GL_MODELVIEW);
        CurrentProjectionState = Perspective;
    }
}
    
void glRender::RenderCube(float posx, float posy)
{
    glPushMatrix();    
    //Translate to correct area
    glTranslatef(posx, posy, 0.0);

    //The bottom
    glPushMatrix();    
    glRectf(0.0f, 0.0f, 1.0f, 1.0f);
    glPopMatrix();

    //The top
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 1.0f);
    glRectf(0.0f, 0.0f, 1.0f, 1.0f);
    glPopMatrix();

    //The front
    glPushMatrix();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f );
    glRectf(0.0f, 0.0f, 1.0f, 1.0f);
    glPopMatrix();

    //The back
    glPushMatrix();
    //Translate to the back
    glTranslatef(0.0f, 1.0f, 0.0f);
    //Rotate up
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glRectf(0.0f, 0.0f, 1.0f, 1.0f);
    glPopMatrix();

    //The left side
    glPushMatrix();
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glRectf(0.0f, 0.0f, 1.0f, 1.0f);
    glPopMatrix();

    //The right side
    glPushMatrix();
    //Translate to the right
    glTranslatef(1.0f, 0.0f, 0.0f);
    //Rotate up
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glRectf(1.0f, 1.0f, 0.0f, 0.0f);
    glPopMatrix();

    //Maybe do a wireframe later...
    glPopMatrix();
}

void glRender::EnableOpenGL()
{
    // Init opengl
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //Smooth shading in of colors
    glShadeModel(GL_SMOOTH);
    //Blend Function
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Blend colors
    glEnable(GL_BLEND);
    //Normalize normals
    glEnable(GL_NORMALIZE);
}