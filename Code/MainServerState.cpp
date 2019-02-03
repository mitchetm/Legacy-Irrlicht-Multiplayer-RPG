#include "MainServerState.h"
#include "OurGameMessages.h"

#include <iostream>

using namespace std;

// a few helper functions to handle lists
void addToList(unsigned int value, std::vector<unsigned int> *list)
{
	list->push_back(value);
}

bool checkInList(unsigned int value, std::vector<unsigned int> *list)
{
	int listSize = list->size();

	for(int i = 0; i < listSize; i++)
	{
		unsigned int comp = list->at(i);

		if(comp == value)
			return true;
	}

	return false;
}

unsigned int getNextValueInList(std::vector<unsigned int> *list) // removes the first one and return it
{
	unsigned int value = list->front();

	return value;
}

bool isListEmpty(std::vector<unsigned int> *list)
{
	return (list->empty());
}

void removeFromList(unsigned int value, std::vector<unsigned int> *list)
{
	int listSize = list->size();

	for(int i = 0; i < listSize; i++)
	{
		unsigned int comp = list->at(i);

		if(comp == value)
		{
			list->erase(list->begin() + i);
			return;
		}
	}
}

MainServerState::MainServerState()
{
	newUsers.clear();
	waitListUsers.clear();
	activeUsers.clear();
	confirmList.clear();
}

MainServerState::~MainServerState()
{
}

void MainServerState::Init(void)
{
	gameMgr = GameManager::getInstance();
	game = Game::getInstance();
	netMgr = NetworkManager::getInstance();

	netMgr->StartServer(netMgr->getStoredPort(), 32);
	system("cls");
	cout << "Server has started" << endl;
}

void MainServerState::DeInit(void)
{
}

void MainServerState::Pause(CStateEngine *engine)
{
}

void MainServerState::Resume(CStateEngine *engine)
{
}

