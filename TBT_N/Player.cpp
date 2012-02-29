//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////
#include "GameValues.h"
#include "IEntity.h"
#include "Player.h"
#include <gl/openglut.h>
#include "Gameplay.h"
#include "Network.h"
#include "Sentry.h"
#include "BitStream.h"

Player::Player( Gameplay & gamePlay ) : IEntity(gamePlay)
{
    position[X] = 3;
    position[Y] = 3;
    EndTurn();
}

Player::~Player()
{

}

void Player::Render()
{
    if(isMyTurn)
        IEntity::RenderAim();
    IEntity::Render();
}

void Player::Update()
{
    
}

void Player::HandleKey( int key, int x, int y )
{
    int originalMoves = moves;
    switch (key)
    {
    case MOVE_UP:
        {
            if(position[Y] + 1 < ARENA_HEIGHT)
            {
                position[Y] += 1;
                moves++;
            }
            break;
        }
    case MOVE_DOWN:
        {
            if (position[Y] > 0)
            {
                position[Y] -= 1;
                moves++;
            }            
            break;
        }
    case MOVE_RIGHT:
        {
            if(position[X] + 1 < ARENA_WIDTH)
            {
                position[X] += 1;
                moves++;
            }            
            break;
        }
    case MOVE_LEFT:
        {
            if (position[X] > 0)
            {
                position[X] -= 1;
                moves++;
            }
            break;
        }
    case AIM_UP:
        {
            aimDirection = UP;
            break;
        }
    case AIM_DOWN:
        {
            aimDirection = DOWN;
            break;
        }
    case AIM_RIGHT:
        {  
            aimDirection = RIGHT;
            break;
        }
    case AIM_LEFT:
        {
            aimDirection = LEFT;
            break;
        }
    /*case SHOOT_KEY:
        {
            if(!isAbleToShoot)
            {
                if(IEntity::isEntityWithinShootingRange(gamePlay->otherPlayer))
                {                    
                    moves++;
                    gamePlay->otherPlayer->LandKnockbackAttack(aimDirection, damage);
                    BitStream * stream = new BitStream();
                    stream->Write((MessageID)YOUR_PLAYER_DATA);
                    gamePlay->otherPlayer->ReadOrWrite(stream, WRITE);
                    Network::GetInstance()->Message(stream);
                    delete stream;
                }
            }
            break;
        } */   
    case SENTRY_KEY:
        {
            vec2_i loc;
            loc[X] = position[X];
            loc[Y] = position[Y];
            switch(aimDirection)
            {
            case RIGHT:
                loc[X] += 1;
                break;
            case LEFT:
                loc[X] -= 1;
                break;
            case UP:
                loc[Y] += 1;
                break;
            case DOWN:
                loc[Y] -=1;
                break;
            }
            gamePlay->CreateSentry(loc, aimDirection);
            moves++;
        }
        break;
    }
    if(moves > MAX_NUMBER_MOVES)
    {
        EndTurn();
    }
    else if(moves != originalMoves)
    {
        BitStream * stream = new BitStream();
        stream->Write((MessageID)MY_PLAYER_DATA);
        ReadOrWrite(stream, WRITE);
        Network::GetInstance()->Message(stream);
        delete stream;
    }
}

void Player::ReadOrWrite( RakNet::BitStream * stream, ROW action )
{
    IEntity::ReadOrWrite(stream, action);
    if(action == READ)
    {
        stream->Read(isAbleToShoot);
        stream->Read(knockBack);
        stream->Read(damage);
        stream->Read(moves);
    }
    else if (action == WRITE)
    {
        stream->Write(isAbleToShoot);
        stream->Write(knockBack);
        stream->Write(damage);
        stream->Write(moves);
    }
}

void Player::EndTurn()
{   
    isMyTurn = false;
    isAbleToShoot = false;
    isAbleToLaySentry = false;
    damage = 2;
    moves = 0;
    isMyTurn = false;

    RakNet::BitStream * stream = new RakNet::BitStream();
    stream->Write((MessageID)MY_PLAYER_DATA);
    ReadOrWrite(stream, WRITE);
    Network::GetInstance()->Message(stream);
    stream->Reset();
    stream->Write((MessageID)BEGIN_YOUR_TURN);
    Network::GetInstance()->Message(stream);
    delete stream;    
}

void Player::HandleMouse( int button, int state, int x, int y )
{
    IEntity::UpdateAimDirection(x, y);
    //if (button == SHOOT_MOUSE_BUTTON)
    //{
    //    // Update AimDirection
    //    if((isAbleToShoot == true) && (isMyTurn == true))
    //    {
    //        // base class helpers            
    //        if(IEntity::isEntityWithinShootingRange(gamePlay->otherPlayer))
    //        {
    //            moves++;
    //            gamePlay->otherPlayer->LandKnockbackAttack(aimDirection, damage);
    //            
    //            // might do special player only stuff later, so not putting into the base class just yet
    //            BitStream * stream = new BitStream();
    //            stream->Write((MessageID)YOUR_PLAYER_DATA);
    //            gamePlay->otherPlayer->ReadOrWrite(stream, WRITE);
    //            Network::GetInstance()->Message(stream);
    //            delete stream;
    //        }
    //    }
    //}
    if (button == SENTRY_MOUSE_BUTTON)
    {
        if((isMyTurn == true) && (isAbleToLaySentry == true))
        {
            IEntity::UpdateAimDirection(x, y);
            vec2_i pos;
            pos[X] = position[X];
            pos[Y] = position[Y];
            switch (aimDirection)
            {
            case RIGHT:
                pos[X] += 1;
                break;
            case UP:
                pos[Y] += 1;
                break;
            case LEFT:
                pos[X] -= 1;
                break;
            case DOWN:
                pos[Y] -= 1;
                break;
            }
            if(gamePlay->CreateSentry(pos, aimDirection))
                isAbleToLaySentry = false;
        }
    }
}

void Player::BeginTurn()
{
    isAbleToShoot = true;
    isAbleToLaySentry = true;
    damage = 1;
    moves = 0;
    isMyTurn = true;
}