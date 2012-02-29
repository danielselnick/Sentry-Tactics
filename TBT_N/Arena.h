//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ARENA_H
#define ARENA_H
#include "Shared.h"
SCDEFINE int NUM_OF_STARS = 750;

class Arena
{
public:
    Arena();
    ~Arena();
    void Render();
private:
    float size;
    float offset;
    float numberofCells;
    Point * stars;
};
#endif