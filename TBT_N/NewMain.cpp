//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////
#include "Foundation.h"
#include <gl/openglut.h>

void GameLoop();

int main(int argc, char** argv)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA | GLUT_MULTISAMPLE);
    glutInitWindowPosition(WINDOW_POSITION_X, WINDOW_POSITION_Y);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInit(&argc, argv);
    if(!glutCreateWindow("Prototupon Pre-Alpha"))
        return 0;
    Foundation* foundation = Foundation::GetInstance();
    foundation->Initialize();
    foundation->SetGameState(InMenuSystem);    
    // Initialize
    // Manage whole game state
    // State Menu
    // State In-Game    
    // Delete
    glutMainLoop();
    system("pause");
}

void GameLoop()
{

}