void MainServerState::HandleEvents(CStateEngine *engine)
{

	while(netMgr->getNewPacket2())
	{
		unsigned char idn;
		unsigned int revisionNumber;
		std::string userName;
		unsigned int modelID;
		//unsigned int r;
		//unsigned int g;
		//unsigned int b;
		unsigned int netID;
		unsigned int activeID;
		unsigned int objID;
		float x, y, z, yAngle;
		bool hasMoved, hasTurned, justStopped;
		RakNet::Time timeStamp;
		unsigned int userSize;

		netMgr->fillBitStreamFromPacket();

		idn = netMgr->readUCharFromBitStream();

		// we check first if its starts with a time stamp
		if(idn == ID_TIMESTAMP)
		{
			timeStamp = netMgr->readTimeStampFromBitStream();

			// the next one is the message identifier
			idn = netMgr->readUCharFromBitStream();
		}

		RakNet::RakString message;

		switch(idn)
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			cout << "Another client has disconnected." << endl;

			// we remove the user, and ask the other clients to do so too
			netID = netMgr->getClientIDFromPacketAddress();
			netMgr->removeClientID(netID);
			removeUser(netID);

			break;
		case ID_REMOTE_CONNECTION_LOST:
			cout << "Another client has lost the connection." << endl;

			// we remove the user, and ask the other clients to do so too
			netID = netMgr->getClientIDFromPacketAddress();
			netMgr->removeClientID(netID);
			removeUser(netID);

			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			cout << "Another client has connected." << endl;
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			cout << "Our connection request has been accepted." << endl;
			break;					
		case ID_NEW_INCOMING_CONNECTION:

			////////
			// START  - this is were the connection with a new clinet starts
			////////

			cout << "A connection is incoming." << endl;

			///////////
			// 1st step
			///////////

			cout << "Step 1" << endl;

			// first we add a new netID and store it with client's SystemAddree
			// we save it in the newUsers list so we know it's new
			// Then we send a ID_REQUEST_REVISION with the netID to check the client's version againest ours
			// the netID is just there for identifactions. There is no use for it now

			netID = netMgr->addClientFromPacketAddress();
			addToList(netID, &newUsers);

			netMgr->clearBitSteram();
			netMgr->writeUCharToBitStream((unsigned char)ID_REQUEST_REVISION);
			netMgr->writeUIntToBitStream(netID);

			netMgr->sendBitStreamToClient(netID, HIGH_PRIORITY, RELIABLE_ORDERED, 0);

			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			cout << "The server is full." << endl;
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			cout << "A client has disconnected." << endl;

			// we remove the user, and ask the other clients to do so too
			netID = netMgr->getClientIDFromPacketAddress();
			netMgr->removeClientID(netID);
			removeUser(netID);

			break;
		case ID_CONNECTION_LOST:
			cout << "A client lost the connection." << endl;

			// we remove the user, and ask the other clients to do so too
			netID = netMgr->getClientIDFromPacketAddress();
			netMgr->removeClientID(netID);
			removeUser(netID);

			break;

		case ID_SEND_CHAT:

			// we display it for loging reasons
			{
				netID = netMgr->getClientIDFromPacketAddress();

				std::string sendMessage;
				sendMessage = netMgr->readStringFromBitStream();

				cout << "(CHAT) " << sendMessage << endl;
				cout << "\a";

				netMgr->clearBitSteram();
				netMgr->writeUCharToBitStream((unsigned char)ID_SEND_CHAT);
				netMgr->writeStringToBitStream((char*)sendMessage.c_str());

				for(int i = 0; i < activeUsers.size(); i++)
				{
					if(netID != activeUsers[i])
					{
						netMgr->sendBitStreamToClient(activeUsers[i], HIGH_PRIORITY, RELIABLE_ORDERED, 0);
					}
				}
			}

			break;

		case ID_INFO_REVISION:

			///////////
			// 3rd step
			///////////

			cout << "Step 3" << endl;

			// here we receive the client's revision number
			// if its not a match, we till it so and remove from newUser list
			// if it's a match, we move the ID to the waiting list
			// after that we check and see if there is anyone waiting in the waiting list
			// if there isn't anyone, we start proccsing this one by asking for its log in and character data
			// if there are others, then we we skip and wait our turn

			revisionNumber = netMgr->readUIntFromBitStream();
			netID = netMgr->readUIntFromBitStream();

			netMgr->clearBitSteram();

			if(netMgr->getRevisionNumber() != revisionNumber)
			{
				netMgr->writeUCharToBitStream((unsigned char)ID_REVISION_BAD);
				removeFromList(netID, &newUsers);

				// we set the last argument to false to send to our client, if its true then it will be send to everyone but our client
				netMgr->sendBitStreamToClient(netID, HIGH_PRIORITY, RELIABLE_ORDERED, 0);

				netMgr->removeClientID(netID);

				cout << "Go to step 4" << endl;
			}
			else
			{
				removeFromList(netID, &newUsers);
				addToList(netID, &waitListUsers);

				// we create the object for this character
				objID = gameMgr->createGameObject();
				gameMgr->addGameComponent(objID, ComponentType::NAME_COMP);
				gameMgr->addGameComponent(objID, ComponentType::BODY_VIEW_COMP);
				gameMgr->addGameComponent(objID, ComponentType::HP_COMP);
				gameMgr->addGameComponent(objID, ComponentType::LOCK_ON_COMP);
				// and don't forget to add the netID for this object
				gameMgr->addResourceID(objID, "netID", netID);

				netObjectList[netID] = objID;

				if(waitListUsers.size() == 1) // there is only us
				{
					// we request user and character data
					netMgr->writeUCharToBitStream((unsigned char)ID_REQUEST_USER_DATA);
					netMgr->writeUIntToBitStream(netID);

					// we set the last argument to false to send to our client, if its true then it will be send to everyone but our client
					netMgr->sendBitStreamToClient(netID, HIGH_PRIORITY, RELIABLE_ORDERED, 0);

					cout << "Go to step 5" << endl;
				}
				// else, do nothing. wait till your turn comes
			}


			break;

		case ID_INFO_USER_DATA:

			///////////
			// 6th step
			///////////

			cout << "Step 6" << endl;

			// we received the user's data and character data
			// we store the data
			// we create the character (server wise)
			// we tell the user that his charactre has been created and his ready to move on to the game wrold

			netID = netMgr->readUIntFromBitStream();
			userName = netMgr->readStringFromBitStream();
			modelID = netMgr->readUIntFromBitStream();
			//r = netMgr->readUIntFromBitStream();
			//g = netMgr->readUIntFromBitStream();
			//b = netMgr->readUIntFromBitStream();

			createCharacter(netID, (char*)userName.c_str(), modelID);

			netMgr->clearBitSteram();
			netMgr->writeUCharToBitStream((unsigned char)ID_CHARACTER_READY);
			netMgr->writeUIntToBitStream(netID);

			// we set the last argument to false to send to our client, if its true then it will be send to everyone but our client
			netMgr->sendBitStreamToClient(netID, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
			cout << "Go to step 7" << endl;

			break;

		case ID_CONFIRM_CHARACTER:

			///////////
			// 8th step
			///////////

			cout << "Step 8" << endl;

			// the client has moved to MainClientState
			// we must send all the current users data to it
			// don't forget to send the netID for the user (fix)

			netID = netMgr->readUIntFromBitStream();

			netMgr->clearBitSteram();
			netMgr->writeUCharToBitStream((unsigned char)ID_ADD_CURRENT_USERS);
			netMgr->writeUIntToBitStream(netID);

			userSize = activeUsers.size();
			netMgr->writeUIntToBitStream(userSize);

			// we check how many active users we have
			if(userSize > 0)
			{
				for(int i = 0; i < userSize; i++)
				{
					unsigned int memberID = activeUsers[i];
					objID = netObjectList[memberID]; // we get the object id
					float mx, my, mz;
					gameMgr->getPosition(objID, mx, my, mz);
					float mxAngle, myAngle, mzAngle;
					gameMgr->getRotation(objID, mxAngle, myAngle, mzAngle);
					std::string mName = gameMgr->getName(objID);
					unsigned int mmodelID = gameMgr->getBodyViewID(objID);
					//unsigned int mr = gameMgr->getRed(objID);
					//unsigned int mg = gameMgr->getGreen(objID);
					//unsigned int mb = gameMgr->getBlue(objID);
					// we also send the HP and Max

					netMgr->writeUIntToBitStream(memberID);
					netMgr->writeFloatToBitStream(mx);
					netMgr->writeFloatToBitStream(my);
					netMgr->writeFloatToBitStream(mz);
					netMgr->writeFloatToBitStream(myAngle);
					netMgr->writeStringToBitStream((char*)mName.c_str());
					netMgr->writeUIntToBitStream(mmodelID);
					//netMgr->writeUIntToBitStream(mr);
					//netMgr->writeUIntToBitStream(mg);
					//netMgr->writeUIntToBitStream(mb);
				}
			}

			netMgr->sendBitStreamToClient(netID, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
			cout << "Go to step 9" << endl;

			break;

		case ID_CONFIRM_CURRENT_USERS:

			///////////
			// 10th step
			///////////

			cout << "Step 10" << endl;

			// OK, we sent the list of active users to the new guy, and he confirms
			// now we take the new guy's info and we send them to everyone else

			netID = netMgr->readUIntFromBitStream();

			// first we check if we do have active users, we could be the first

			// if we are the first, then no need to finish the process
			// we activate the new guy so he can start sending updates
			// we add him to the active list here so we can send him other players updates

			// after we are done, we check our waiting list
			// if there is/are someone(s) waiting, then we start the process with them too

			if(isListEmpty(&activeUsers)) // we have no active users
			{
				removeFromList(netID, &waitListUsers);
				addToList(netID, &activeUsers);

				netMgr->clearBitSteram();
				netMgr->writeUCharToBitStream((unsigned char)ID_YOU_ARE_ACTIVE);

				netMgr->sendBitStreamToClient(netID, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
				cout << "Go to step 13" << endl;

				// we also check if has someone in the waiting list
				if(!isListEmpty(&waitListUsers))
				{
					netID = getNextValueInList(&waitListUsers);

					netMgr->clearBitSteram();
					netMgr->writeUCharToBitStream((unsigned char)ID_REQUEST_USER_DATA);
					netMgr->writeUIntToBitStream(netID);

					netMgr->sendBitStreamToClient(netID, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
					cout << "Go back to step 5" << endl;
				}

			}
			else // we do have active users
			{
				// we put all the new guy's data in a bitstream first

				netMgr->clearBitSteram();

				netMgr->writeUCharToBitStream((unsigned char)ID_REQUST_ADD_NEW_USER);

				objID = netObjectList[netID];
				float x, y, z;
				gameMgr->getPosition(objID, x, y, z);
				float xAngle, yAngle, zAngle;
				gameMgr->getRotation(objID, xAngle, yAngle, zAngle);

				netMgr->writeUIntToBitStream(netID);
				netMgr->writeFloatToBitStream(x);
				netMgr->writeFloatToBitStream(y);
				netMgr->writeFloatToBitStream(z);
				netMgr->writeFloatToBitStream(yAngle);
				netMgr->writeStringToBitStream((char*)gameMgr->getName(objID).c_str());
				netMgr->writeUIntToBitStream(gameMgr->getBodyViewID(objID));
				//netMgr->writeUIntToBitStream(gameMgr->getRed(objID));
				//netMgr->writeUIntToBitStream(gameMgr->getGreen(objID));
				//netMgr->writeUIntToBitStream(gameMgr->getBlue(objID));

				// now we add our new guy to the confirm list
				// he will not be activated till every active user replies that he was added
				confirmList[netID] = std::vector<unsigned int>();

				// now we go throught every active user
				// we add them to the confirm list
				// then we send the bit stream

				int listSize = activeUsers.size();

				for(int i = 0; i < listSize; i++)
				{
					activeID = activeUsers[i];
					addToList(activeID, &confirmList[netID]);

					// now we send
					netMgr->sendBitStreamToClient(activeID, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
					cout << "Go to step 11: " << i << endl;
				}
			}

			break;

		case ID_CONFIRM_NEW_USER:

			///////////
			// 12th step
			///////////

			cout << "Step 12" << endl;

			// we have receive confirmation that a new guy in the waiting list has been added in one of the active users
			// we remove the active user from the confirm list
			// if the cofirm list is empty, then all active users have added the new guy
			// we activate the new guy, then we start the process with another new guy

			activeID = netMgr->readUIntFromBitStream();
			netID = netMgr->readUIntFromBitStream();

			removeFromList(activeID, &confirmList[netID]);

			// now we check if everyone has confirmed

			if(confirmList[netID].empty())
			{
				// we remove the new guy from the confirm list
				confirmList.erase(netID);

				// now we move the new guy from the waiting list to the active list
				removeFromList(netID, &waitListUsers);
				addToList(netID, &activeUsers);

				// we activate the new guy
				netMgr->clearBitSteram();
				netMgr->writeUCharToBitStream((unsigned char)ID_YOU_ARE_ACTIVE);

				netMgr->sendBitStreamToClient(netID, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
				cout << "Go to step 13" << endl;

				// now we check if there is someone else waiting in the waiting list
				// if there is, we send them ID_REQUEST_USER_DATA to start proccessing them too
				if(!isListEmpty(&waitListUsers))
				{
					netID = getNextValueInList(&waitListUsers);

					netMgr->clearBitSteram();
					netMgr->writeUCharToBitStream((unsigned char)ID_REQUEST_USER_DATA);
					netMgr->writeUIntToBitStream(netID);

					netMgr->sendBitStreamToClient(netID, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
					cout << "Go back to step 5" << endl;
				}
			}

			break;

		case ID_UPDATE:

			// we receive updates from all active users here
			// we take the data and we snyc it with the server's data
			// then we send the data to the rest of the active users, except the original sender

			netID = netMgr->readUIntFromBitStream();
			objID = netObjectList[netID];

			// we check if the client moved
			hasMoved = netMgr->readBoolFromBitStream();

			if(hasMoved)
			{
				x = netMgr->readFloatFromBitStream();
				y = netMgr->readFloatFromBitStream();
				z = netMgr->readFloatFromBitStream();

				gameMgr->setPosition(objID, x, y, z);

				justStopped = netMgr->readBoolFromBitStream();
			}

			// we check if the client turned
			hasTurned = netMgr->readBoolFromBitStream();

			if(hasTurned)
			{
				yAngle = netMgr->readFloatFromBitStream();
				float xAngle = 0;
				float zAngle = 0;

				gameMgr->setRotation(objID, xAngle, yAngle, zAngle);
			}

			userSize = activeUsers.size();

			if(userSize > 0)
			{
				netMgr->clearBitSteram();

				netMgr->writeUCharToBitStream((unsigned char)ID_UPDATE);

				netMgr->writeUIntToBitStream(netID);

				netMgr->writeBoolToBitStream(hasMoved);

				if(hasMoved)
				{
					netMgr->writeFloatToBitStream(x);
					netMgr->writeFloatToBitStream(y);
					netMgr->writeFloatToBitStream(z);

					netMgr->writeBoolToBitStream(justStopped);
				}

				netMgr->writeBoolToBitStream(hasTurned);

				if(hasTurned)
				{
					netMgr->writeFloatToBitStream(yAngle);
				}
			}

			for(int i = 0; i < userSize; i++)
			{
				activeID = activeUsers[i];

				if(netID != activeID)
				{
					netMgr->sendBitStreamToClient(activeID, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
				}
			}

			break;

		case ID_REQUEST_LOCK_ON: // a client wants to lockon to someone. We send the target HP also

			netID = netMgr->readUIntFromBitStream();
			activeID = netMgr->readUIntFromBitStream();

			if(activeID != 0) // if zero then we are requesting to remove the lock on
			{
				// make sure that target exsits
				if(checkInList(activeID, &activeUsers))
				{
					objID = netObjectList[netID];
					unsigned int actObjID = netObjectList[activeID];

					// we check first we already have a target
					unsigned int oldTargetID = gameMgr->getTarget(objID);
					if(oldTargetID != 0) // we alread have a target. We realse it first
					{
						gameMgr->setTarget(objID, 0);
						gameMgr->removeHunter(oldTargetID, objID);
					}

					gameMgr->setTarget(objID, actObjID);
					gameMgr->addHunter(actObjID, objID);

					// now we send the target ID, and its MaxHP and HP
					netMgr->clearBitSteram();

					netMgr->writeUCharToBitStream((unsigned int) ID_ACCEPT_LOCK_ON);

					netMgr->writeUIntToBitStream(activeID);
					netMgr->writeIntToBitStream(gameMgr->getMaxHP(actObjID));
					netMgr->writeIntToBitStream(gameMgr->getHP(actObjID));

					netMgr->sendBitStreamToClient(netID, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
				}
				else
				{
					// the target does not exsit, we ignor
				}
			}
			else // we want to remove our lock on
			{
				objID = netObjectList[netID];
				unsigned int actObjID = gameMgr->getTarget(objID);

				gameMgr->setTarget(objID, 0);
				gameMgr->removeHunter(actObjID, objID);
			}

			break;

		case ID_UPDATE_HP:
			{
				netID = netMgr->readUIntFromBitStream();
				int hp = netMgr->readIntFromBitStream();

				objID = netObjectList[netID];

				gameMgr->setHP(objID, hp);

				// now we check if we have others targteing this character, or if we are part of a party
				// those are the ones that are concerend with the HP change

				netMgr->clearBitSteram();

				netMgr->writeUCharToBitStream((unsigned char) ID_UPDATE_HP);
				netMgr->writeUIntToBitStream(netID);
				netMgr->writeIntToBitStream(hp);

				// first we check hunters
				int listSize = gameMgr->getHunterListSize(objID);
				for(int i = 0; i < listSize; i++)
				{
					unsigned int huntObjID = gameMgr->getHunterByIndex(objID, i);
					unsigned int huntNetID = gameMgr->getResourceID(huntObjID, "netID");

					netMgr->sendBitStreamToClient(huntNetID, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
				}

				// then we send to team mates
			}
			break;

		case ID_START_ATTACK: // someone attacked
			{
				netID = netMgr->readUIntFromBitStream();
				unsigned int objID = netObjectList[netID];

				float x, y, z, yAngle;
				float xAngle = 0, zAngle = 0;
				unsigned int ap;

				x = netMgr->readFloatFromBitStream();
				y = netMgr->readFloatFromBitStream();
				z = netMgr->readFloatFromBitStream();
				yAngle = netMgr->readFloatFromBitStream();
				ap = netMgr->readUIntFromBitStream();

				gameMgr->setPosition(objID, x, y, z);
				gameMgr->setRotation(objID, xAngle, yAngle, zAngle);
				gameMgr->setActionID(objID, 1);// for now, 1 = regular attack
				gameMgr->setActionPower(objID, ap); // this will modify the attack power

				netMgr->clearBitSteram();

				netMgr->writeUCharToBitStream((unsigned char) ID_TIMESTAMP);
				netMgr->writeTimeStampToBitStream(timeStamp);

				netMgr->writeUCharToBitStream((unsigned char) ID_ATTACK);
				netMgr->writeUIntToBitStream(netID);
				netMgr->writeFloatToBitStream(x);
				netMgr->writeFloatToBitStream(y);
				netMgr->writeFloatToBitStream(z);
				netMgr->writeFloatToBitStream(yAngle);
				netMgr->writeUIntToBitStream(ap);

				int listSize = activeUsers.size();

				for(int i = 0; i < listSize; i++)
				{
					activeID = activeUsers[i];

					if(activeID != netID)
					{
						netMgr->sendBitStreamToClient(activeID, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
					}
				}
			}
			break;

		case ID_DEAD: // someone is dead
			{
				netID = netMgr->readUIntFromBitStream();
				unsigned int objID = netObjectList[netID];

				float x, y, z, yAngle;
				float xAngle = 0, zAngle = 0;

				x = netMgr->readFloatFromBitStream();
				y = netMgr->readFloatFromBitStream();
				z = netMgr->readFloatFromBitStream();
				yAngle = netMgr->readFloatFromBitStream();

				gameMgr->setPosition(objID, x, y, z);
				gameMgr->setRotation(objID, xAngle, yAngle, zAngle);

				netMgr->clearBitSteram();

				netMgr->writeUCharToBitStream((unsigned char) ID_TIMESTAMP);
				netMgr->writeTimeStampToBitStream(timeStamp);

				netMgr->writeUCharToBitStream((unsigned char) ID_DEAD);
				netMgr->writeUIntToBitStream(netID);
				netMgr->writeFloatToBitStream(x);
				netMgr->writeFloatToBitStream(y);
				netMgr->writeFloatToBitStream(z);
				netMgr->writeFloatToBitStream(yAngle);

				int listSize = activeUsers.size();

				for(int i = 0; i < listSize; i++)
				{
					activeID = activeUsers[i];

					if(activeID != netID)
					{
						netMgr->sendBitStreamToClient(activeID, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
					}
				}

			}
			break;

		case ID_RESPAWN: // someone is spawning
			{
				// this is very simple. just send the ID_RESPAWN to eveyone
				// we unsed all the targets and hunters
				netID = netMgr->readUIntFromBitStream();
				unsigned int hp = netMgr->readUIntFromBitStream();
				unsigned int objID = netObjectList[netID];

				gameMgr->setTarget(objID, 0);
				gameMgr->clearHunterList(objID);
				gameMgr->setHP(objID, hp);

				netMgr->clearBitSteram();

				netMgr->writeUCharToBitStream((unsigned char) ID_RESPAWN);
				netMgr->writeUIntToBitStream(netID);

				int listSize = activeUsers.size();

				for(int i = 0; i < listSize; i++)
				{
					activeID = activeUsers[i];

					if(activeID != netID)
					{
						netMgr->sendBitStreamToClient(activeID, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
					}
				}

			}
			break;

		case ID_BLOCK_START: // someone start blocking
			{
				netID = netMgr->readUIntFromBitStream();
				unsigned int objID = netObjectList[netID];

				float x, y, z, yAngle;
				float xAngle = 0, zAngle = 0;

				x = netMgr->readFloatFromBitStream();
				y = netMgr->readFloatFromBitStream();
				z = netMgr->readFloatFromBitStream();
				yAngle = netMgr->readFloatFromBitStream();

				gameMgr->setPosition(objID, x, y, z);
				gameMgr->setRotation(objID, xAngle, yAngle, zAngle);

				netMgr->clearBitSteram();

				netMgr->writeUCharToBitStream((unsigned char) ID_TIMESTAMP);
				netMgr->writeTimeStampToBitStream(timeStamp);

				netMgr->writeUCharToBitStream((unsigned char) ID_BLOCK_START);
				netMgr->writeUIntToBitStream(netID);
				netMgr->writeFloatToBitStream(x);
				netMgr->writeFloatToBitStream(y);
				netMgr->writeFloatToBitStream(z);
				netMgr->writeFloatToBitStream(yAngle);

				int listSize = activeUsers.size();

				for(int i = 0; i < listSize; i++)
				{
					activeID = activeUsers[i];

					if(activeID != netID)
					{
						netMgr->sendBitStreamToClient(activeID, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
					}
				}
			}
			break;

		case ID_BLOCK_END: // someone stopped blocking
			{
				netID = netMgr->readUIntFromBitStream();
				unsigned int objID = netObjectList[netID];

				netMgr->clearBitSteram();

				netMgr->writeUCharToBitStream((unsigned char) ID_TIMESTAMP);
				netMgr->writeTimeStampToBitStream(timeStamp);

				netMgr->writeUCharToBitStream((unsigned char) ID_BLOCK_END);
				netMgr->writeUIntToBitStream(netID);

				int listSize = activeUsers.size();

				for(int i = 0; i < listSize; i++)
				{
					activeID = activeUsers[i];

					if(activeID != netID)
					{
						netMgr->sendBitStreamToClient(activeID, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
					}
				}
			}
			break;

		default:
			cout << "Message with identifier " << (int)idn << " has arrived." << endl;
			break;
		}

		netMgr->DestroyPacket2();
	}

}

void MainServerState::Update(CStateEngine *engine)
{
	//// we check any new messages need to be sent
	//// in the future this should be in a seprate thread

}

void MainServerState::Draw(CStateEngine *engine)
{
}

void MainServerState::createCharacter(unsigned int netID, char *userName, unsigned int modelNumber)
{
	unsigned int objID = netObjectList[netID];
	gameMgr->addGameComponent(objID, ComponentType::HP_COMP);
	gameMgr->addGameComponent(objID, ComponentType::ACTION_COMP);

	gameMgr->setName(objID, userName);
	gameMgr->setBodyViewID(objID, modelNumber);
	//gameMgr->setRed(objID, r);
	//gameMgr->setGreen(objID, g);
	//gameMgr->setBlue(objID, b);

	// we also set the HP. for now we set it to 100
	gameMgr->setMaxHP(objID, 100);
	gameMgr->setHP(objID, 100);
}

void MainServerState::removeUser(unsigned int userID)
{
	removeFromList(userID, &newUsers);
	removeFromList(userID, &waitListUsers);
	removeFromList(userID, &activeUsers);

	if(confirmList.find(userID) != confirmList.end())
		confirmList.erase(confirmList.find(userID)->first);
	// TODO: we check if the deleted clients is not part of a confirmation list

	// now we send the remove command to all users, either active or waiting

	netMgr->clearBitSteram();
	netMgr->writeUCharToBitStream((unsigned char)ID_REMOVE_USER);
	netMgr->writeUIntToBitStream(userID);

	int listSize = activeUsers.size();
	for(int i = 0; i < listSize; i++)
		netMgr->sendBitStreamToClient(activeUsers[i], HIGH_PRIORITY, RELIABLE_ORDERED, 0);

	listSize = waitListUsers.size();
	for(int i = 0; i < listSize; i++)
		netMgr->sendBitStreamToClient(waitListUsers[i], HIGH_PRIORITY, RELIABLE_ORDERED, 0);

	if(netObjectList.find(userID) != netObjectList.end())
	{
		//// we make sure that no one is locked on to a someone who will be removed
		unsigned int objID = netObjectList[userID];
		//int listSize = gameMgr->getHunterListSize(objID);
		//for(int i = 0; i < listSize; i++)
		//{
		//	unsigned int hunterID = gameMgr->getHunterByIndex(objID, i);

		//	gameMgr->setTarget(hunterID, 0);
		//}

		gameMgr->deleteGameObject(objID);
		netObjectList.erase(userID);
	}
}
