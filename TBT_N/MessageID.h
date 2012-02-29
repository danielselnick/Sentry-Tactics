//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#ifndef _MESSAGE_ID_H_
#define _MESSAGE_ID_H_
// We are extending the MessageIdentifiers
#include "MessageIdentifiers.h"

//////////////////////////////////////////////////////////////////////////
// Game Messages Enum
// GM_C_* = Game Message from Client
// GM_H_* = Game Message from Host
// GM_D_* = Game Message from Directory
//////////////////////////////////////////////////////////////////////////

// The game messages are written as if the two peers are in casual conversation
enum GameMessages
{
    // Test "helloworld" message
    // ID_USER_PACKET_ENUM is where we extend from RakNet Message ID
    N_HELLOWORLD = ID_USER_PACKET_ENUM + 1,
    START_GAME,
    // One peer is telling the other that you need to update YOUR player data, because I changed it
    YOUR_PLAYER_DATA,
    // One peer is telling the other that you need to update MY player data, because I changed it
    MY_PLAYER_DATA,
    // One peer is telling the other that it's now YOUR turn
    BEGIN_YOUR_TURN,
    // One peer is telling the other that their time is up!
    END_YOUR_TURN,
    // Place sentry
    CREATE_SENTRY,
    I_WIN_GAME,
    YOU_WIN_GAME,
};

enum MessageType
{
    Client,
    Host,
    Menu,
};

#endif // _MESSAGE_ID_H_