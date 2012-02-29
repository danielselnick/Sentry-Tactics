//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#ifndef SENTRY_H
#define SENTRY_H
#include "Shared.h"
#include "IEntity.h"
#include "BitStream.h"

class Gameplay;

class Sentry : public IEntity
{
public:
    Sentry(Gameplay &);
	Sentry(Gameplay & gameplay, vec2_i loc, AimDirection direction);
	~Sentry();
    void Update();
    void Render();
    void HandleKey(int key, int x, int y);
    void ReadOrWrite(BitStream * stream, ROW action);
    bool Shoot();
    bool hasShot;
protected:
private:
    int damage;
    
};

#endif // SENTRY_H