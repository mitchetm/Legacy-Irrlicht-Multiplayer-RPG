#pragma once

#include "Singleton.h"

//#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "NetworkIDManager.h"
#include "NetworkIDObject.h"
#include "BitStream.h"
#include "GetTime.h"

#include <string>
#include <map>

using namespace RakNet;

class NetworkManager : public Singleton<NetworkManager>
{

public:

	NetworkManager();
	~NetworkManager();

	void setRevisionNumber(unsigned int rev);
	unsigned int getRevisionNumber(void);
	void setStoredIP(char *ip);
	char *getStoredIP(void);
	void setStoredPort(unsigned int port);
	unsigned int getStoredPort(void);

	void StartServer(int port, int max_clients);
	void StartClient(char *ip, int port);

	void shutDown(void);

	bool GetIsServer(void);

	//void RegisterRPC(void(&RPCMessage)(RPCParameters *rpcParameters));

	// get network time
	unsigned long long int getSystemTimer(void);

	// do we have a new packet
	bool getNewPacket2(void);
	// destroy packet
	void DestroyPacket2(void);

	// add a new client from packet
	unsigned int addClientFromPacketAddress(void);
	// get clientID from packet - if we want to check
	unsigned int getClientIDFromPacketAddress(void);
	void removeClientID(unsigned int clientID);

	// put packet in a bitstream
	void fillBitStreamFromPacket(void);
	// clear bitstream
	void clearBitSteram(void);

	// read data bitstream
	char readCharFromBitStream(void);
	unsigned char readUCharFromBitStream(void);
	int readIntFromBitStream(void);
	unsigned int readUIntFromBitStream(void);
	float readFloatFromBitStream(void);
	bool readBoolFromBitStream(void);
	std::string readStringFromBitStream(void); // we use RakNet::RakString internally
	// read time stamp
	unsigned long long int readTimeStampFromBitStream(void);

	// write data to bitstream
	void writeCharToBitStream(char value);
	void writeUCharToBitStream(unsigned char value);
	void writeIntToBitStream(int value);
	void writeUIntToBitStream(unsigned int value);
	void writeFloatToBitStream(float value);
	void writeBoolToBitStream(bool value);
	void writeStringToBitStream(char *value);
	// write time stamp from outside
	void writeTimeStampToBitStream(unsigned long long int time);
	// write system time stamp
	void writeSystemTimeStampToBitStream(void);

	// send bitstream to client (clientID, int priority, int reliability, char orderingChannel)
	void sendBitStreamToClient(unsigned int clientID, int priority, int reliability, char orderingChannel);
	// send bitstream to all (int priority, int reliability, char orderingChannel);  // when client sends this, it will go to the server
	void sendBitStreamToAll(int priority, int reliability, char orderingChannel);

private:

	// we use this to find the systemAddress from the clientID
	RakNet::SystemAddress getSystemAddressFromClientID(unsigned int clientID);

	unsigned int revisionNumber;
	std::string storedIP;
	unsigned int storedPort;

	RakPeerInterface *m_peer;
	bool m_isServer;

	NetworkIDManager m_serverNetworkIDManager;

	RakNet::Packet *packet; // to send and receive data
	RakNet::BitStream inBitStream; // to read from and to fill the packet
	RakNet::BitStream outBitStream; // to write in it and send it

	unsigned int clientIDCount;
	std::map<unsigned int, int> clientList; // we map an ID to each client index
};
