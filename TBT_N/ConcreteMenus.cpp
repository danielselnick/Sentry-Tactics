//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#include "ConcreteMenus.h"
#include "Menu.h"
#include <vector>
#include "Shared.h"
#include "BaseMenu.h"
#include "Foundation.h"
#include "Network.h"
#include <openglut_std.h>

//////////////////////////////////////////////////////////////////////////
// MainMenu
//////////////////////////////////////////////////////////////////////////

MainMenu::MainMenu(NewMenu & menu) : BaseMenu(menu)
{   
    m_vecItems[0] = "Exit";
    m_vecItems.push_back("Credits");
    m_vecItems.push_back("Join Game");
    m_vecItems.push_back("Create Game");

    m_nSelectedItem = m_vecItems.size() - 1;
    str_header = "Main Menu";
}

void MainMenu::HandleKey( int key, int x, int y )
{        
    BaseMenu::HandleKey( key, x, y );
    if((key == KEY_ENTER) || (key == KEY_SPACE))
    {
        BaseMenu *menu;
        switch ( m_nSelectedItem )
        {
        case GOTO_LAST_MENU:
            {
                menu = dynamic_cast<BaseMenu *>(menuSystem->exitMenu);
            } break;
        case JOIN_GAME:
            {
                menu = dynamic_cast<BaseMenu *>(menuSystem->hostListMenu); 
            } break;
        case CREATE_GAME:
            {
                menu = dynamic_cast<BaseMenu *>(menuSystem->lobbyMenu);
                if(NULL != menu)
                    if(Network::GetInstance()->CreateGame())
                    {
                        menuSystem->Switch(menu);
                        return;
                    }
                else
                    return;
            } break;
        case CREDITS:
            {
                menu = dynamic_cast<BaseMenu *>(menuSystem->creditsMenu);
            } break;
        }
        // want game to crash if this isn't instantiated
        if (NULL != menu)
        {
            menuSystem->Switch(menu);
        }
    }
}
#define REFRESH_LIST 1

//////////////////////////////////////////////////////////////////////////
// HostListMenu
//////////////////////////////////////////////////////////////////////////

HostListMenu::HostListMenu(NewMenu & menu): BaseMenu(menu)
{
    customIP = "swampthaeng.is-a-geek.org";
    // Refresh the host listing
    m_vecItems.push_back("Refresh");    // 1
    //mvec_hostList = &CNetwork::Instance().GetClient().GetHostList();
    str_header = "Host List";
}

HostListMenu::~HostListMenu()
{
    
}

void HostListMenu::Render()
{
    while(m_vecItems.size() > 2)
    {
        m_vecItems.pop_back();
    }
    vector<RakNet::SystemAddress> hostList = Network::GetInstance()->GetHostList();
    for(vector<RakNet::SystemAddress>::iterator it = hostList.begin(); it != hostList.end(); it++)
    {
        m_vecItems.push_back(it->ToString(false));
    }
    BaseMenu::Render();
}

void HostListMenu::HandleKey( int key, int x, int y )
{ 
    BaseMenu::HandleKey(key, x, y);
    if((key == KEY_ENTER) || (key == KEY_SPACE))
    {        
        switch(m_nSelectedItem)
        {            
        case GOTO_LAST_MENU:
            {
                BaseMenu *menu = dynamic_cast<BaseMenu *>(menuSystem->mainMenu);
                if(NULL != menu)
                {
                    menuSystem->Switch(menu);
                }
            } break;
        case 1:
            {
                Network::GetInstance()->Ping();
            } break;
        default:
            {
                if(m_vecItems.size() > m_nSelectedItem)
                {
                    Network::GetInstance()->ConnectToPlayer(Network::GetInstance()->GetHostList()[m_nSelectedItem - 2]);
                }                
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// LobbyMenu
//////////////////////////////////////////////////////////////////////////

LobbyMenu::LobbyMenu(NewMenu & menu) : BaseMenu(menu)
{
    m_vecItems.push_back("Start Game"); // 1
    str_header = "Game Lobby";
}

void LobbyMenu::HandleKey( int key, int x, int y )
{
    BaseMenu::HandleKey(key, x, y);
    if((key == KEY_ENTER) || (key == KEY_SPACE))
    {
        switch(m_nSelectedItem)
        {
        case GOTO_LAST_MENU:
            {
                BaseMenu *menu = dynamic_cast<BaseMenu*>(menuSystem->mainMenu);
                if(menu != NULL)
                {
                    menuSystem->Switch(menu);
                }
            }
            break;
        case 1:
            {
                menuSystem->StartGame();
                BitStream * stream = new BitStream();
                stream->Write((MessageID)END_YOUR_TURN);
                Network::GetInstance()->Message(stream);
                delete stream;
            }
            break;
        }
    }
}

void LobbyMenu::Render()
{
    while(m_vecItems.size() > 2)
    {
        m_vecItems.pop_back();
    }
    int j = Network::GetInstance()->NumberOfConnections();
    for (int i = 0; i < j; i++)
    {
        m_vecItems.push_back("player");
    }
    BaseMenu::Render();
}

//////////////////////////////////////////////////////////////////////////
// ExitMenu
//////////////////////////////////////////////////////////////////////////

ExitMenu::ExitMenu(NewMenu & menu) : BaseMenu(menu)
{
    m_vecItems.push_back("Return to Menu");
    m_vecItems[0] = "Exit Game";
    str_header = "Don't leave me :(";
}
void ExitMenu::HandleKey( int key, int x, int y )
{
    if( key == KEY_ENTER || key == KEY_SPACE )
    {
        switch ( m_nSelectedItem )
        {
            // Since this is the last menu, the menu system will go to state NULL
            // Call the global pointer to quit the game
        case GOTO_LAST_MENU:
            {
                Foundation::DestroyInstance();
                exit(0);
            }
            break;
        case 1:
            {
                BaseMenu *menu = dynamic_cast<BaseMenu *>(menuSystem->mainMenu);
                if (NULL != menu)
                {
                    menuSystem->Switch(menu);
                }
            }            
            break;
        }
    }
    BaseMenu::HandleKey(key, x, y);
}

//////////////////////////////////////////////////////////////////////////
// CreditsMenu
//////////////////////////////////////////////////////////////////////////

CreditsMenu::CreditsMenu(NewMenu & menu) : BaseMenu(menu)
{
    m_vecItems.push_back("Created by Daniel Selnick");
}

void CreditsMenu::HandleKey( int key, int x, int y )
{
    BaseMenu::HandleKey(key, x, y);
    if((key == KEY_ENTER) || (key == KEY_SPACE))
    {
        switch(m_nSelectedItem)
        {
        case GOTO_LAST_MENU:
            {                
                BaseMenu* menu = dynamic_cast<BaseMenu*>(menuSystem->mainMenu);
                if(menu != NULL)
                {
                    menuSystem->Switch(menu);
                }
            }break;
        }
    }
}

EndGameMenu::EndGameMenu( NewMenu & menu) : BaseMenu(menu)
{
    m_vecItems.push_back("Game over");
}

void EndGameMenu::HandleKey( int key, int x, int y )
{
    BaseMenu::HandleKey(key, x, y);
}

void EndGameMenu::PushItem( string item )
{
    m_vecItems.push_back(item);
}