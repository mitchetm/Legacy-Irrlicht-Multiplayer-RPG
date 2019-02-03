// add all the incldues and globals
#include "GameManager.h"
#include "InputManager.h"
#include "GraphicManager.h"
#include "PhysicsManager.h"
#include "SoundManager.h"
#include "NetworkManager.h"
#include "DataBaseManager.h"
#include "Game.h"

#include "StateEngine.h"
#include "SplashState.h"
#include "MainMenuState.h"
#include "MainClientState.h"
#include "MainServerState.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

//main function
int main(int argv, char** argc)
{
	// we but the revision number here
	// this is so we can check the client and server are from the same revision
	unsigned int revision = 64;

	// first, for testing we check if are running as a server or clinet
	// we check if we have any arguments to run the game
	// chek if we have ip and port info also
	bool server = false;
	std::string ip;
	std::string portString; // to store the port as a string first
	unsigned int port;
	unsigned int screenWidth = 800;
	unsigned int screenHieght = 600;
	bool isFullScreen = false;
	unsigned int videoType = 5; // OpenGL

	if(argv == 1) // no arguments, we assume its a client, and connect to localhost on port 6000
	{
		server = false;
		ip = "localhost";
		portString = "6000";
	}
	else
	{
		// first we set the data in case we don't change it
		ip = "localhost";
		portString = "6000";

		// we go one argument at at time

		// we check the first argumet that if starts with a '-' to check its an argument, not a value
		for(int i = 1; i < argv; i++)
		{
			if(argc[i][0] == '-')
			{
				// now we check if its a [c]lient or a [s]erver
				switch(argc[i][1])
				{
				case 'c': // we are a client

					server = false;

					break;

				case 's': // we are a server

					server = true;

					break;

				case 'i': // set the IP adress

					ip = argc[i+1]; // we take the value after it.
					i++; // so we jump an argument name and its value

					break;

				case 'p': // set the port number

					portString = argc[i+1]; // we take the value after it.
					i++; // so we jump an argument name and its value

					break;

				case 'f': // full mode

					isFullScreen = true;
					videoType = 4; // DirectX9
					screenWidth = 1600;
					screenHieght = 1200;

					break;
				}
			}
		}
	}

	// after we are done, we change the port from string to int
	port = atoi(portString.c_str());

	// we print a message to see what are doing
	cout << "running as ";
	if(server)
		cout << "server: ";
	else
		cout << "client: ";
	if(!server)
		cout << ip << "|";
	cout << port << endl;
		
	// The whole application will be broken into 4 main parts
	// 1- Init Eveything
	// 2- Start and Run
	// 3- Handel Error
	// 4- Destory everything and then quit

	try // this is to mark where the errors will come from
	{
		/////////////////////////////
		// Step 1: Init Everything //
		/////////////////////////////

		// Init all classes/managers/etc...
		GraphicManager *gMgr = 0;
		InputManager *iMgr = 0;
		PhysicsManager *pMgr = 0;
		SoundManager *sMgr = 0;
		DataBaseManager *dbMgr = 0;
		if(!server)
		{
			gMgr = GraphicManager::getInstance();
			iMgr = InputManager::getInstance();
			pMgr = PhysicsManager::getInstance();
			sMgr = SoundManager::getInstance();
			dbMgr = DataBaseManager::getInstance();
		}
		GameManager *gameMgr = GameManager::getInstance();
		NetworkManager *netMgr = NetworkManager::getInstance();
		Game *game = Game::getInstance();

		////////////////
		// End Step 1 //
		////////////////

		//////////////////////////
		// Step 2: Start and Run//
		//////////////////////////

		CStateEngine sEng; // we create our FSM
		sEng.Init();

		// Start or run all the managers that needs to be run to work
		// Then start the state manager

		// we add info to the NetwrokManager
		netMgr->setRevisionNumber(revision);
		netMgr->setStoredIP((char*)ip.c_str());
		netMgr->setStoredPort(port);

		if(!server)
		{
			dbMgr->loadDataFromFileList("ourStuff/mainDataFile.txt");

			// we fill in our database
			// we load our main db file that has all the paths for the rest of the tables
			dbMgr->loadTable("ourStuff/data/dbmainList.xml.txt");

			// now we get the path for each main table, and we load them all
			std::vector<std::string> resultList;
			if(dbMgr->getAllRowNumbersFromTable("main_db_list", resultList))
			{
				int listSize = resultList.size();

				for(int i  = 0 ; i < listSize; i++)
				{
					std::string path = dbMgr->getRecordAsString("main_db_list", "path", (char*)resultList[i].c_str());

					dbMgr->loadTable((char*)path.c_str());
				}
			}
			else // there were no rows returne????
			{
				// throw something
			}

			//dbMgr->showXML();

			game->setScreenSize(screenWidth, screenHieght); // we will have to fix this so can load it from a configuation file
			gMgr->run(videoType, screenWidth, screenHieght, 32, isFullScreen, false, false, iMgr); // next time get screen data from another source, 5 = OpenGL, 4 = DirectX9
			gMgr->SetWindowCaption(L"Hunt'nOutlaw");
			sMgr->run();

			sEng.ChangeState(new SplashState()); // we start with spalsh screen
			//sEng.ChangeState(new MainMenuState()); // we plug in the first state: the main menu
			//sEng.ChangeState(new MainClientState()); // we plug in the first state: the main menu

			// figure this out later. We advance Irrlicht timer a bit before we start the states
			for(int i = 0; i < 10; i++)
			{
				gMgr->tick();
				gMgr->getNewDeltaTime();
			}
		}
		else
		{
			sEng.ChangeState(new MainServerState()); // we start with spalsh screen
		}

		while(sEng.IsRunning()) // we keep running till we quit
		{
			sEng.CheckRequests(); // we check here is states were chagned.

			sEng.HandleEvents();
			sEng.Update();
			sEng.Draw();
		}

		sEng.DeInit(); // clear any state we still have

		////////////////
		// End Step 2 //
		////////////////
	}

	//////////////////////////
	// Step 3: Handel Error //
	//////////////////////////

	// If there is any error this part will excute
	// If not then it will be skipped

	catch(char *errorMsg)
	{
		// desplay the error in any way you like
		system("pause");
	}

	////////////////
	// End Step 3 //
	////////////////

	////////////////////////////////
	// Step 4: Destory Everything //
	////////////////////////////////

	// Destory and deInit everything and then quit

	if(!server)
	{
		GraphicManager::getInstance()->killInstance();
		InputManager::getInstance()->killInstance();
		PhysicsManager::getInstance()->killInstance();
		SoundManager::getInstance()->killInstance();
		DataBaseManager::getInstance()->killInstance();
	}
	GameManager::getInstance()->killInstance();
	NetworkManager::getInstance()->killInstance();
	Game::getInstance()->killInstance();

	////////////////
	// End Step 4 //
	////////////////

	return 0;
}