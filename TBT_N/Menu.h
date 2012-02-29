//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#ifndef NEWMENU_H
#define NEWMENU_H

#include "Shared.h"
#include "IGameService.h"

#define MENU_FONT GLUT_BITMAP_8_BY_13
#define MENU_FONT_SELECTED GLUT_BITMAP_HELVETICA_18
#define FONT_HEIGHT_INCREMENT 34;
#define MENU_START_LOCATION_X ( WINDOW_WIDTH * 0.10f )
#define MENU_START_LOCATION_Y ( WINDOW_HEIGHT - (WINDOW_HEIGHT * 0.250f) )
#define MENU_FONT_HEADER GLUT_BITMAP_TIMES_ROMAN_24
#define MENU_HOSTLIST_START_LOC_X 100
#define MENU_HOSTLIST_START_LOC_Y 100
#define MENU_HOSTLIST_MAX_NUM_HOSTS 10
#define MENU_MAX_NUM_HOSTLIST 10
#define GOTO_LAST_MENU 0
#define JOIN_GAME 2
#define CREATE_GAME 3
#define CREDITS 1
static vec4_f MENU_BACKGROUND_COLOR = 
{ colorBlack[R], colorBlack[G], colorBlack[B], colorBlack[A] };
static vec4_f MENU_FONT_COLOR = 
{ colorWhite[R], colorWhite[G], colorWhite[B], colorWhite[A] };
static vec4_f MENU_FONT_COLOR_SELECTED = 
{ colorGreen[R], colorGreen[G], colorGreen[B], colorGreen[A] };

//////////////////////////////////////////////////////////////////////////
// Forward declaration of internal menu classes
//////////////////////////////////////////////////////////////////////////
// The base menu which all others inherit from
class BaseMenu;
// Intial main menu
class MainMenu;
// Player chooses between joining a localhost, internet, or LAN game
class JoinMenu;
// Player chooses between creating a localhost, internet, or LAN game
class CreateGameMenu;
// Host listing of all the hosts a player can join
class HostListMenu;
// The game lobby with other players, waiting for the game to begin
class LobbyMenu;
// Credits!
class CreditsMenu;
// When a player has won the game
class EndGameMenu;
// Double check that the player wishes to leave
class ExitMenu;

class Foundation;
class BaseMenu;
class GameFinder;

class NewMenu : public IGameService
{
public:
	NewMenu();
	~NewMenu();
    void Initialize();
    void Render();
    void Update() {}
    void HandleKey(int key, int x, int y);
    // todo: choosing menu with mouse
    void Mouse(int button, int state, int x, int y);
    void EndGameScreen(bool isWinner);
protected:
private:
    void StartGame();
    void Switch(BaseMenu *);
    BaseMenu * currentMenu;
    GameFinder * gameFinder;
    MainMenu * mainMenu;
    HostListMenu * hostListMenu;
    LobbyMenu * lobbyMenu;
    CreditsMenu * creditsMenu;
    EndGameMenu * endGameMenu;
    ExitMenu * exitMenu;
    // don't want to write geters, so I'll make em my buddies :D
    friend class BaseMenu;
    friend class MainMenu;
    friend class HostListMenu;
    friend class LobbyMenu;
    friend class EndGameMenu;
    friend class CreditsMenu;
    friend class ExitMenu;
};

#endif // NEWMENU_H