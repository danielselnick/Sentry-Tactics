/// \file
/// \brief \b [Internal] Datagram reliable, ordered, unordered and sequenced sends.  Flow control.  Message splitting, reassembly, and coalescence.
///
/// This file is part of RakNet Copyright 2003 Jenkins Software LLC
///
/// Usage of RakNet is subject to the appropriate license agreement.
/// Creative Commons Licensees are subject to the
/// license found at
/// http://creativecommons.org/licenses/by-nc/2.5/
/// Single application licensees are subject to the license found at
/// http://www.jenkinssoftware.com/SingleApplicationLicense.html
/// Custom license users are subject to the terms therein.
/// GPL license users are subject to the GNU General Public
/// License as published by the Free
/// Software Foundation; either version 2 of the License, or (at your
/// option) any later version.

#ifndef __RELIABILITY_LAYER_H
#define __RELIABILITY_LAYER_H

#include "RakMemoryOverride.h"
#include "MTUSize.h"
#include "DS_LinkedList.h"
#include "DS_List.h"
#include "SocketLayer.h"
#include "PacketPriority.h"
#include "DS_Queue.h"
#include "BitStream.h"
#include "InternalPacket.h"
#include "RakNetStatistics.h"
#include "SHA1.h"
#include "DS_OrderedList.h"
#include "DS_RangeList.h"
#include "DS_BPlusTree.h"
#include "DS_MemoryPool.h"
#include "CCRakNetUDT.h"
#include "DS_Multilist.h"
#include "RakNetDefines.h"
#include "DS_Heap.h"
#include "BitStream.h"
#include "NativeFeatureIncludes.h"

#if LIBCAT_SECURITY==1
#include <cat/AllTunnel.hpp>
#endif

/// Number of ordered streams available. You can use up to 32 ordered streams
#define NUMBER_OF_ORDERED_STREAMS 32 // 2^5

#define RESEND_TREE_ORDER 32

namespace RakNet {

	/// Forward declarations
class PluginInterface2;
class RakNetRandom;
typedef uint64_t reliabilityHeapWeightType;

// int SplitPacketIndexComp( SplitPacketIndexType const &key, InternalPacket* const &data );
struct SplitPacketChannel//<SplitPacketChannel>
{
	CCTimeType lastUpdateTime;

	// This is here for progress notifications, since progress notifications return the first packet data, if available
	InternalPacket *firstPacket;

	// DataStructures::OrderedList<SplitPacketIndexType, InternalPacket*, SplitPacketIndexComp> splitPacketList;
	DataStructures::List<InternalPacket*> splitPacketList;
};
int RAK_DLL_EXPORT SplitPacketChannelComp( SplitPacketIdType const &key, SplitPacketChannel* const &data );

// Helper class
struct BPSTracker
{
	BPSTracker();
	~BPSTracker();
	void Reset(const char *file, unsigned int line);
	void Push1(CCTimeType time, uint64_t value1);
//	void Push2(RakNet::TimeUS time, uint64_t value1, uint64_t value2);
	uint64_t GetBPS1(CCTimeType time);
	uint64_t GetBPS1Threadsafe(CCTimeType time);
//	uint64_t GetBPS2(RakNetTimeUS time);
//	void GetBPS1And2(RakNetTimeUS time, uint64_t &out1, uint64_t &out2);
	uint64_t GetTotal1(void) const;
//	uint64_t GetTotal2(void) const;

	struct TimeAndValue2
	{
		TimeAndValue2();
		~TimeAndValue2();
		TimeAndValue2(CCTimeType t, uint64_t v1);
	//	TimeAndValue2(RakNet::TimeUS t, uint64_t v1, uint64_t v2);
	//	uint64_t value1, value2;
		uint64_t value1;
		CCTimeType time;
	};

