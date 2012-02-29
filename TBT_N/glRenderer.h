//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef GLFUNC_H
#define GLFUNC_H
#include "IGameService.h"
#include "Shared.h"
#include <queue>
#define INIT_WINDOW_POSITION_X 100
#define INIT_WINDOW_POSITION_Y 100
#define INIT_WINDOW_SIZE_X 800
#define INIT_WINDOW_SIZE_Y 480

enum ProjectionState
{
    Ortho,
    Perspective
};

class glRender
{
public:
    glRender();
    ~glRender();
    SINGLETON_DECLARATION(glRender);
    void EnableOpenGL();
    void HandleKey(unsigned char key, int x, int y);
    void glPrint(const char*, void * font, float r, float g, float b, float x, float y);
    void glPrint(const char*, void*, vec4_f, vec2_f);
    void glProjectionState(ProjectionState);
    void RenderCube(float posx, float posy);
private:
    ProjectionState CurrentProjectionState;
};

#endif //GLFUNC_H