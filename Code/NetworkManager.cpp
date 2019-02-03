#include "NetworkManager.h"
#include <iostream>

using namespace std;

NetworkManager *Singleton<NetworkManager>::sInstance = 0;

NetworkManager::NetworkManager()
{
	revisionNumber = 0;
	storedIP = "localhost";
	storedPort = 6000;

	m_peer = RakNet::RakPeerInterface::GetInstance();
	packet = 0;

	//m_peer->SetNetworkIDManager(&m_serverNetworkIDManager);

	clientIDCount = 0;
}

NetworkManager::~NetworkManager()
{
	RakNet::RakPeerInterface::DestroyInstance(m_peer);
}

void NetworkManager::setRevisionNumber(unsigned int rev)
{
	revisionNumber = rev;
}

unsigned int NetworkManager::getRevisionNumber(void)
{
	return revisionNumber;
}

void NetworkManager::setStoredIP(char *ip)
{
	storedIP = ip;
}

char *NetworkManager::getStoredIP(void)
{
	return (char*)storedIP.c_str();
}

void NetworkManager::setStoredPort(unsigned int port)
{
	storedPort = port;
}

unsigned int NetworkManager::getStoredPort(void)
{
	return storedPort;
}

void NetworkManager::StartServer(int port, int max_clients)
{
	m_peer->Startup(max_clients, &RakNet::SocketDescriptor(port,0), 1);
	m_isServer = true;

	m_peer->SetMaximumIncomingConnections(max_clients);

	//m_serverNetworkIDManager.SetIsNetworkIDAuthority(true);
	//m_peer->SetNetworkIDManager(&m_serverNetworkIDManager);
}

void NetworkManager::StartClient(char *ip, int port)
{
	m_peer->Startup(1,&RakNet::SocketDescriptor(), 1);
	m_isServer = false;

	m_peer->Connect(ip, port, 0,0);

	//m_serverNetworkIDManager.SetIsNetworkIDAuthority(false);
}

void NetworkManager::shutDown(void)
{
	m_peer->Shutdown(300);
}

bool NetworkManager::GetIsServer(void)
{
	return m_isServer;
}

//void NetworkManager::RegisterRPC(void(&RPCMessage)(RPCParameters *rpcParameters))
//{
//	REGISTER_STATIC_RPC(m_peer, RPCMessage);
//}

unsigned long long int NetworkManager::getSystemTimer(void)
{
	return RakNet::GetTime();
}

bool NetworkManager::getNewPacket2(void)
{
	packet = m_peer->Receive();

	if(packet)
	{
		return true;
	}
	else
		return false;
}

void NetworkManager::DestroyPacket2(void)
{
	if(packet)
		m_peer->DeallocatePacket(packet);

	packet = 0;
}

unsigned int NetworkManager::addClientFromPacketAddress(void)
{
	if(packet)
	{
		clientIDCount++;

		clientList[clientIDCount] = m_peer->GetIndexFromSystemAddress(packet->systemAddress);

		return clientIDCount;
	}

	return 0;
}

unsigned int NetworkManager::getClientIDFromPacketAddress(void)
{
	unsigned int netID = 0;

	if(packet)
	{
		std::map<unsigned int, int>::iterator itr;

		int index = m_peer->GetIndexFromSystemAddress(packet->systemAddress);

		for(itr = clientList.begin(); itr != clientList.end(); itr++)
		{
			if(itr->second == index)
			{
				netID = itr->first;
			}
		}
	}

	return netID;
}

void NetworkManager::removeClientID(unsigned int clientID)
{
	if(clientList.find(clientID) != clientList.end())
	{
		clientList.erase(clientID);
	}
}

void NetworkManager::fillBitStreamFromPacket(void)
{
	//mainBitStream.Reset();
	inBitStream.Reset();

	if(packet)
		//mainBitStream = RakNet::BitStream((unsigned char*)packet->data, packet->length, false);
		inBitStream = RakNet::BitStream((unsigned char*)packet->data, packet->length, false);
}

void NetworkManager::clearBitSteram(void)
{
	//mainBitStream.Reset();
	outBitStream.Reset();
}