	uint64_t total1, lastSec1;
//	uint64_t total2, lastSec2;
	DataStructures::Queue<TimeAndValue2> dataQueue;
	void ClearExpired1(CCTimeType time);
//	void ClearExpired2(RakNet::TimeUS time);
};

/// Datagram reliable, ordered, unordered and sequenced sends.  Flow control.  Message splitting, reassembly, and coalescence.
class ReliabilityLayer//<ReliabilityLayer>
{
public:

	// Constructor
	ReliabilityLayer();

	// Destructor
	~ReliabilityLayer();

	/// Resets the layer for reuse
	void Reset( bool resetVariables, int MTUSize, bool _useSecurity );

	/// Set the time, in MS, to use before considering ourselves disconnected after not being able to deliver a reliable packet
	/// Default time is 10,000 or 10 seconds in release and 30,000 or 30 seconds in debug.
	/// \param[in] time Time, in MS
	void SetTimeoutTime( RakNet::TimeMS time );

	/// Returns the value passed to SetTimeoutTime. or the default if it was never called
	/// \param[out] the value passed to SetTimeoutTime
	RakNet::TimeMS GetTimeoutTime(void);

	/// Packets are read directly from the socket layer and skip the reliability layer because unconnected players do not use the reliability layer
	/// This function takes packet data after a player has been confirmed as connected.
	/// \param[in] buffer The socket data
	/// \param[in] length The length of the socket data
	/// \param[in] systemAddress The player that this data is from
	/// \param[in] messageHandlerList A list of registered plugins
	/// \param[in] MTUSize maximum datagram size
	/// \retval true Success
	/// \retval false Modified packet
	bool HandleSocketReceiveFromConnectedPlayer(
		const char *buffer, unsigned int length, SystemAddress systemAddress, DataStructures::List<PluginInterface2*> &messageHandlerList, int MTUSize,
		SOCKET s, RakNetRandom *rnr, unsigned short remotePortRakNetWasStartedOn_PS3, CCTimeType timeRead);

	/// This allocates bytes and writes a user-level message to those bytes.
	/// \param[out] data The message
	/// \return Returns number of BITS put into the buffer
	BitSize_t Receive( unsigned char**data );

	/// Puts data on the send queue
	/// \param[in] data The data to send
	/// \param[in] numberOfBitsToSend The length of \a data in bits
	/// \param[in] priority The priority level for the send
	/// \param[in] reliability The reliability type for the send
	/// \param[in] orderingChannel 0 to 31.  Specifies what channel to use, for relational ordering and sequencing of packets.
	/// \param[in] makeDataCopy If true \a data will be copied.  Otherwise, only a pointer will be stored.
	/// \param[in] MTUSize maximum datagram size
	/// \param[in] currentTime Current time, as per RakNet::GetTimeMS()
	/// \param[in] receipt This number will be returned back with ID_SND_RECEIPT_ACKED or ID_SND_RECEIPT_LOSS and is only returned with the reliability types that contain RECEIPT in the name
	/// \return True or false for success or failure.
	bool Send( char *data, BitSize_t numberOfBitsToSend, PacketPriority priority, PacketReliability reliability, unsigned char orderingChannel, bool makeDataCopy, int MTUSize, CCTimeType currentTime, uint32_t receipt );

	/// Call once per game cycle.  Handles internal lists and actually does the send.
	/// \param[in] s the communication  end point
	/// \param[in] systemAddress The Unique Player Identifier who shouldhave sent some packets
	/// \param[in] MTUSize maximum datagram size
	/// \param[in] time current system time
	/// \param[in] maxBitsPerSecond if non-zero, enforces that outgoing bandwidth does not exceed this amount
	/// \param[in] messageHandlerList A list of registered plugins
	void Update( SOCKET s, SystemAddress systemAddress, int MTUSize, CCTimeType time,
		unsigned bitsPerSecondLimit,
		DataStructures::List<PluginInterface2*> &messageHandlerList,
		RakNetRandom *rnr, unsigned short remotePortRakNetWasStartedOn_PS3);
	

