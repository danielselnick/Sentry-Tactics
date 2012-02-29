//////////////////////////////////////////////////////////////////////////
// Author: Daniel Selnick
// Email: D.Selnick@gmail.com
// Copyright 2010 Daniel Selnick
// License found in License.txt
//////////////////////////////////////////////////////////////////////////

#ifndef RAKNET_H
#define RAKNET_H

// Pre-processor include RakNet.lib
#pragma comment(lib, "RakNet.lib")
// Holds the packet type
#include "RakNetTypes.h"
// Holds communications
#include "RakPeerInterface.h"
// Bit stream object for sending data
#include "BitStream.h"
// Shared Message IDs
#include "MessageID.h"
// Networked Objects
#include "NetworkIDObject.h"
// Networked ID Manager to handle the objects
#include "NetworkIDManager.h"
// I hate typing RakNet:: over and over again
using namespace RakNet;

#define PEER1_PORT 60000
#define PEER2_PORT 60001
#define PEER_MAX_CONNECTIONS 2
#define PACKET_PRIORITY HIGH_PRIORITY
#define PACKET_RELIABILITY RELIABLE_ORDERED
#define LOCAL_HOST_ADDRESS "127.0.0.1"
#define LAN_SUBNET "255.255.255.255"

#endif // RAKNET_H