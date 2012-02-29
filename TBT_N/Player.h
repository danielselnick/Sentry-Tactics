//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_H
#define PLAYER_H
#include "IEntity.h"
#include "Shared.h"
#include "BitStream.h"
#include "GameValues.h"

class GamePlay;

class Player : public IEntity
{
public:
    Player(Gameplay & gamePlay);
	~Player();
    void Render();
    void Update();
    void HandleKey(int key, int x, int y);
    // READ from the Stream, or WRITE on to the Stream
    void ReadOrWrite(RakNet::BitStream * stream, ROW action);
    void BeginTurn();
    void EndTurn();
    void HandleMouse(int button, int state, int x, int y);
    bool Shoot();
    bool IsMyTurn() const { return isMyTurn; }
protected:
private:
    bool isAbleToShoot;
    bool isAbleToLaySentry;
    bool isMyTurn;
    int moves;
};
#endif // PLAYER_H