	/// If Read returns -1 and this returns true then a modified packetwas detected
	/// \return true when a modified packet is detected
	bool IsCheater( void ) const;

	/// Were you ever unable to deliver a packet despite retries?
	/// \return true means the connection has been lost.  Otherwise not.
	bool IsDeadConnection( void ) const;

	/// Causes IsDeadConnection to return true
	void KillConnection(void);

	/// Get Statistics
	/// \return A pointer to a static struct, filled out with current statistical information.
	RakNetStatistics * const GetStatistics( RakNetStatistics *rns );

	///Are we waiting for any data to be sent out or be processed by the player?
	bool IsOutgoingDataWaiting(void);
	bool AreAcksWaiting(void);

	// Set outgoing lag and packet loss properties
	void ApplyNetworkSimulator( double _maxSendBPS, RakNet::TimeMS _minExtraPing, RakNet::TimeMS _extraPingVariance );

	/// Returns if you previously called ApplyNetworkSimulator
	/// \return If you previously called ApplyNetworkSimulator
	bool IsNetworkSimulatorActive( void );

	void SetSplitMessageProgressInterval(int interval);
	void SetUnreliableTimeout(RakNet::TimeMS timeoutMS);
	/// Has a lot of time passed since the last ack
	bool AckTimeout(RakNet::TimeMS curTime);
	CCTimeType GetNextSendTime(void) const;
	CCTimeType GetTimeBetweenPackets(void) const;
	CCTimeType GetAckPing(void) const;
	RakNet::TimeMS GetTimeLastDatagramArrived(void) const {return timeLastDatagramArrived;}

	// If true, will update time between packets quickly based on ping calculations
	//void SetDoFastThroughputReactions(bool fast);

	// Encoded as numMessages[unsigned int], message1BitLength[unsigned int], message1Data (aligned), ...
	//void GetUndeliveredMessages(RakNet::BitStream *messages, int MTUSize);

private:
	/// Send the contents of a bitstream to the socket
	/// \param[in] s The socket used for sending data
	/// \param[in] systemAddress The address and port to send to
	/// \param[in] bitStream The data to send.
	void SendBitStream( SOCKET s, SystemAddress systemAddress, RakNet::BitStream *bitStream, RakNetRandom *rnr, unsigned short remotePortRakNetWasStartedOn_PS3, CCTimeType currentTime);

	///Parse an internalPacket and create a bitstream to represent this data
	/// \return Returns number of bits used
	BitSize_t WriteToBitStreamFromInternalPacket( RakNet::BitStream *bitStream, const InternalPacket *const internalPacket, CCTimeType curTime );


	/// Parse a bitstream and create an internal packet to represent this data
	InternalPacket* CreateInternalPacketFromBitStream( RakNet::BitStream *bitStream, CCTimeType time );

	/// Does what the function name says
	unsigned RemovePacketFromResendListAndDeleteOlderReliableSequenced( const MessageNumberType messageNumber, CCTimeType time, DataStructures::List<PluginInterface2*> &messageHandlerList, SystemAddress systemAddress );

	/// Acknowledge receipt of the packet with the specified messageNumber
	void SendAcknowledgementPacket( const DatagramSequenceNumberType messageNumber, CCTimeType time);

	/// This will return true if we should not send at this time
	bool IsSendThrottled( int MTUSize );

	/// We lost a packet
	void UpdateWindowFromPacketloss( CCTimeType time );

	/// Increase the window size
	void UpdateWindowFromAck( CCTimeType time );

	/// Parse an internalPacket and figure out how many header bits would be written.  Returns that number
	BitSize_t GetMaxMessageHeaderLengthBits( void );
	BitSize_t GetMessageHeaderLengthBits( const InternalPacket *const internalPacket );

	/// Get the SHA1 code
	void GetSHA1( unsigned char * const buffer, unsigned int nbytes, char code[ SHA1_LENGTH ] );

