//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef GUI_H
#define GUI_H
#include "IGameService.h"
#include "GameValues.h"
#include "Shared.h"
#define GUI_FONT GLUT_BITMAP_HELVETICA_18

class GUI : public IGameService
{
public:
    void Render();
    void Update();
    void HandleKey(unsigned char key, int x, int y);
    void Mouse(int button, int state, int x, int y);
protected:
private:
    vec2_f turnpos;
};

#endif // GUI_H