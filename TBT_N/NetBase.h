//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#ifndef NETBASE_H
#define NETBASE_H
#include "IGameService.h"
#include "RakNet.h"

struct Player_t
{
    AddressOrGUID loc;
    RakNet::RakString name;
};

class NetBase
{
public:
    NetBase() {}
    ~NetBase() {}
protected:
    RakNet::RakPeerInterface * mp_peer;
    Packet * mp_packet;
    BitStream m_bsOut;
    RakNet::RakString m_rs;
};

#endif // NETBASE_H