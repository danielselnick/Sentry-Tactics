//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////
#include "Network.h"
#include "NetBase.h"
#include "Gameplay.h"
#include "RakNet.h"
#include "Foundation.h"
#include "BitStream.h"

SINGLETON_DEFINITION(Network);
Network::Network()
{
    mp_peer = RakNet::RakPeerInterface::GetInstance();
}

Network::~Network()
{

}

bool Network::ConnectToPlayer( SystemAddress host )
{
    ConnectionAttemptResult result = mp_peer->Connect(host.ToString(false), host.port, 0, 0);
    switch (result)
    {
    case CONNECTION_ATTEMPT_STARTED:
        cout << "Connected to player" << endl;
        return true;
        break;
    case INVALID_PARAMETER:
        cout << "invalid parameter when connecting to peer" << endl;
        break;
    case CANNOT_RESOLVE_DOMAIN_NAME:
        cout << "can not resolve the domain in in connection attempt." << endl;
        break;
    case ALREADY_CONNECTED_TO_ENDPOINT:
        cout << "already connected to peer" << endl;
        break;
    case CONNECTION_ATTEMPT_ALREADY_IN_PROGRESS:
        cout << "already attempting to connect to peer" << endl;
        break;
    case SECURITY_INITIALIZATION_FAILED: 
        cout << "no security, very insecure" << endl;
        break;
    }
    return false;   
}

bool Network::CreateGame()
{
    mp_peer->SetMaximumIncomingConnections(PEER_MAX_CONNECTIONS);
    cout << "Game Created" << endl;
    return true;
}

void Network::StartGame()
{
    if (mp_peer->NumberOfConnections() == 1)
    {   
        RakNet::BitStream * stream = new RakNet::BitStream();
        stream->Write((RakNet::MessageID)START_GAME);
        Message(stream);
        delete stream;
        cout << "Game Started" << endl;
    }
    else
    {
        cout << "Can't start game, must have only one peer connected" << endl;
    }
}

void Network::Disconnect()
{

}

void Network::Update()
{
    for(this->mp_packet = mp_peer->Receive(); mp_packet; mp_peer->DeallocatePacket(mp_packet), mp_packet = mp_peer->Receive())
    {
        // Handle all the RakNet Messages
        switch(mp_packet->data[0])
        {
        case ID_UNCONNECTED_PONG:
            {
                bool isFound = false;
                cout << "Received PONG" << endl;
                for(vector<RakNet::SystemAddress>::iterator it = m_hostList.begin(); it != m_hostList.end(); it++)
                {
                   
                    if(it->binaryAddress == mp_packet->systemAddress.binaryAddress)
                    {
                        isFound = true;
                    }
                }
                if (!isFound)
                {
                    m_hostList.push_back(mp_packet->systemAddress);
                }                
            }
            break;
        case START_GAME:
            {
                Foundation::GetInstance()->SetGameState(InGame);
            }
            break;
        case ID_CONNECTION_REQUEST_ACCEPTED:
            {
                cout << "Connected to game!" << endl;
            }
            break;
        }
        // Handle all the gameplay messages
        uint8 gameMsg = static_cast<uint8>(mp_packet->data[0]);
        if(gameMsg > 113 )
        {
            RakNet::BitStream * stream = new RakNet::BitStream(mp_packet->data, mp_packet->length, false);
            Foundation::GetInstance()->GetGamePlay()->HandleUpdate(stream);
            delete stream;
        }
    }
}

// Pings for active hosts!
void Network::Ping()
{   
    cout << "Pinging local subnet" << endl;
    mp_peer->Ping(LAN_SUBNET, PEER1_PORT, true);
    mp_peer->Ping(LAN_SUBNET, PEER2_PORT, true);
}

vector<RakNet::SystemAddress> Network::GetHostList() const
{
    return m_hostList;
}

bool Network::StartUp()
{
    StartupResult result = mp_peer->Startup(PEER_MAX_CONNECTIONS, &RakNet::SocketDescriptor(PEER1_PORT, LOCAL_HOST_ADDRESS), 1);
    if(result == RAKNET_STARTED)
    {
        cout << "RakNet Started" << endl;
        return true;
    }
    else if(result == SOCKET_PORT_ALREADY_IN_USE)
    {
        result = mp_peer->Startup(PEER_MAX_CONNECTIONS, &RakNet::SocketDescriptor(PEER2_PORT, LOCAL_HOST_ADDRESS), 1);
        cout << "RakNet Started" << endl;
        return (result == RAKNET_STARTED);
    }
    else
        return false;        
}

short Network::NumberOfConnections()
{
    return (mp_peer->NumberOfConnections());
}

void Network::Message( RakNet::BitStream * stream )
{
    mp_peer->Send(stream, PACKET_PRIORITY, PACKET_RELIABILITY, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}