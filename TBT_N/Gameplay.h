//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include "IGameService.h"
#include "Shared.h"
#include "RakNet.h"
#include "GameValues.h"

class Player;
class Arena;
class Network;
class Sentry;

class Gameplay : public IGameService
{
public:
    Gameplay();
    ~Gameplay();
    void Render();
    void Update();
    void HandleKey(int key, int x, int y);
    void Mouse(int button, int state, int x, int y);
    void HandleUpdate(RakNet::BitStream * stream);
    bool CreateSentry(vec2_i location, AimDirection direction);
    void StartGame();
protected:
private:
    void CheckWinOrLose();
    NetworkIDManager * idManager;
    Arena * arena;

    friend class IEntity;

    friend class Player;
    Player * localPlayer;
    Player * otherPlayer;
    
    friend class Sentry;
    vector<Sentry *> * sentries;
    
    bool isMyTurn;
};

#endif // 