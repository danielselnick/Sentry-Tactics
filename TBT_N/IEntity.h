//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#ifndef ENTITY_H
#define ENTITY_H
#include "Shared.h"
#include "GameValues.h"
#include "RakNet.h"

class Gameplay;
// Although it's similar to IGameInterface, it will change later on as needed
// to be more specific to gameplay
class IEntity : public NetworkIDObject
{
public:
    IEntity(Gameplay & gPlay);
	~IEntity();
    virtual void Render();
    virtual void Update() = 0;
    virtual void HandleKey(int key, int x, int y) = 0;
    virtual void ReadOrWrite(RakNet::BitStream * stream, ROW action);
    // shoot at another entity, check's if the entity is within range, if it is then it lands a hit
    virtual bool isEntityWithinShootingRange(IEntity * entity);
    // lands a hit on to the entity
    virtual void LandKnockbackAttack(AimDirection, int);
    void UpdateAimDirection(int x, int y);
    bool IsOutofBounds();
    void SetEntityColor(vec4_f color);
    void SetAimColor(vec4_f color);
    void SetPosition(vec2_i newPosition);
protected:
    vec2_i position;
    int knockBack;
    int damage;
    int range;
    Gameplay * gamePlay;
    AimDirection aimDirection;
    void RenderAim();
    vec4_f entityColor;
    vec4_f aimColor;
private:
};
#endif // ENTITY_