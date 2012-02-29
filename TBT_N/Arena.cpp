//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#include "Arena.h"
#include <gl/openglut.h>
#include "GameValues.h"
#include <stdlib.h>
#include <time.h>

void Arena::Render()
{
    glPushMatrix();
    glBegin(GL_POLYGON);
    //Vertices are given in counter clock wise direction
    //if they're given in clockwise direction, then they're considered back facing 
    //Top right 
    glColor4f(0.2f, 0.3f, 0.1f, 0.2f);
    glVertex3f(VIEW_WIDTH, VIEW_HEIGHT, 0.0f); 
    //Top left    
    glColor4f(1.0f, 0.0f, 0.6f, 0.1f);        
    glVertex3f(0.0f, VIEW_HEIGHT, 0.0f);
    //Bottom left
    glColor4f(0.4f, 0.5f, 0.33f, 0.5f);        
    glVertex3f(0.0f, 0.0f, 0.0f); 
    //Bottom right
    glColor4f(0.7f, 0.8f, 0.9f, 0.15f);        
    glVertex3f(VIEW_WIDTH, 0.0f, 0.0f);
    glEnd();

    glColor4f(1.0f, 1.0f, 1.0f, 0.85f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, stars);
    glDrawArrays(GL_POINTS, 0, NUM_OF_STARS);
    glDisableClientState(GL_VERTEX_ARRAY);

    ////Save any previous changes
    glPushMatrix();
    //Render the arena grid
    glBegin(GL_LINES);
    //Color of all the lines white
    glColor4f(1.0f, 1.0f, 1.0f, 0.33f);
    // lines which are parallel to the X axis
    for(float i = 0.0f; i < VIEW_HEIGHT; i += CELL_HEIGHT)
    {
        glVertex3f(0.0f, i, 0.1f);
        glVertex3f(VIEW_WIDTH, i, 0.1f);
    }
    // lines which are parrallel to the Y axis
    for(float i = 0.0f; i < VIEW_WIDTH; i +=CELL_WIDTH)
    {
        glVertex3f(i, 0.0f, 0.1f);
        glVertex3f(i, VIEW_HEIGHT, 0.1f);
    }
    
    glEnd();
    glPopMatrix();
    glPopMatrix();
}

Arena::Arena()
{
    stars = new Point[NUM_OF_STARS];
    srand(time(NULL));
    for(int i = 0; i < NUM_OF_STARS; i++)
    {
        stars[i].x = rand() % VIEW_WIDTH;
        stars[i].y = rand() % VIEW_HEIGHT;
    }
}

Arena::~Arena()
{
    delete []stars;
}