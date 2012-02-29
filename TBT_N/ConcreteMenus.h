//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#ifndef CONCRETEMENUS_H
#define CONCRETEMENUS_H
#include "BaseMenu.h"

class MainMenu : public BaseMenu
{
public:
    MainMenu(NewMenu &);
    void HandleKey(int key, int x, int y);
};

class HostListMenu : public BaseMenu
{
public:
    HostListMenu(NewMenu &);
    ~HostListMenu();
    void HandleKey(int key, int x, int y);
    void Render();
private:
    string customIP;
};

class RakPeerInterface;
class LobbyMenu : public BaseMenu
{
public:
    LobbyMenu(NewMenu &);
    void HandleKey(int key, int x, int y);
    void Render();
private:
};

class CreditsMenu : public BaseMenu
{
public:
    CreditsMenu(NewMenu &);
    void HandleKey(int key, int x, int y);
};

class EndGameMenu : public BaseMenu
{
public:
    EndGameMenu(NewMenu &);
    void HandleKey(int key, int x, int y);
    void PushItem(string item);
};

class ExitMenu : public BaseMenu
{
public:
    ExitMenu(NewMenu &);
    void HandleKey(int key, int x, int y);
};
#endif // CONCRETEMENUS_H