	/// Check the SHA1 code
	bool CheckSHA1( char code[ SHA1_LENGTH ], unsigned char * const buffer, unsigned int nbytes );

	/// Search the specified list for sequenced packets on the specified ordering channel, optionally skipping those with splitPacketId, and delete them
	void DeleteSequencedPacketsInList( unsigned char orderingChannel, DataStructures::List<InternalPacket*>&theList, int splitPacketId = -1 );

	/// Search the specified list for sequenced packets with a value less than orderingIndex and delete them
	void DeleteSequencedPacketsInList( unsigned char orderingChannel, DataStructures::Queue<InternalPacket*>&theList );

	/// Returns true if newPacketOrderingIndex is older than the waitingForPacketOrderingIndex
	bool IsOlderOrderedPacket( OrderingIndexType newPacketOrderingIndex, OrderingIndexType waitingForPacketOrderingIndex );

	/// Split the passed packet into chunks under MTU_SIZE bytes (including headers) and save those new chunks
	void SplitPacket( InternalPacket *internalPacket );

	/// Insert a packet into the split packet list
	void InsertIntoSplitPacketList( InternalPacket * internalPacket, CCTimeType time );

	/// Take all split chunks with the specified splitPacketId and try to reconstruct a packet. If we can, allocate and return it.  Otherwise return 0
	InternalPacket * BuildPacketFromSplitPacketList( SplitPacketIdType splitPacketId, CCTimeType time,
		SOCKET s, SystemAddress systemAddress, RakNetRandom *rnr, unsigned short remotePortRakNetWasStartedOn_PS3);
	InternalPacket * BuildPacketFromSplitPacketList( SplitPacketChannel *splitPacketChannel, CCTimeType time );

	/// Delete any unreliable split packets that have long since expired
	//void DeleteOldUnreliableSplitPackets( CCTimeType time );

	/// Creates a copy of the specified internal packet with data copied from the original starting at dataByteOffset for dataByteLength bytes.
	/// Does not copy any split data parameters as that information is always generated does not have any reason to be copied
	InternalPacket * CreateInternalPacketCopy( InternalPacket *original, int dataByteOffset, int dataByteLength, CCTimeType time );

	/// Get the specified ordering list
	DataStructures::LinkedList<InternalPacket*> *GetOrderingListAtOrderingStream( unsigned char orderingChannel );

	/// Add the internal packet to the ordering list in order based on order index
	void AddToOrderingList( InternalPacket * internalPacket );

	/// Inserts a packet into the resend list in order
	void InsertPacketIntoResendList( InternalPacket *internalPacket, CCTimeType time, bool firstResend, bool modifyUnacknowledgedBytes );

	/// Memory handling
	void FreeMemory( bool freeAllImmediately );

	/// Memory handling
	void FreeThreadSafeMemory( void );

	// Initialize the variables
	void InitializeVariables( void );

	/// Given the current time, is this time so old that we should consider it a timeout?
	bool IsExpiredTime(unsigned int input, CCTimeType currentTime) const;

	// Make it so we don't do resends within a minimum threshold of time
	void UpdateNextActionTime(void);


	/// Does this packet number represent a packet that was skipped (out of order?)
	//unsigned int IsReceivedPacketHole(unsigned int input, RakNet::TimeMS currentTime) const;

	/// Skip an element in the received packets list
	//unsigned int MakeReceivedPacketHole(unsigned int input) const;

	/// How many elements are waiting to be resent?
	unsigned int GetResendListDataSize(void) const;

	/// Update all memory which is not threadsafe
	void UpdateThreadedMemory(void);

	void CalculateHistogramAckSize(void);

	// Used ONLY for RELIABLE_ORDERED
	// RELIABLE_SEQUENCED just returns the newest one
	DataStructures::List<DataStructures::LinkedList<InternalPacket*>*> orderingList;
	DataStructures::Queue<InternalPacket*> outputQueue;
	int splitMessageProgressInterval;
	CCTimeType unreliableTimeout;

