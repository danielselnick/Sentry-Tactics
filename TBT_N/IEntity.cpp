//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////
#include "IEntity.h"
#include <gl/openglut.h>
#include "Shared.h"
#include "GameValues.h"
#include "Gameplay.h"
#include "BitStream.h"

void IEntity::Render()
{
    glColor4f(entityColor[R], entityColor[G], entityColor[B], 1.0f);
    glBegin(GL_POLYGON);
    // top right
    glVertex3i(position[X] * CELL_WIDTH + CELL_WIDTH, position[Y] * CELL_HEIGHT + CELL_HEIGHT, ARENA_Z );
    // top left
    glVertex3i(position[X] * CELL_WIDTH, position[Y] * CELL_HEIGHT + CELL_HEIGHT, ARENA_Z);
    // bottom left
    glVertex3i(position[X] * CELL_WIDTH, position[Y] * CELL_HEIGHT, ARENA_Z);
    // botom right
    glVertex3i(position[X] * CELL_WIDTH + CELL_WIDTH, position[Y] * CELL_HEIGHT, ARENA_Z);
    glEnd();
}

IEntity::~IEntity()
{

}

IEntity::IEntity( Gameplay & gPlay )
{
    gamePlay = &gPlay;
    position[X] = 0;
    position[Y] = 0;
    knockBack = 0;
    damage = 1;
    range = 3;
    aimDirection = RIGHT;
}

void IEntity::ReadOrWrite( RakNet::BitStream * stream, ROW action )
{
    if(action == READ)
    {
        stream->Read(position[X]);
        stream->Read(position[Y]);
        stream->Read(aimDirection);
    }
    else if(action == WRITE)
    {
        stream->Write(position[X]);
        stream->Write(position[Y]);
        stream->Write(aimDirection);
    }
}

void IEntity::LandKnockbackAttack( AimDirection direction, int damage)
{
    knockBack += damage;
    switch (direction)
    {
    case RIGHT:
        position[X] += knockBack;
        break;
    case UP:
        position[Y] += knockBack;
        break;
    case LEFT:
         position[X] -= knockBack;
        break;
    case DOWN:
        position[Y] -= knockBack;
        break;
    }
}

void IEntity::UpdateAimDirection( int x, int y )
{
    if((x > position[X]) && (y == position[Y]))
        aimDirection = RIGHT;
    else if((x < position[X]) && (y == position[Y]))
        aimDirection = LEFT;
    else if((x == position[X]) && (y > position[Y]))
        aimDirection = UP;
    else if((x == position[X]) && (y < position[Y]))
        aimDirection = DOWN;
}

bool IEntity::isEntityWithinShootingRange(IEntity * entity)
{
    switch (aimDirection)
    {
    case RIGHT:
        if ((entity->position[X] - position[X] <= range) && (entity->position[Y] == position[Y]))
            return true;
        break;
    case UP:
        if((entity->position[X] == position[X]) && (entity->position[Y] - position[Y] <= range))
            return true;
        break;
    case LEFT:
        if((position[X] - entity->position[X] <= range ) && (entity->position[Y] == position[Y]))
            return true;
        break;
    case DOWN:
        if ((entity->position[X] == position[X]) && (position[Y] - entity->position[Y] <= range))
            return true;
        break;
    }
    return false;
}

bool IEntity::IsOutofBounds()
{
    // right
    if (position[X] > ARENA_WIDTH)
    {
        return true;
    }
    // left
    else if (position[X] < 0)
    {
        return true;
    }
    // up
    else if (position[Y] > ARENA_HEIGHT)
    {
        return true;
    }
    // down
    else if(position[Y] < 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void IEntity::RenderAim()
{
    glPushMatrix();
    glEnable(GL_BLEND);
    glColor4f(aimColor[R], aimColor[G], aimColor[B], 0.5f);
    switch(aimDirection)
    {
    case RIGHT:
        glBegin(GL_POLYGON);
        // top right
        glVertex3i(((position[X] + range) * CELL_WIDTH) + CELL_WIDTH, (position[Y] * CELL_HEIGHT) + CELL_HEIGHT, ARENA_Z);
        // top left
        glVertex3i(position[X] * CELL_WIDTH, position[Y] * CELL_HEIGHT + CELL_HEIGHT, ARENA_Z);
        // bottom left
        glVertex3i(position[X] * CELL_WIDTH, position[Y] * CELL_HEIGHT, ARENA_Z);
        // bottom right
        glVertex3i((position[X] + range) * CELL_WIDTH + CELL_WIDTH, position[Y] * CELL_HEIGHT, ARENA_Z);
        glEnd();
        break;
    case UP:
        glBegin(GL_POLYGON);
        // top right
        glVertex3i(position[X] * CELL_WIDTH + CELL_WIDTH, (position[Y] + range) * CELL_HEIGHT + CELL_HEIGHT, ARENA_Z);
        // top left
        glVertex3i(position[X] * CELL_WIDTH, (position[Y] + range) * CELL_HEIGHT + CELL_HEIGHT, ARENA_Z);
        // bottom left
        glVertex3i(position[X] * CELL_WIDTH, position[Y] * CELL_HEIGHT, ARENA_Z);
        // bottom right 
        glVertex3i(position[X] * CELL_WIDTH + CELL_WIDTH, position[Y] * CELL_HEIGHT, ARENA_Z);
        glEnd();    
        break;
    case LEFT:
        glBegin(GL_POLYGON);
        // top right
        glVertex3i(position[X] * CELL_WIDTH + CELL_WIDTH, position[Y] * CELL_HEIGHT + CELL_HEIGHT, ARENA_Z);
        // top left
        glVertex3i((position[X] - range) * CELL_WIDTH, position[Y] * CELL_HEIGHT + CELL_HEIGHT, ARENA_Z);
        // bottom left
        glVertex3i((position[X] - range) * CELL_WIDTH, position[Y] * CELL_HEIGHT, ARENA_Z);
        // bottom right
        glVertex3i(position[X] * CELL_WIDTH + CELL_WIDTH, position[Y] * CELL_HEIGHT, ARENA_Z);
        glEnd();
        break;
    case DOWN:
        glBegin(GL_POLYGON);
        // top right
        glVertex3i(position[X] * CELL_WIDTH + CELL_WIDTH, position[Y] * CELL_HEIGHT + CELL_HEIGHT, ARENA_Z);
        // top left
        glVertex3i(position[X] * CELL_WIDTH, position[Y] * CELL_HEIGHT + CELL_HEIGHT, ARENA_Z);
        // bottom left
        glVertex3i(position[X] * CELL_WIDTH, (position[Y] - range) * CELL_HEIGHT, ARENA_Z);
        // bottom right 
        glVertex3i(position[X] * CELL_WIDTH + CELL_WIDTH, (position[Y] - range) * CELL_HEIGHT, ARENA_Z);
        glEnd();
        break;
    }
    glPopMatrix();
}

void IEntity::SetEntityColor( vec4_f color )
{
    CloneColor(color, entityColor);
}

void IEntity::SetAimColor( vec4_f color )
{
    CloneColor(color, aimColor);
}

void IEntity::SetPosition( vec2_i newPosition )
{
    position[X] = newPosition[X];
    position[Y] = newPosition[Y];
}