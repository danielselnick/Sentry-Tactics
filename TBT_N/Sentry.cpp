//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////
#include "Sentry.h"
#include "GameValues.h"
#include "Gameplay.h"
#include <gl/openglut.h>
#include "Player.h"
#include "Network.h"

Sentry::Sentry(Gameplay & gameplay, vec2_i loc, AimDirection aimdirection) : IEntity(gameplay)
{
    position[X] = loc[X];
    position[Y] = loc[Y];
    aimDirection = aimdirection;
    hasShot = false;
    damage = 2;
}

Sentry::Sentry( Gameplay & gp) : IEntity(gp)
{
    position[X] = 0;
    position[Y] = 0;
    aimDirection = DOWN;
    damage = 1;
    range = 3;
}

Sentry::~Sentry()
{

}

void Sentry::Update()
{
    if(!hasShot)
    {
        if(IEntity::isEntityWithinShootingRange(gamePlay->otherPlayer))
        {
            gamePlay->otherPlayer->LandKnockbackAttack(aimDirection, damage);

            BitStream * stream = new BitStream();
            stream->Write((MessageID)YOUR_PLAYER_DATA);
            gamePlay->otherPlayer->ReadOrWrite(stream, WRITE);
            Network::GetInstance()->Message(stream);
            delete stream;

            hasShot = true;
        }
    }
}

void Sentry::Render()
{
    IEntity::RenderAim();
    IEntity::Render();
}

void Sentry::HandleKey( int key, int x, int y )
{
}

void Sentry::ReadOrWrite( RakNet::BitStream * stream, ROW action )
{
    IEntity::ReadOrWrite(stream, action);
    // custom sentry stuff
}