	struct MessageNumberNode
	{
		DatagramSequenceNumberType messageNumber;
		MessageNumberNode *next;
	};
	struct DatagramHistoryNode
	{
		DatagramHistoryNode() {}
		DatagramHistoryNode(MessageNumberNode *_head) :
		head(_head) {}
		MessageNumberNode *head;
	};
	// Queue length is programmatically restricted to DATAGRAM_MESSAGE_ID_ARRAY_LENGTH
	// This is essentially an O(1) lookup to get a DatagramHistoryNode given an index
	DataStructures::Queue<DatagramHistoryNode> datagramHistory;
	DataStructures::MemoryPool<MessageNumberNode> datagramHistoryMessagePool;


	void RemoveFromDatagramHistory(DatagramSequenceNumberType index);
	MessageNumberNode* GetMessageNumberNodeByDatagramIndex(DatagramSequenceNumberType index);
	void AddFirstToDatagramHistory(DatagramSequenceNumberType datagramNumber);
	MessageNumberNode* AddFirstToDatagramHistory(DatagramSequenceNumberType datagramNumber, DatagramSequenceNumberType messageNumber);
	MessageNumberNode* AddSubsequentToDatagramHistory(MessageNumberNode *messageNumberNode, DatagramSequenceNumberType messageNumber);
	DatagramSequenceNumberType datagramHistoryPopCount;
	
	DataStructures::MemoryPool<InternalPacket> internalPacketPool;
	// DataStructures::BPlusTree<DatagramSequenceNumberType, InternalPacket*, RESEND_TREE_ORDER> resendTree;
	InternalPacket *resendBuffer[RESEND_BUFFER_ARRAY_LENGTH];
	InternalPacket *resendLinkedListHead;
	InternalPacket *unreliableLinkedListHead;
	void RemoveFromUnreliableLinkedList(InternalPacket *internalPacket);
	void AddToUnreliableLinkedList(InternalPacket *internalPacket);
//	unsigned int numPacketsOnResendBuffer;
	//unsigned int blockWindowIncreaseUntilTime;
	//	DataStructures::RangeList<DatagramSequenceNumberType> acknowlegements;
	// Resend list is a tree of packets we need to resend

	// Set to the current time when the resend queue is no longer empty
	// Set to zero when it becomes empty
	// Set to the current time if it is not zero, and we get incoming data
	// If the current time - timeResendQueueNonEmpty is greater than a threshold, we are disconnected
//	CCTimeType timeResendQueueNonEmpty;
	RakNet::TimeMS timeLastDatagramArrived;


	// If we backoff due to packetloss, don't remeasure until all waiting resends have gone out or else we overcount
//	bool packetlossThisSample;
//	int backoffThisSample;
//	unsigned packetlossThisSampleResendCount;
//	CCTimeType lastPacketlossTime;

	//DataStructures::Queue<InternalPacket*> sendPacketSet[ NUMBER_OF_PRIORITIES ];
	DataStructures::Heap<reliabilityHeapWeightType, InternalPacket*, false> outgoingPacketBuffer;
	reliabilityHeapWeightType outgoingPacketBufferNextWeights[NUMBER_OF_PRIORITIES];
	void InitHeapWeights(void);
	reliabilityHeapWeightType GetNextWeight(int priorityLevel);
//	unsigned int messageInSendBuffer[NUMBER_OF_PRIORITIES];
//	double bytesInSendBuffer[NUMBER_OF_PRIORITIES];


    DataStructures::OrderedList<SplitPacketIdType, SplitPacketChannel*, SplitPacketChannelComp> splitPacketChannelList;

	MessageNumberType sendReliableMessageNumberIndex;
	MessageNumberType internalOrderIndex;
	//unsigned int windowSize;
	RakNet::BitStream updateBitStream;
	OrderingIndexType waitingForOrderedPacketWriteIndex[ NUMBER_OF_ORDERED_STREAMS ], waitingForSequencedPacketWriteIndex[ NUMBER_OF_ORDERED_STREAMS ];
	
