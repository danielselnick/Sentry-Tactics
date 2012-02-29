//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#ifndef GAMEVALUES_H
#define GAMEVALUES_H
#include "Shared.h"

SCDEFINE int ARENA_WIDTH = 20;
SCDEFINE int ARENA_HEIGHT = 20;
SCDEFINE int ARENA_Z = 1;
SCDEFINE int CELL_WIDTH = WINDOW_WIDTH / ARENA_WIDTH;
SCDEFINE int CELL_HEIGHT = WINDOW_HEIGHT / ARENA_HEIGHT;
SCDEFINE int VIEW_WIDTH = WINDOW_WIDTH;
SCDEFINE int VIEW_HEIGHT = WINDOW_HEIGHT;
#define END_TURN_KEY KEY_ENTER
#define MAX_NUMBER_MOVES 5

#define MYCOLOR colorYellow
#define MYAIMCOLOR colorWhite
#define MYSENTRYCOLOR Orange
#define MYSENTRYAIMCOLOR colorLtGrey

#define ENEMYCOLOR colorRed
#define ENEMYAIMCOLOR colorWhite
#define ENEMYSENTRYCOLOR Violet
#define ENEMYSENTRYAIMCOLOR colorLtGrey

#define DVORAK
#ifdef DVORAK
#define MOVE_UP ','
#define MOVE_DOWN 'o'
#define MOVE_RIGHT 'e'
#define MOVE_LEFT 'a'
#else
#define MOVE_UP 'w'
#define MOVE_DOWN 's'
#define MOVE_RIGHT 'd'
#define MOVE_LEFT 'a'
#endif

#ifdef DVORAK
// using number pad
#define AIM_UP 56
#define AIM_RIGHT 54
#define AIM_LEFT 52
#define AIM_DOWN 50
#else
#define AIM_UP GLUT_KEY_UP
#define AIM_RIGHT GLUT_KEY_RIGHT
#define AIM_LEFT GLUT_KEY_LEFT
#define AIM_DOWN GLUT_KEY_DOWN
#endif

#define SHOOT_KEY KEY_SPACE
#define SHOOT_MOUSE_BUTTON GLUT_LEFT_BUTTON
#define SENTRY_MOUSE_BUTTON GLUT_RIGHT_BUTTON
// tab == 9
#define SENTRY_KEY 9

enum AimDirection
{
    RIGHT, UP, LEFT, DOWN,
};

#endif // GAMEVALUES_H