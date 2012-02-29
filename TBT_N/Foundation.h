//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#ifndef FOUNDATION_H
#define FOUNDATION_H
#include "IGameService.h"
#include "Shared.h"

class glRender;
class NewMenu;
class Gameplay;

enum GameState
{
    InMenuSystem,
    InGame
};

// Controls the game state, and services
class Foundation
{
public:	
    SINGLETON_DECLARATION(Foundation);
    void Initialize();
    void SetGameState( GameState );
    void WindowReshape(int width, int height);
    void Mouse(int button, int state, int x, int y);
    void HandleKey(int key, int x, int y);
    void Render();
    void Update();
    void MouseWheel(int position);
    void WindowStatus(int state);
    void Close();
    void StartGame();
    vec2_i WindowSize;
    Gameplay * GetGamePlay();
    NewMenu * GetMenu();
protected:
    Foundation();
    ~Foundation();
private:
    NewMenu * menu;
    Gameplay * gamePlay;
    IGameService * currentService;
};

#endif // FOUNDATION_H