	// STUFF TO NOT MUTEX HERE (called from non-conflicting threads, or value is not important)
	OrderingIndexType waitingForOrderedPacketReadIndex[ NUMBER_OF_ORDERED_STREAMS ], waitingForSequencedPacketReadIndex[ NUMBER_OF_ORDERED_STREAMS ];
	bool deadConnection, cheater;
	// unsigned int lastPacketSendTime,retransmittedFrames, sentPackets, sentFrames, receivedPacketsCount, bytesSent, bytesReceived,lastPacketReceivedTime;
	SplitPacketIdType splitPacketId;
	RakNet::TimeMS timeoutTime; // How long to wait in MS before timing someone out
	//int MAX_AVERAGE_PACKETS_PER_SECOND; // Name says it all
//	int RECEIVED_PACKET_LOG_LENGTH, requestedReceivedPacketLogLength; // How big the receivedPackets array is
//	unsigned int *receivedPackets;
	RakNetStatistics statistics;

//	CCTimeType histogramStart;
//	unsigned histogramBitsSent;


	/// Memory-efficient receivedPackets algorithm:
	/// receivedPacketsBaseIndex is the packet number we are expecting
	/// Everything under receivedPacketsBaseIndex is a packet we already got
	/// Everything over receivedPacketsBaseIndex is stored in hasReceivedPacketQueue
	/// It stores the time to stop waiting for a particular packet number, where the packet number is receivedPacketsBaseIndex + the index into the queue
	/// If 0, we got got that packet.  Otherwise, the time to give up waiting for that packet.
	/// If we get a packet number where (receivedPacketsBaseIndex-packetNumber) is less than half the range of receivedPacketsBaseIndex then it is a duplicate
	/// Otherwise, it is a duplicate packet (and ignore it).
	// DataStructures::Queue<CCTimeType> hasReceivedPacketQueue;
	DataStructures::Queue<bool> hasReceivedPacketQueue;
	DatagramSequenceNumberType receivedPacketsBaseIndex;
	bool resetReceivedPackets;

	CCTimeType lastUpdateTime;
	CCTimeType timeBetweenPackets, nextSendTime, ackPing;
//	CCTimeType ackPingSamples[ACK_PING_SAMPLES_SIZE]; // Must be range of unsigned char to wrap ackPingIndex properly
	CCTimeType ackPingSum;
	unsigned char ackPingIndex;
	//CCTimeType nextLowestPingReset;
	RemoteSystemTimeType remoteSystemTime;
//	bool continuousSend;
//	CCTimeType lastTimeBetweenPacketsIncrease,lastTimeBetweenPacketsDecrease;
	// Limit changes in throughput to once per ping - otherwise even if lag starts we don't know about it
	// In the meantime the connection is flooded and overrun.
	CCTimeType nextAllowedThroughputSample;
	bool bandwidthExceededStatistic;

	// If Update::maxBitsPerSecond > 0, then throughputCapCountdown is used as a timer to prevent sends for some amount of time after each send, depending on
	// the amount of data sent
	long long throughputCapCountdown;

	unsigned receivePacketCount;

#ifdef _DEBUG
	struct DataAndTime//<InternalPacket>
	{
		SOCKET s;
		char data[ MAXIMUM_MTU_SIZE ];
		unsigned int length;
		RakNet::TimeMS sendTime;
		//	SystemAddress systemAddress;
		unsigned short remotePortRakNetWasStartedOn_PS3;
	};
	DataStructures::Queue<DataAndTime*> delayList;

	// Internet simulator
	double packetloss;
	RakNet::TimeMS minExtraPing, extraPingVariance;
#endif

	CCTimeType elapsedTimeSinceLastUpdate;

	CCTimeType nextAckTimeToSend;

	RakNet::CCRakNetUDT congestionManager;
	uint32_t unacknowledgedBytes;
	
