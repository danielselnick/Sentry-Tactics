//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////
#include "Menu.h"
#include "BaseMenu.h"
#include "ConcreteMenus.h"
#include "Foundation.h"
#include "Network.h"

NewMenu::NewMenu()
{
    
}

NewMenu::~NewMenu()
{
    delete mainMenu;
    delete hostListMenu;
    delete lobbyMenu;
    delete creditsMenu;
    delete exitMenu;
}

void NewMenu::Render()
{
    currentMenu->Render();
}

void NewMenu::HandleKey(int key, int x, int y)
{
    currentMenu->HandleKey(key, x, y);
}

void NewMenu::Switch(BaseMenu * menu)
{
    currentMenu = menu;
}

void NewMenu::StartGame()
{
    Foundation::GetInstance()->StartGame();
}

void NewMenu::Mouse( int button, int state, int x, int y )
{

}

void NewMenu::EndGameScreen( bool isWinner )
{
    Foundation::GetInstance()->SetGameState(InMenuSystem);
    Network::GetInstance()->Disconnect();
    Switch(mainMenu);
}

void NewMenu::Initialize()
{
    mainMenu = new MainMenu(*this);
    hostListMenu = new HostListMenu(*this);
    lobbyMenu = new LobbyMenu(*this);
    creditsMenu = new CreditsMenu(*this);
    exitMenu = new ExitMenu(*this);
    endGameMenu = new EndGameMenu(*this);
    currentMenu = mainMenu;
}