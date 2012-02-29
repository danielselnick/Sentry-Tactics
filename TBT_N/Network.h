//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#ifndef NEWNETWORK_H
#define NEWNETWORK_H
#include "Shared.h"
#include "NetBase.h"
#include "BitStream.h"

using namespace RakNet;

class Network : public NetBase
{
public:
    SINGLETON_DECLARATION(Network);
	Network();
	~Network();
    bool StartUp();
    bool ConnectToPlayer(RakNet::SystemAddress address);
    bool CreateGame();
    void StartGame();
    void Disconnect();
    void Update();
    void Ping();
    short NumberOfConnections();
    vector<RakNet::SystemAddress> GetHostList() const;
    void Message(RakNet::BitStream * stream);
protected:
private:
    vector<SystemAddress> m_hostList;
};

#endif // NEWNETWORK_H