	bool ResendBufferOverflow(void) const;
	void ValidateResendList(void) const;
	void ResetPacketsAndDatagrams(void);
	void PushPacket(CCTimeType time, InternalPacket *internalPacket, bool isReliable);
	void PushDatagram(void);
	bool TagMostRecentPushAsSecondOfPacketPair(void);
	void ClearPacketsAndDatagrams(bool releaseToInternalPacketPoolIfNeedsAck);
	void MoveToListHead(InternalPacket *internalPacket);
	void RemoveFromList(InternalPacket *internalPacket, bool modifyUnacknowledgedBytes);
	void AddToListTail(InternalPacket *internalPacket, bool modifyUnacknowledgedBytes);
	void PopListHead(bool modifyUnacknowledgedBytes);
	bool IsResendQueueEmpty(void) const;
	void SortSplitPacketList(DataStructures::List<InternalPacket*> &data, unsigned int leftEdge, unsigned int rightEdge) const;
	void SendACKs(SOCKET s, SystemAddress systemAddress, CCTimeType time, RakNetRandom *rnr, unsigned short remotePortRakNetWasStartedOn_PS3);

	DataStructures::List<InternalPacket*> packetsToSendThisUpdate;
	DataStructures::List<bool> packetsToDeallocThisUpdate;
	// boundary is in packetsToSendThisUpdate, inclusive
	DataStructures::List<unsigned int> packetsToSendThisUpdateDatagramBoundaries;
	DataStructures::List<bool> datagramsToSendThisUpdateIsPair;
	DataStructures::List<unsigned int> datagramSizesInBytes;
	BitSize_t datagramSizeSoFar;
	BitSize_t allDatagramSizesSoFar;
	double totalUserDataBytesAcked;
	CCTimeType timeOfLastContinualSend;
	CCTimeType timeToNextUnreliableCull;

	// This doesn't need to be a member, but I do it to avoid reallocations
	DataStructures::RangeList<DatagramSequenceNumberType> incomingAcks;

	// Every 16 datagrams, we make sure the 17th datagram goes out the same update tick, and is the same size as the 16th
	int countdownToNextPacketPair;
	InternalPacket* AllocateFromInternalPacketPool(void);
	void ReleaseToInternalPacketPool(InternalPacket *ip);

	DataStructures::RangeList<DatagramSequenceNumberType> acknowlegements;
	DataStructures::RangeList<DatagramSequenceNumberType> NAKs;
	bool remoteSystemNeedsBAndAS;

	unsigned int GetMaxDatagramSizeExcludingMessageHeaderBytes(void);
	BitSize_t GetMaxDatagramSizeExcludingMessageHeaderBits(void);

	// ourOffset refers to a section within externallyAllocatedPtr. Do not deallocate externallyAllocatedPtr until all references are lost
	void AllocInternalPacketData(InternalPacket *internalPacket, InternalPacketRefCountedData **refCounter, unsigned char *externallyAllocatedPtr, unsigned char *ourOffset);
	// Set the data pointer to externallyAllocatedPtr, do not allocate
	void AllocInternalPacketData(InternalPacket *internalPacket, unsigned char *externallyAllocatedPtr);
	// Allocate new
	void AllocInternalPacketData(InternalPacket *internalPacket, unsigned int numBytes, const char *file, unsigned int line);
	void FreeInternalPacketData(InternalPacket *internalPacket, const char *file, unsigned int line);
	DataStructures::MemoryPool<InternalPacketRefCountedData> refCountedDataPool;

	BPSTracker bpsMetrics[RNS_PER_SECOND_METRICS_COUNT];

#if LIBCAT_SECURITY==1
public:
	cat::AuthenticatedEncryption* GetAuthenticatedEncryption(void) { return &auth_enc; }

protected:
	cat::AuthenticatedEncryption auth_enc;
	bool useSecurity;
#endif // LIBCAT_SECURITY
};

} // namespace RakNet

#endif
