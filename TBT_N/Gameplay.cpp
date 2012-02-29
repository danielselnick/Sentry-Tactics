//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////
#include "Gameplay.h"
#include "Player.h"
#include "GUI.h"
#include <gl/openglut.h>
#include "Foundation.h"
#include "Shared.h"
#include "Arena.h"
#include "BitStream.h"
#include "Network.h"
#include "Sentry.h"
#include "MessageID.h"
#include "RakNet.h"
#include "Menu.h"

//////////////////////////////////////////////////////////////////////////
// Gameplay
//////////////////////////////////////////////////////////////////////////

Gameplay::Gameplay( )
{
    arena = new Arena();
    localPlayer = new Player(*this);
    localPlayer->SetAimColor(MYAIMCOLOR);
    localPlayer->SetEntityColor(MYCOLOR);
    otherPlayer = new Player(*this);
    otherPlayer->SetAimColor(ENEMYAIMCOLOR);
    otherPlayer->SetEntityColor(ENEMYCOLOR);
    sentries = new vector<Sentry *>();
    idManager = new NetworkIDManager();
}

void Gameplay::Render()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, VIEW_WIDTH, 0.0, VIEW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);

    arena->Render();    
    localPlayer->Render();
    glColor3f(0, 0, 0);
    otherPlayer->Render();

    for (vector<Sentry*>::iterator it = sentries->begin(); it != sentries->end(); it++)
    {
        (*it)->Render();
    }
}

void Gameplay::Update()
{
    // Update local player
    localPlayer->Update();
    // Update other player
    otherPlayer->Update();
    // update all of the sentries
    for (vector<Sentry*>::iterator it = sentries->begin(); it != sentries->end(); it++)
    {        
        (*it)->Update();
    }
    CheckWinOrLose();
}

void Gameplay::HandleKey( int key, int x, int y )
{
    if(key == END_TURN_KEY)
    {
        localPlayer->EndTurn();
    }
    else if(localPlayer->IsMyTurn())
    {
        localPlayer->HandleKey(key, x, y);        
    }
}

void Gameplay::HandleUpdate( RakNet::BitStream * stream )
{
    MessageID id;
    stream->Read(id);
    switch (id)
    {
        // Other player says: "update MY player data" because I changed it.
    case MY_PLAYER_DATA:
        {
            otherPlayer->ReadOrWrite(stream, READ);
        }
    	break;
        // Other player says: "update YOUR player data" because I changed it.
    case YOUR_PLAYER_DATA:
        {
            localPlayer->ReadOrWrite(stream, READ);
        }break;
    case BEGIN_YOUR_TURN:
        {
            for (vector<Sentry*>::iterator it = sentries->begin(); it != sentries->end(); it++)
            {
                (*it)->hasShot = false;
            }
            localPlayer->BeginTurn();
        }break;
    case END_YOUR_TURN:
        {
            localPlayer->EndTurn();
        }break;      
        // enemy sentry!
    case CREATE_SENTRY:
        {
            Sentry * sentry = new Sentry(*this);
            sentries->push_back(sentry);
            sentry->SetAimColor(ENEMYSENTRYAIMCOLOR);
            sentry->SetEntityColor(ENEMYSENTRYCOLOR);

            NetworkID id;
            sentry->SetNetworkIDManager(idManager);
            stream->Read(id);
            sentry->SetNetworkID(id);

            sentry->ReadOrWrite(stream, READ);
        }break;
    case I_WIN_GAME:
        {
            vec2_f loc;
            loc[X] = VIEW_WIDTH / 2;
            loc[Y] = VIEW_HEIGHT / 2;
            cout << "You have lost!" << endl;
            Foundation::GetInstance()->SetGameState(InMenuSystem);
        }break;
    case YOU_WIN_GAME:
        {
            vec2_f loc;
            loc[X] = VIEW_WIDTH / 2;
            loc[Y] = VIEW_HEIGHT / 2;
            cout << "You have won" << endl;
            Foundation::GetInstance()->SetGameState(InMenuSystem);
        }break;
    }
}

// return bool as a sentry may be an invalid location in the future
bool Gameplay::CreateSentry( vec2_i location, AimDirection direction)
{
    Sentry * sentry = new Sentry(*this, location, direction);
    sentry->SetAimColor(MYSENTRYAIMCOLOR);
    sentry->SetEntityColor(MYSENTRYCOLOR);
    sentry->SetNetworkIDManager(idManager);
    sentries->push_back(sentry);

    BitStream * stream = new BitStream();    
    stream->Write((MessageID)CREATE_SENTRY);
    stream->Write(sentry->GetNetworkID());
    sentry->ReadOrWrite(stream, WRITE);
    Network::GetInstance()->Message(stream);
    delete stream;
    return true;
}

Gameplay::~Gameplay()
{
    delete localPlayer;
    delete otherPlayer;
    delete idManager;
    for (vector<Sentry*>::iterator it = sentries->begin(); it != sentries->end(); it++)
    {
        delete (*it);
    }
}

void Gameplay::Mouse( int button, int state, int x, int y )
{
    // convert into gameplay scale
    y = (y * -1) + WINDOW_HEIGHT;
    x = x / CELL_WIDTH;
    y = y / CELL_HEIGHT;
    localPlayer->HandleMouse(button, state, x, y);
}

void Gameplay::CheckWinOrLose()
{
    BitStream * stream;
    if (localPlayer->IsOutofBounds())
    {
        stream = new BitStream();
        stream->Write((MessageID)YOU_WIN_GAME);
        Network::GetInstance()->Message(stream);
        cout << "You have lost!" << endl;
        Foundation::GetInstance()->SetGameState(InMenuSystem);
        Foundation::GetInstance()->GetMenu()->EndGameScreen(false);
    }
    else if (otherPlayer->IsOutofBounds())
    {
        stream = new BitStream();
        stream->Write((MessageID)I_WIN_GAME);
        Network::GetInstance()->Message(stream);
        cout << "You have won!" << endl;
        Foundation::GetInstance()->SetGameState(InMenuSystem);
        Foundation::GetInstance()->GetMenu()->EndGameScreen(true);
    }
    else
    {
        // do nothing
    }
}

void Gameplay::StartGame()
{
    RakNet::BitStream * stream = new RakNet::BitStream();
    vec2_i pos;
    pos[X] = 3;
    pos[Y] = ARENA_HEIGHT - 3;
    localPlayer->SetPosition(pos);
    stream->Write((MessageID)MY_PLAYER_DATA);
    localPlayer->ReadOrWrite(stream, WRITE);
    Network::GetInstance()->Message(stream);

    pos[X] = ARENA_WIDTH - 3;
    pos[Y] = 3;
    otherPlayer->SetPosition(pos);
    stream->Reset();
    stream->Write((MessageID)YOUR_PLAYER_DATA);
    otherPlayer->ReadOrWrite(stream, WRITE);
    Network::GetInstance()->Message(stream);
    delete stream;
}