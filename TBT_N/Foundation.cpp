//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////
#include "Foundation.h"
#include "glRenderer.h"
#include "Menu.h"
#include "Gameplay.h"
#include "Network.h"
#include <gl/openglut.h>

void glutSetCallbacks();
void glutDisplay();
void glutReshape(int, int);
void glutMouse(int, int, int, int);
void glutKeyboard(unsigned char, int, int);
void glutSpecial(int, int, int);
void glutPassiveMotion();
void glutWindowStatus(int);
void glutClose( void );
void glutPassiveMotion(int x, int y);

SINGLETON_DEFINITION(Foundation);

Foundation::Foundation()
{
    menu = new NewMenu();
    gamePlay = new Gameplay();
}

Foundation::~Foundation()
{
    delete menu;
    delete gamePlay;
}

void Foundation::SetGameState( GameState state)
{
    IGameService * service;
    switch(state)
    {
        
    case InMenuSystem:
        {
            service = dynamic_cast<IGameService *>(menu);
        } break;
    case InGame:
        {
            service = dynamic_cast<IGameService *>(gamePlay);
        } break;
    }
    if(service != NULL)
    {
        currentService = service;
    }
}

void Foundation::WindowReshape( int width, int height )
{
    WindowSize[X] = width;
    WindowSize[Y] = height;
}

void Foundation::Mouse( int button, int state, int x, int y )
{
    if(currentService != NULL)
        currentService->Mouse(button, state, x, y);
}

void Foundation::HandleKey( int key, int x, int y )
{
    if(currentService != NULL)
        currentService->HandleKey(key, x, y);
}

void Foundation::Render()
{
    if(currentService != NULL)
        currentService->Render();
    glutSwapBuffers();
}

void Foundation::Update()
{
    if(currentService != NULL)
        currentService->Update();
    Network::GetInstance()->Update();
}

void Foundation::MouseWheel(int position)
{

}

void Foundation::WindowStatus( int state )
{

}

void Foundation::Close()
{

}

Gameplay * Foundation::GetGamePlay()
{
    return gamePlay;
}

void Foundation::Initialize()
{
    menu->Initialize();
    WindowSize[X] = WINDOW_WIDTH;
    WindowSize[Y] = WINDOW_HEIGHT;
    glutSetCallbacks();
    Network * network = Network::GetInstance();
    if(!network->StartUp())
    {
        cout << "RakNet couldn't be started, terminating program" << endl;
        system("pause");
        exit(0);
    }
    else
        cout << "RakNet Started" << endl;
    glRender::GetInstance()->EnableOpenGL();
}

NewMenu * Foundation::GetMenu()
{
    return menu;
}

void Foundation::StartGame()
{
    this->SetGameState(InGame);
    Network::GetInstance()->StartGame();
    gamePlay->StartGame();
}

void glutSetCallbacks()
{
    glutDisplayFunc(glutDisplay);
    glutIdleFunc(glutDisplay);
    glutReshapeFunc(glutReshape);
    glutKeyboardFunc(glutKeyboard);
    glutSpecialFunc(glutSpecial);
    glutMouseFunc(glutMouse);
    glutWindowStatusFunc(glutWindowStatus);
    glutPassiveMotionFunc(glutPassiveMotion);
    glutCloseFunc( glutClose );
}

void glutDisplay()
{
    Foundation::GetInstance()->Update();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Foundation::GetInstance()->Render();
    glutPostRedisplay();
}

void glutReshape(int width, int height)
{
    Foundation::GetInstance()->WindowReshape(width, height);
}

void glutMouse(int button, int state, int x, int y)
{
    Foundation::GetInstance()->Mouse(button, state, x, y);
}

void glutKeyboard(unsigned char key, int x, int y  )
{
    Foundation::GetInstance()->HandleKey(key, x, y);
}

void MouseWheel( int position )
{
    Foundation::GetInstance()->MouseWheel(position);
}

void glutWindowStatus(int state)
{
    Foundation::GetInstance()->WindowStatus(state);
}

void glutClose( void )
{
    Foundation::GetInstance()->Close();
}

void glutSpecial( int key, int x, int y)
{
    Foundation::GetInstance()->HandleKey(key, x, y);
}

void glutPassiveMotion( int x, int y )
{
    Foundation::GetInstance()->Mouse(-1, -1, x, y);
}