char NetworkManager::readCharFromBitStream(void)
{
	char value;

	//mainBitStream.Read(value);
	inBitStream.Read(value);

	return value;
}

unsigned char NetworkManager::readUCharFromBitStream(void)
{
	unsigned char value;

	//mainBitStream.Read(value);
	inBitStream.Read(value);

	return value;
}

int NetworkManager::readIntFromBitStream(void)
{
	int value;

	//mainBitStream.Read(value);
	inBitStream.Read(value);

	return value;
}

unsigned int NetworkManager::readUIntFromBitStream(void)
{
	unsigned int value;

	//mainBitStream.Read(value);
	inBitStream.Read(value);

	return value;
}

float NetworkManager::readFloatFromBitStream(void)
{
	float value;

	//mainBitStream.Read(value);
	inBitStream.Read(value);

	return value;
}

bool NetworkManager::readBoolFromBitStream(void)
{
	bool value;

	//mainBitStream.Read(value);
	inBitStream.Read(value);

	return value;
}

std::string NetworkManager::readStringFromBitStream(void)
{
	RakNet::RakString rValue;

	//mainBitStream.Read(rValue);
	inBitStream.Read(rValue);

	std::string value(rValue.C_String());

	return value;
}

unsigned long long int NetworkManager::readTimeStampFromBitStream(void)
{
	RakNet::Time value;

	//mainBitStream.Read(value);
	inBitStream.Read(value);

	return value;
}

void NetworkManager::writeCharToBitStream(char value)
{
	//mainBitStream.Write(value);
	outBitStream.Write(value);
}

void NetworkManager::writeUCharToBitStream(unsigned char value)
{
	//mainBitStream.Write(value);
	outBitStream.Write(value);
}

void NetworkManager::writeIntToBitStream(int value)
{
	//mainBitStream.Write(value);
	outBitStream.Write(value);
}

void NetworkManager::writeUIntToBitStream(unsigned int value)
{
	//mainBitStream.Write(value);
	outBitStream.Write(value);
}

void NetworkManager::writeFloatToBitStream(float value)
{
	//mainBitStream.Write(value);
	outBitStream.Write(value);
}

void NetworkManager::writeBoolToBitStream(bool value)
{
	//mainBitStream.Write(value);
	outBitStream.Write(value);
}

void NetworkManager::writeStringToBitStream(char *value)
{
	RakNet::RakString rValue(value);

	outBitStream.Write(rValue);
}

void NetworkManager::writeTimeStampToBitStream(unsigned long long int time)
{
	//mainBitStream.Write(time);
	outBitStream.Write(time);
}

void NetworkManager::writeSystemTimeStampToBitStream(void)
{
	RakNet::Time value = RakNet::GetTime();

	//mainBitStream.Write(value);
	outBitStream.Write(value);
}

void NetworkManager::sendBitStreamToClient(unsigned int clientID, int priority, int reliability, char orderingChannel)
{
	//m_peer->Send(&mainBitStream, (PacketPriority)priority, (PacketReliability)reliability, orderingChannel, getSystemAddressFromClientID(clientID), false);
	m_peer->Send(&outBitStream, (PacketPriority)priority, (PacketReliability)reliability, orderingChannel, getSystemAddressFromClientID(clientID), false);
}

void NetworkManager::sendBitStreamToAll(int priority, int reliability, char orderingChannel)
{
	//m_peer->Send(&mainBitStream, (PacketPriority)priority, (PacketReliability)reliability, orderingChannel, UNASSIGNED_SYSTEM_ADDRESS, true);
	m_peer->Send(&outBitStream, (PacketPriority)priority, (PacketReliability)reliability, orderingChannel, UNASSIGNED_SYSTEM_ADDRESS, true);
}

RakNet::SystemAddress NetworkManager::getSystemAddressFromClientID(unsigned int clientID)
{
	if(clientList.find(clientID) != clientList.end())
	{
		return m_peer->GetSystemAddressFromIndex(clientList[clientID]);
	}

	return 0;
}
