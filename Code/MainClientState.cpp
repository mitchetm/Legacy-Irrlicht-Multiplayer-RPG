#include "MainClientState.h"
#include "MainMenuState.h"
#include "OurGameMessages.h"

#include <iostream>

using namespace std;

MainClientState::MainClientState()
{
	mainThemeChannleID = 0;
}

MainClientState::~MainClientState()
{
}

void MainClientState::Init(void)
{
	gameMgr = GameManager::getInstance();
	game = Game::getInstance();
	gMgr = GraphicManager::getInstance();
	iMgr = InputManager::getInstance();
	dbMgr = DataBaseManager::getInstance();
	netMgr = NetworkManager::getInstance();
	sMgr = SoundManager::getInstance();

	// varibales to get database data
	std::string modelPath;
	std::vector<std::string> texturePathList;
	float yOffset;
	float rotationOffset;
	float scaleOffset;
	int start, end;
	float frame_speed;
	bool loop;

	netObjectList.clear();

	userName = game->getUserName();
	isNetActive = false; // we are not active on the network. We don't send or receive any updates yet
	netUpdateTimer = 0.1; // there was some sorta data cloge that kept one client from letting the other clients send anything. We try to fight this by only net updating 10 times a sec

	hasMoved = false; // we need these so we don't have to send updates unless they have moved or turned
	hasTurned = false;
	justStopped = false;

	gMgr->setBackgroundColor(255, 125, 125, 125);
	
	Inventory = gMgr->DrawInventory();
	gMgr->InvVisible(false, Inventory);
	isInventory = false;
	Slot = gMgr->CreateImagegui
	// draw the chat box area

	creatChatBox();

	// remove later, this is so we can see the mouse position and delta position
	mouseStaticID = gMgr->createStaticText(0, 0, 600, 100);


	// for testing the cube moving. Remove latre
	moveForward = 0;
	moveSide = 0;
	maxXAngle = 70;
	minXAngle = -25;

	speed = 100;

	cameraFarOut = 70;

	// we replace our old code with GameManager code

	// we create a game object for our main character
	myID = gameMgr->createGameObject();
	// we add components to our game object
	gameMgr->addGameComponent(myID, ComponentType::NAME_COMP);
	gameMgr->addGameComponent(myID, ComponentType::MOVEMENT_COMP);
	gameMgr->addGameComponent(myID, ComponentType::BODY_VIEW_COMP);
	gameMgr->addGameComponent(myID, ComponentType::HP_COMP);
	gameMgr->addGameComponent(myID, ComponentType::MP_COMP);
	gameMgr->addGameComponent(myID, ComponentType::AP_COMP);
	gameMgr->addGameComponent(myID, ComponentType::ACTION_COMP);
	gameMgr->addGameComponent(myID, ComponentType::LOCK_ON_COMP);

	// now for our camera
	cameraID = gameMgr->createGameObject();

	unsigned int testObjectNameID;
	unsigned int bodyViewID;
	//unsigned int r;
	//unsigned int g;
	//unsigned int b;

	//game->getCubeColor(r, g, b);
	//gameMgr->setRed(myID, r);
	//gameMgr->setGreen(myID, g);
	//gameMgr->setBlue(myID, b);

	bodyViewID = game->getBodyViewID();
	gameMgr->setBodyViewID(myID, bodyViewID);

	getModelAndTextureFromDB(bodyViewID, modelPath, texturePathList, yOffset, rotationOffset, scaleOffset);
	getAnimationInfoFromDB(bodyViewID, "stand", start, end, frame_speed, loop);

	gameMgr->setYOffset(myID, yOffset);
	gameMgr->setRotationOffset(myID, rotationOffset);
	cout << "check 3" << endl;

	testObjectID = gMgr->createAnimatedMesh((char*)modelPath.c_str());
	int listSize = texturePathList.size();
	for(int i = 0; i < listSize; i++)
	{
		gMgr->setNodeTexture(testObjectID, i, (char*)texturePathList[i].c_str());
	}
	gMgr->setAnimationFrameLoop(testObjectID, start, end);
	gMgr->setAnimationFrameSpeed(testObjectID, frame_speed);
	gMgr->setAnimationLoop(testObjectID, loop);
	gMgr->setNodePosition(testObjectID, 0, yOffset, 0);
	//gMgr->setNodeScale(testObjectID, 3.5, 3.5, 3.5);
	gMgr->setNodeScale(testObjectID, scaleOffset, scaleOffset, scaleOffset);
	//Meta = gMgr->MetaSelector();
	//TS = gMgr->NodeAnimator();

	//gMgr->AddAnimatorToMesh(Meta, testObjectID, TS);
	testObjectNameID = gMgr->create3DText((char*)dbMgr->getDataAsString("test_font").c_str(), (char*)userName.c_str());
	gMgr->setNodePosition(testObjectNameID, 0, 10, 0);
	gMgr->setNodeParent(testObjectID, testObjectNameID);
	gameMgr->setName(myID, game->getUserName());
	gameMgr->addResourceID(myID, "bodyID", testObjectID);
	gameMgr->addResourceID(myID, "nameID", testObjectNameID);
	cout << "check 4" << endl;

	// we set our HP
	gameMgr->setMaxHP(myID, 100); // no need to set MinHP. it zero be default
	gameMgr->setHP(myID, 100);
	// we also set our AP
	gameMgr->setMaxAP(myID, 100);
	gameMgr->setAP(myID, 100);

	// set state player state
	// 1 = stop
	// 2 = walking/running
	// 3 = attacking
	// 4 = dead
	// 5 = start blocking
	// 6 = blocking
	// 7 = stop blocking
	gameMgr->setState(myID, 1);

	// load from irr file
	WeirdMap = gMgr->loadmap("Map1.irr", 0);
	gMgr->CreateDefaultSkyBox();

	// let try seting up the ninjas golems
	texturePathList.clear();
	getModelAndTextureFromDB(1, modelPath, texturePathList, yOffset, rotationOffset, scaleOffset);
	getAnimationInfoFromDB(1, "attack_1", start, end, frame_speed, loop);

	golem1ID = gMgr->createAnimatedMesh((char*)modelPath.c_str());
	listSize = texturePathList.size();
	for(int i = 0; i < listSize; i++)
	{
		gMgr->setNodeTexture(golem1ID, i, (char*)texturePathList[i].c_str());
	}
	gMgr->setNodeScale(golem1ID, 200, 200, 200);
	gMgr->setAnimationFrameSpeed(golem1ID, 0.2);
	gMgr->setAnimationFrameLoop(golem1ID, start, end);
	gMgr->setNodePosition(golem1ID, 700, -20, 3000);
	gMgr->setNodeRotation(golem1ID, 0, -90, 0);


	// now for golem 2
	texturePathList.clear();
	getModelAndTextureFromDB(2, modelPath, texturePathList, yOffset, rotationOffset, scaleOffset);
	getAnimationInfoFromDB(2, "attack_1", start, end, frame_speed, loop);

	// I don't want both golems to start the animation from the same frame. I'm going to let the second golem to start from the middel of the animation
	int secondFramStart = start + (end - start) / 2;

	golem2ID = gMgr->createAnimatedMesh((char*)modelPath.c_str());
	listSize = texturePathList.size();
	for(int i = 0; i < listSize; i++)
	{
		gMgr->setNodeTexture(golem2ID, i, (char*)texturePathList[i].c_str());
	}
	gMgr->setNodeScale(golem2ID, 200, 200, 200);
	gMgr->setAnimationFrameSpeed(golem2ID, 0.2);
	gMgr->setAnimationFrameLoop(golem2ID, start, end);
	gMgr->setAnimationCurrentFrame(golem2ID, secondFramStart);
	gMgr->setNodePosition(golem2ID, -700, -20, 3000);
	gMgr->setNodeRotation(golem2ID, 0, 90, 0);
 
	arenaZLimit = 2000;
	inArena = false;
	soundFadeSpeed = 0.3;
	soundVolume = 1;

	isLockOnDown = false;
	isAttackDown = false;
	isHPupDown = false;
	isHPlowDown = false;

	// particle test
	unsigned int partID = gMgr->createParticle();
	gMgr->createParticlePointEmitter(partID, 0, 0.006, 0, 5, 10, 255, 255, 255, 255, 255, 255, 255, 255, 5000, 5000, 20, 20, 20, 20, 20);
	gMgr->addParticleFadeOutAffector(partID, 0, 0, 0, 0, 5000);
	gMgr->setNodeTexture(partID, 0, "ourStuff/smoke2.jpg");
	gMgr->setNodeMaterialType(partID, 12); // EMT_TRANSPARENT_ADD_COLOR = dark area is transparnt
	gMgr->setNodeMaterialFlag(partID, 8, false); // EMF_LIGHTING = control is lighted
	gMgr->setNodePosition(partID, 0, 5, 10);

	// billboard test
	unsigned int billID = gMgr->createBillboard();
	gMgr->setBillboardSize(billID, 20, 20);
	gMgr->setBillBoardColor(billID, 255, 255, 255, 255, 255, 255, 255, 255);
	gMgr->setNodeTexture(billID, 0, "ourStuff/fireball.bmp");
	gMgr->setNodeMaterialType(billID, 12); // EMT_TRANSPARENT_ADD_COLOR = dark area is transparnt
	gMgr->setNodeMaterialFlag(billID, 8, false); // EMF_LIGHTING = control is lighted
	gMgr->setNodePosition(billID, 0, 5, 30);

	// set up shooting fireball
	maxFireBalls = 10;
	currentNumberOfFireBalls = 0;
	fireBallSpeed = 300;
	fireBallCoolDown = 0.5;
	fireBallReady = fireBallCoolDown;
	fireBallLifeSpan = 10;
	isFireDown = false;

	isChatBoxActive = false;
	enterKeyDown = false;

	lockOnChange = false;

	exitMessageBoxID = 0;
	holdingESC = false;

	apRunLimitValue = 2;
	apRunLimit = apRunLimitValue;
	apDecreaseSpeedValue = 1.0 / (100.0 / 6.0); // want to lose 100 AP in 6 seconds
	apDecreaseSpeed = apDecreaseSpeedValue;

	apBlockLimitValue = 1;
	apBlockLimit = apBlockLimitValue;
	apIncreaseSpeedValue = 1.0 / (100.0 / 10.0); // want to gain 1000 AP in 10 seconds
	apIncreaseSpeed = apIncreaseSpeedValue;

	frezz = false;
	frezzIncreaseSpeedValue = 1.0 / (100.0 / 50.0); // gain 20 AP in 10 seconds
	frezzIncreaseSpeed = frezzIncreaseSpeedValue;
}

void MainClientState::DeInit(void)
{
	netObjectList.clear();

	//dbMgr->clearData();
	netMgr->shutDown();
	gMgr->clearGUI();
	gMgr->clearSceneManager();

	sMgr->playSound(mainThemeChannleID, dbMgr->getDataAsInt("main_theme_id"));
	sMgr->setVolum(mainThemeChannleID, 1);

	gameMgr->clearAllGameObjects();
	gameMgr->clearAllGameComponents();
}

void MainClientState::Pause(CStateEngine *engine)
{
}

void MainClientState::Resume(CStateEngine *engine)
{
}

void MainClientState::HandleEvents(CStateEngine *engine)
{
	// ESC does one of the following in sequnese
	// if we chat box active, we unactivated it
	// if we are locked on, we unlock
	// we show the exit message box
	if(iMgr->getKeyState(KEY_ESCAPE) && !holdingESC)
	{
		//engine->RequestChangeState(new MainMenuState());
		holdingESC = true;

		if(isChatBoxActive)
		{
			gMgr->removeGUIFocus(chatEditBoxID);
			isChatBoxActive = false;
		}
		else if(gameMgr->getTarget(myID) != 0) // we are locked on
		{
			lockOnTarget(0);
			lockOnChange = true;
		}
		else if(exitMessageBoxID == 0) // exit message box is off
		{
			exitMessageBoxID = gMgr->createMessageBox(L"Quit", L"Would you like to exit to the main menu?", EMBF_OK | EMBF_CANCEL);
		}
		else if(exitMessageBoxID != 0) // exit message box is on
		{
			//gMgr->sendGUIEvent(0, exitMessageBoxID, irr::gui::EGET_ELEMENT_CLOSED);
			
			// pressing ESC again while the message box is on will remove it anyways
			// TODO: fix a big that crashes the game when deleting the message box manually
			exitMessageBoxID = 0;
		}
	}
	if(!iMgr->getKeyState(KEY_ESCAPE) && holdingESC)
	{
		holdingESC = false;
	}

	// we check for some GUI events

	// we check our exit message box
	// we need to check both the OK and CANACEL buttons
	// if OK, we go back to the main menu
	unsigned int numberOfMessageBoxOK = iMgr->getNumberOfElementsForThisEvent(irr::gui::EGET_MESSAGEBOX_OK);
	if(numberOfMessageBoxOK > 0)
	{
		for(int i = 0; i < numberOfMessageBoxOK; i++)
		{
			if(iMgr->getElementFromEventByIndex(irr::gui::EGET_MESSAGEBOX_OK, i) == exitMessageBoxID)
			{
				engine->RequestChangeState(new MainMenuState());
			}
		}
	}
	// if CANACEL, we set our exitMessageBoxID to 0
	unsigned int numberOfMessageBoxCancel = iMgr->getNumberOfElementsForThisEvent(irr::gui::EGET_MESSAGEBOX_CANCEL);
	if(numberOfMessageBoxCancel > 0)
	{
		for(int i = 0; i < numberOfMessageBoxCancel; i++)
		{
			if(iMgr->getElementFromEventByIndex(irr::gui::EGET_MESSAGEBOX_CANCEL, i) == exitMessageBoxID)
			{
				// althought the message box is removed from Irrlicht, we have to remove it from our gMgr
				gMgr->removeGUIElement(exitMessageBoxID);
				exitMessageBoxID = 0;
			}
		}
	}

	unsigned int numberOfButtonsClicked = iMgr->getNumberOfElementsForThisEvent(irr::gui::EGET_BUTTON_CLICKED);
	if(numberOfButtonsClicked > 0) // then there were button that were clicked
	{
		for(int i = 0; i < numberOfButtonsClicked; i++)
		{
			if(iMgr->getElementFromEventByIndex(irr::gui::EGET_BUTTON_CLICKED, i) == chatSubmitButtonID)
			{
				moveChatToHistory();
			}
		}
	}

	// check if the hit enter in the chat edit box
	unsigned int numberOfEntersHit = iMgr->getNumberOfElementsForThisEvent(irr::gui::EGET_EDITBOX_ENTER);
	if(numberOfEntersHit > 0)
	{
		for(int i = 0; i < numberOfEntersHit; i++)
		{
			if(iMgr->getElementFromEventByIndex(irr::gui::EGET_EDITBOX_ENTER, i) == chatEditBoxID)
				// we check if have any text at all
				// if there isn't any, we take foucs away from the chat box
				if(!moveChatToHistory()) // we have not text this time
				{
					gMgr->removeGUIFocus(chatEditBoxID);

					isChatBoxActive = false;

					// we also set the enter key down for now so it doesnt' count for this frame
					// if we unfocused the chat box, then check the enter key, it will focus it again :P
					enterKeyDown = true;
				}
		}
	}

	// we also check if the chat box got foucs through mouse click
	// we don't check the mouse click, just if the chat box got foucs
	unsigned int numberOfGotFocus = iMgr->getNumberOfElementsForThisEvent(irr::gui::EGET_ELEMENT_FOCUSED);
	if(numberOfGotFocus > 0) // then there were button that were clicked
	{
		for(int i = 0; i < numberOfGotFocus; i++)
		{
			if(iMgr->getElementFromEventByIndex(irr::gui::EGET_ELEMENT_FOCUSED, i) == chatEditBoxID)
			{
				isChatBoxActive = true;
			}
		}
	}

	unsigned int numberOfLostFocus = iMgr->getNumberOfElementsForThisEvent(irr::gui::EGET_ELEMENT_FOCUS_LOST);
	if(numberOfLostFocus > 0) // then there were button that were clicked
	{
		for(int i = 0; i < numberOfLostFocus; i++)
		{
			if(iMgr->getElementFromEventByIndex(irr::gui::EGET_ELEMENT_FOCUS_LOST, i) == chatEditBoxID)
			{
				isChatBoxActive = false;
			}
		}
	}

	// when done, clear the gui event list so we can check for more next frame
	iMgr->clearGUIEventList();

	if(iMgr->getKeyState(irr::KEY_RETURN) && !enterKeyDown)
	{
		if(!isChatBoxActive)
		{
			isChatBoxActive = true;

			gMgr->setGUIFocus(chatEditBoxID);
		}

		enterKeyDown = true;
	}
	else if(!iMgr->getKeyState(irr::KEY_RETURN) && enterKeyDown)
	{
		enterKeyDown = false;
	}

	// we handle network messages here
	// also check if disconnect for any reason, we go back to the main menu

	while(netMgr->getNewPacket2())
	{
		unsigned char idn;
		unsigned int netID;
		float nx, ny , nz, nyAngle;
		bool nhasMoved, nhasTurned, njustStopped;
		std::string name;
		unsigned int modelID;
		//unsigned int r, g, b;
		unsigned int numberOfUsers;
		RakNet::Time timeStamp;

		netMgr->fillBitStreamFromPacket();

		idn = netMgr->readUCharFromBitStream();

		core::stringw fullText;
		core::stringw oldText;

		// we check first if its starts with a time stamp
		if(idn == ID_TIMESTAMP)
		{
			timeStamp = netMgr->readTimeStampFromBitStream();

			// the next one is the message identifier
			idn = netMgr->readUCharFromBitStream();
		}

		switch(idn)
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			cout << "Another client has disconnected." << endl;
			break;
		case ID_REMOTE_CONNECTION_LOST:
			cout << "Another client has lost the connection." << endl;
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			cout << "Another client has connected." << endl;
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			cout << "Our connection request has been accepted." << endl;
			break;
		case ID_NEW_INCOMING_CONNECTION:
			cout << "A connection is incoming." << endl;
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			cout << "The server is full." << endl;
			break;
		case ID_DISCONNECTION_NOTIFICATION: // we go back to the main menu

			cout << "A client has disconnected." << endl;

			engine->RequestChangeState(new MainMenuState());

			break;
		case ID_CONNECTION_LOST: // we go back to the main menu

			cout << "A client lost the connection." << endl;

			engine->RequestChangeState(new MainMenuState());

			break;

		case ID_SEND_CHAT:

			{
				// we display this in the history edit box
				std::string sendMessage = netMgr->readStringFromBitStream();

				// we put our name at the beining
				fullText = core::stringw(sendMessage.c_str());

				// and add to the old history box
				oldText = core::stringw(gMgr->getGUIText(chatHistoryEditBoxID));
				oldText += L"\n";
				oldText += fullText;

				// then we put the text in the histroy box
				gMgr->setGUIText(chatHistoryEditBoxID, oldText.c_str());
			}

			break;

		case ID_ADD_CURRENT_USERS:

			///////////
			// 9th step
			///////////

			cout << "Step 9" << endl;

			// we were givig a lst of all active usres
			// we add them
			// then we send the confirmation that everything is fine

			// we add the netID for our character now (fix this)
			netID = netMgr->readUIntFromBitStream();

			gameMgr->addResourceID(myID, "netID", netID);

			// we get the number of users sent
			numberOfUsers = netMgr->readUIntFromBitStream();

			// now we go one by one, if any
			for(int i  = 0; i < numberOfUsers; i++)
			{
				netID = netMgr->readUIntFromBitStream();
				nx = netMgr->readFloatFromBitStream();
				ny = netMgr->readFloatFromBitStream();
				nz = netMgr->readFloatFromBitStream();
				nyAngle = netMgr->readFloatFromBitStream();
				name = netMgr->readStringFromBitStream();
				modelID = netMgr->readUIntFromBitStream();
				//r = netMgr->readUIntFromBitStream();
				//g = netMgr->readUIntFromBitStream();
				//b = netMgr->readUIntFromBitStream();

				// now create characters/cubes
				addNewUser(netID, nx, ny, nz, nyAngle, (char*)name.c_str(), modelID);
			}

			// we are done. Send confirm
			netMgr->clearBitSteram();
			netMgr->writeUCharToBitStream((unsigned char)ID_CONFIRM_CURRENT_USERS);
			netMgr->writeUIntToBitStream(gameMgr->getResourceID(myID, "netID"));

			netMgr->sendBitStreamToAll(HIGH_PRIORITY, RELIABLE_ORDERED, 0);
			cout << "Go to step 10" << endl;

			break;

		case ID_REQUST_ADD_NEW_USER :


			///////////
			// 11th step
			///////////

			cout << "Step 11" << endl;

			// we get a message that a new user was added
			// we added him here
			// then we send a confirm that we added him

			// get the values
			netID = netMgr->readUIntFromBitStream();
			nx = netMgr->readFloatFromBitStream();
			ny = netMgr->readFloatFromBitStream();
			nz = netMgr->readFloatFromBitStream();
			nyAngle = netMgr->readFloatFromBitStream();
			name = netMgr->readStringFromBitStream();
			modelID = netMgr->readUIntFromBitStream();
			//r = netMgr->readUIntFromBitStream();
			//g = netMgr->readUIntFromBitStream();
			//b = netMgr->readUIntFromBitStream();

			// now create characters/cubes
			addNewUser(netID, nx, ny, nz, nyAngle, (char*)name.c_str(), modelID);

			// we are done. Send confirm, our ID, and the ID of the new guy
			netMgr->clearBitSteram();
			netMgr->writeUCharToBitStream((unsigned char)ID_CONFIRM_NEW_USER);
			netMgr->writeUIntToBitStream(gameMgr->getResourceID(myID, "netID"));
			netMgr->writeUIntToBitStream(netID);

			netMgr->sendBitStreamToAll(HIGH_PRIORITY, RELIABLE_ORDERED, 0);
			cout << "Go to step 12" << endl;

			break;

		case ID_YOU_ARE_ACTIVE:

			///////////
			// 13th step
			///////////

			cout << "Step 13" << endl;

			// we have been activated. We can now start receving and sending updates

			isNetActive = true;

			cout << "We are active" << endl;

			break;

		case ID_UPDATE:

			// we recevie someone else's update, we put it here

			netID = netMgr->readUIntFromBitStream();

			nhasMoved = netMgr->readBoolFromBitStream();

			if(nhasMoved)
			{
				nx = netMgr->readFloatFromBitStream();
				ny = netMgr->readFloatFromBitStream();
				nz = netMgr->readFloatFromBitStream();

				njustStopped = netMgr->readBoolFromBitStream();
			}

			nhasTurned = netMgr->readBoolFromBitStream();

			if(nhasTurned)
			{
				nyAngle = netMgr->readFloatFromBitStream();
			}

			unsigned int objID;
			unsigned int cubeID;

			if(netObjectList.find(netID) != netObjectList.end())
			{
				objID = netObjectList[netID];
				cubeID = gameMgr->getResourceID(objID, "bodyID");

				// only update when we are at halt, moving, or stopping
				if(gameMgr->getState(objID) == 1 || gameMgr->getState(objID) == 2 || gameMgr->getState(objID) == 3 || gameMgr->getState(objID) == 4)
				{
					if(nhasMoved)
					{
						//gameMgr->setPosition(objID, nx, ny, nz);

						//gMgr->setNodePosition(cubeID, nx, ny, nz);

						// we set our finial goal
						// then we fill in the direction
						// then we set the state if we are moving or stopping

						float startX, startY, startZ;
						gameMgr->getPosition(objID, startX, startY, startZ);
						float dirX = nx - startX;
						float dirY = ny - startY;
						float dirZ = nz - startZ;

						gMgr->normalizeVector(dirX, dirY, dirZ);

						// we set the end point
						gameMgr->setMoveToX(objID, nx);
						gameMgr->setMoveToY(objID, ny);
						gameMgr->setMoveToZ(objID, nz);

						// we set the direction to move too
						gameMgr->setMoveTowardX(objID, dirX);
						gameMgr->setMoveTowardY(objID, dirY);
						gameMgr->setMoveTowardZ(objID, dirZ);

						// setting the animation
						// we check which state we are in
						// if we weren't moving, then we set the animation to run/walk
						if(gameMgr->getState(objID) == 1) // we are in stop/stand state
						{
							int start, end;
							float frame_speed;
							bool loop;

							unsigned int modelNumber = gameMgr->getBodyViewID(objID);
							getAnimationInfoFromDB(modelNumber, "run", start, end, frame_speed, loop);
							gMgr->setAnimationFrameLoop(cubeID, start, end);
							gMgr->setAnimationFrameSpeed(cubeID, frame_speed);
							gMgr->setAnimationLoop(cubeID, loop);
						}


						// now we set we either moving or heading to a stop
						if(!njustStopped) // we are not stopping
						{
							gameMgr->setState(objID, 2); // moving
						}
						else // we are stopping
						{
							gameMgr->setState(objID, 4); // stopping
						}
					}

					if(nhasTurned)
					{
						unsigned int nxAngle = 0;
						unsigned int nzAngle = 0;

						gameMgr->setRotation(objID, nxAngle, nyAngle, nzAngle);

						gMgr->setNodeRotation(cubeID, nxAngle, nyAngle + gameMgr->getRotationOffset(objID), nzAngle);
					}
				}
			}

			break;

		case ID_REMOVE_USER:

			netID = netMgr->readUIntFromBitStream();

			removeUser(netID);

			break;

		case ID_ACCEPT_LOCK_ON:
			{

				// the server sent the target's info (HP and MaxHP)

				netID = netMgr->readUIntFromBitStream();
				unsigned int objID = netObjectList[netID];
				int maxHP = netMgr->readIntFromBitStream();
				int HP = netMgr->readIntFromBitStream();

				// we set the target for our character
				gameMgr->setTarget(myID, objID);

				// now we set the info
				gameMgr->setMaxHP(objID, maxHP);
				gameMgr->setHP(objID, HP);

			}
			break;

		case ID_UPDATE_HP: // a object changed its HP
			{
				netID = netMgr->readUIntFromBitStream();
				int hp = netMgr->readIntFromBitStream();

				unsigned int objID = netObjectList[netID];

				gameMgr->setHP(objID, hp);
			}
			break;

		case ID_ATTACK: // someone is attacking
			{
				netID = netMgr->readUIntFromBitStream();
				unsigned int objID = netObjectList[netID];
				unsigned int bodyID = gameMgr->getResourceID(objID, "bodyID");
				unsigned int bodyModelID = gameMgr->getBodyViewID(objID);

				float x, y, z, xAngle = 0, yAngle, zAngle = 0;
				unsigned int ap;

				x = netMgr->readFloatFromBitStream();
				y = netMgr->readFloatFromBitStream();
				z = netMgr->readFloatFromBitStream();
				yAngle = netMgr->readFloatFromBitStream();
				ap = netMgr->readUIntFromBitStream();

				gameMgr->setState(objID, 5); // we set its state to attack
				gameMgr->setPosition(objID, x, y, z);
				gameMgr->setRotation(objID, xAngle, yAngle, zAngle);
				gameMgr->setActionID(objID, 1); // 1 for now
				gameMgr->setActionPower(objID, (float)ap);

				gMgr->setNodePosition(bodyID, x, gameMgr->getYOffset(objID), z);
				gMgr->setNodeRotation(bodyID, 0, yAngle + gameMgr->getRotationOffset(objID), 0);

				int start, end;
				float frame_speed;
				bool loop;

				getAnimationInfoFromDB(bodyModelID, "attack_1", start, end, frame_speed, loop);
				gMgr->setAnimationFrameLoop(bodyID, start, end);
				gMgr->setAnimationFrameSpeed(bodyID, frame_speed);
				gMgr->setAnimationLoop(bodyID, loop);
			}
			break;

		case ID_DEAD: // someone is dead
			{
				netID = netMgr->readUIntFromBitStream();
				unsigned int objID = netObjectList[netID];
				unsigned int bodyID = gameMgr->getResourceID(objID, "bodyID");
				unsigned int bodyModelID = gameMgr->getBodyViewID(objID);

				float x, y, z, xAngle = 0, yAngle, zAngle = 0;

				x = netMgr->readFloatFromBitStream();
				y = netMgr->readFloatFromBitStream();
				z = netMgr->readFloatFromBitStream();
				yAngle = netMgr->readFloatFromBitStream();

				gameMgr->setState(objID, 6); // we set its state to dead
				gameMgr->setPosition(objID, x, y, z);
				gameMgr->setRotation(objID, xAngle, yAngle, zAngle);

				gMgr->setNodePosition(bodyID, x, gameMgr->getYOffset(objID), z);
				gMgr->setNodeRotation(bodyID, 0, yAngle + gameMgr->getRotationOffset(objID), 0);

				int start, end;
				float frame_speed;
				bool loop;

				getAnimationInfoFromDB(bodyModelID, "dead", start, end, frame_speed, loop);
				gMgr->setAnimationFrameLoop(bodyID, start, end);
				gMgr->setAnimationFrameSpeed(bodyID, frame_speed);
				gMgr->setAnimationLoop(bodyID, loop);
			}
			break;

		case ID_RESPAWN: // someone spawn
			{
				netID = netMgr->readUIntFromBitStream();
				unsigned int objID = netObjectList[netID];
				unsigned int bodyID = gameMgr->getResourceID(objID, "bodyID");
				unsigned int bodyModelID = gameMgr->getBodyViewID(objID);

				gameMgr->setState(objID, 1); // we set its state to stop

				gMgr->setNodePosition(bodyID, 0, gameMgr->getYOffset(objID), 0);
				gMgr->setNodeRotation(bodyID, 0, gameMgr->getRotationOffset(objID), 0);
				gameMgr->setPosition(objID, 0, 0, 0);
				gameMgr->setRotation(objID, 0, 0, 0);

				int start, end;
				float frame_speed;
				bool loop;

				getAnimationInfoFromDB(bodyModelID, "stand", start, end, frame_speed, loop);
				gMgr->setAnimationFrameLoop(bodyID, start, end);
				gMgr->setAnimationFrameSpeed(bodyID, frame_speed);
				gMgr->setAnimationLoop(bodyID, loop);

				// we also check if he is our privose target, we remove him
				if(gameMgr->getTarget(myID) == objID)
				{
					gameMgr->setTarget(myID, 0);
				}
			}
			break;

		case ID_BLOCK_START: // someone is blocking
			{
				netID = netMgr->readUIntFromBitStream();
				unsigned int objID = netObjectList[netID];
				unsigned int bodyID = gameMgr->getResourceID(objID, "bodyID");
				unsigned int bodyModelID = gameMgr->getBodyViewID(objID);

				float x, y, z, xAngle = 0, yAngle, zAngle = 0;

				x = netMgr->readFloatFromBitStream();
				y = netMgr->readFloatFromBitStream();
				z = netMgr->readFloatFromBitStream();
				yAngle = netMgr->readFloatFromBitStream();

				gameMgr->setState(objID, 7); // we set its state to block
				gameMgr->setPosition(objID, x, y, z);
				gameMgr->setRotation(objID, xAngle, yAngle, zAngle);

				gMgr->setNodePosition(bodyID, x, gameMgr->getYOffset(objID), z);
				gMgr->setNodeRotation(bodyID, 0, yAngle + gameMgr->getRotationOffset(objID), 0);

				int start, end;
				float frame_speed;
				bool loop;

				getAnimationInfoFromDB(bodyModelID, "block", start, end, frame_speed, loop);
				gMgr->setAnimationFrameLoop(bodyID, start, end);
				gMgr->setAnimationFrameSpeed(bodyID, frame_speed);
				gMgr->setAnimationLoop(bodyID, loop);
			}
			break;

		case ID_BLOCK_END: // someone has stopped blocking
			{
				netID = netMgr->readUIntFromBitStream();
				unsigned int objID = netObjectList[netID];
				unsigned int bodyID = gameMgr->getResourceID(objID, "bodyID");
				unsigned int bodyModelID = gameMgr->getBodyViewID(objID);

				//float x, y, z, xAngle = 0, yAngle, zAngle = 0;

				//x = netMgr->readFloatFromBitStream();
				//y = netMgr->readFloatFromBitStream();
				//z = netMgr->readFloatFromBitStream();
				//yAngle = netMgr->readFloatFromBitStream();

				gameMgr->setState(objID, 8); // we set its state to stop blocking
				//gameMgr->setPosition(objID, x, y, z);
				//gameMgr->setRotation(objID, xAngle, yAngle, zAngle);

				//gMgr->setNodePosition(bodyID, x, gameMgr->getYOffset(objID), z);
				//gMgr->setNodeRotation(bodyID, 0, yAngle + gameMgr->getRotationOffset(objID), 0);

				int start, end;
				float frame_speed;
				bool loop;

				getAnimationInfoFromDB(bodyModelID, "block", start, end, frame_speed, loop);
				gMgr->setAnimationFrameLoop(bodyID, start, end);
				gMgr->setAnimationCurrentFrame(bodyID, end);
				gMgr->setAnimationFrameSpeed(bodyID, frame_speed * -1);
				gMgr->setAnimationLoop(bodyID, loop);
			}
			break;

		default:
			cout << "Message with identifier " << idn << "has arrived." << endl;
			break;
		}

		netMgr->DestroyPacket2();
	}

	if(!isChatBoxActive)
	{
		// we check if the lock down button (1) was pressesd
		if(iMgr->getKeyState(irr::KEY_KEY_1) && !isLockOnDown)
		{
			// we check first if we already have a target, and if its not already selected
			unsigned int targetID = findNearestObject();

			//if(gameMgr->getTarget(myID) != targetID && targetID != 0)
			//{
			//	sendLockOnTarget(targetID);
			//}
			lockOnTarget(targetID);
			// we set this to true to update or rotation once.
			lockOnChange = true;

			isLockOnDown = true; // to make sure we don't press the button till we release it first
		}
		if(!iMgr->getKeyState(irr::KEY_KEY_1) && isLockOnDown)
		{
			isLockOnDown = false;
		}

		//// we check if want to unlock a target. button (2)
		////if(iMgr->getKeyState(irr::KEY_KEY_2) && gameMgr->getTarget(myID) != 0) // we check first if have a target to begine with
		//if(iMgr->getKeyState(irr::KEY_KEY_2)) // we check first if have a target to begine with
		//{
		//	//gameMgr->setTarget(myID, 0);
		//	//sendLockOnTarget(0);
		//	lockOnTarget(0);
		//	// we set this to true to update or rotation once.
		//	lockOnChange = true;
		//}
	}

	// get key movement and set speed
	moveSide = 0;
	moveForward = 0;

	if(!isChatBoxActive && !frezz)
	{
		if(iMgr->getKeyState(irr::KEY_RIGHT) || iMgr->getKeyState(irr::KEY_KEY_D))
			moveSide -= 1;
		if(iMgr->getKeyState(irr::KEY_LEFT) || iMgr->getKeyState(irr::KEY_KEY_A))
			moveSide += 1;
		if(iMgr->getKeyState(irr::KEY_UP) || iMgr->getKeyState(irr::KEY_KEY_W))
			moveForward += 1;
		if(iMgr->getKeyState(irr::KEY_DOWN) || iMgr->getKeyState(irr::KEY_KEY_S))
			moveForward -= 1;
	}

	
	if(iMgr->getKeyState(irr::KEY_KEY_I)) {

		if(isInventory == false) {
			gMgr->InvVisible(true, Inventory);
			isInventory = true;
			isInvEvents = true;
		}

		else if (isInventory == true) {
			gMgr->InvVisible(false, Inventory);
			isInventory = false;
			isInvEvents = false;
		}

		

	}
	

	// we check if the mosue right click is down
	isRightClickDown = false;

	// TODO fix this, the right button doesn't resoponse
	if(iMgr->getKeyState(irr::KEY_RBUTTON))
		isRightClickDown = true;

	if(!isChatBoxActive && !frezz)
	{
		//// test, to see if HP will change
		//// this part increase the HP by 10
		//if(iMgr->getKeyState(irr::KEY_KEY_3) && !isHPupDown)
		//{
		//	int hp = gameMgr->getHP(myID);
		//	int Maxhp = gameMgr->getMaxHP(myID);

		//	if(hp < Maxhp)
		//	{
		//		gameMgr->setHP(myID, hp + 50);

		//		updateHP();
		//	}

		//	isHPupDown = true;
		//}
		//if(!iMgr->getKeyState(irr::KEY_KEY_3) && isHPupDown)
		//{
		//	isHPupDown = false;
		//}
		//// this part decresae the HP by 10
		//if(iMgr->getKeyState(irr::KEY_KEY_4) && !isHPlowDown)
		//{
		//	int hp = gameMgr->getHP(myID);
		//	int Minhp = gameMgr->getMinHP(myID);

		//	if(hp > Minhp)
		//	{
		//		gameMgr->setHP(myID, hp - 50);

		//		updateHP();
		//	}

		//	if(checkForDeath()) // if we are dead
		//	{
		//		sendDeath();
		//	}

		//	isHPlowDown = true;
		//}
		//if(!iMgr->getKeyState(irr::KEY_KEY_4) && isHPlowDown)
		//{
		//	isHPlowDown = false;
		//}

		// this is the attack part, button left control
		if(iMgr->getKeyState(irr::KEY_LCONTROL) && (gameMgr->getState(myID) == 1 || gameMgr->getState(myID) == 2) && !isAttackDown)
		{
			isAttackDown = true;

			// we attack

			attack();

			// then we send attack to server

			sendAttack();

			// last, we decrese the AP;
			float ap = gameMgr->getAP(myID);
			ap -= 30; // lose 30 AP for attack
			gameMgr->setAP(myID, ap);

			//if(ap <= 0)
			//	frezz = true;
		}
		else if(!iMgr->getKeyState(irr::KEY_LCONTROL) && isAttackDown)
		{
			isAttackDown = false;
		}

		// shooting
		if(iMgr->getKeyState(irr::KEY_KEY_2))
		{
			shootFireBall();
		}

		// block, button left shift
		if(iMgr->getKeyState(irr::KEY_LSHIFT) && (gameMgr->getState(myID) == 1 || gameMgr->getState(myID) == 2))
		{
			// we block

			block();

			// then we send block to server

			sendBlock();
		}
		else if(!iMgr->getKeyState(irr::KEY_LSHIFT) && gameMgr->getState(myID) == 6)
		{
			// we stop blocking

			stopBlock();

			// then we send stop block to server

			sendStopBlock();
		}

	} // end of if(!isChatBoxActive)

}

void MainClientState::Update(CStateEngine *engine)
{
	// you have to tick this first in Irrlicht to get the new time and to start the drawing for this frame
	gMgr->tick();

	// we get the delta time so we can move objects in the correct speed
	float deltaTime = gMgr->getNewDeltaTime();

	// we start first by updating the other players and objects
	updateNetObjects(deltaTime);


	updateCameraLook();

	// we only move if are in normal state (not attacking, blocking, etc...)
	switch(gameMgr->getState(myID))
	{
	case 1:
	case 2:

		updatePlayerMovement(deltaTime);

		break;

	case 3:
		break;

	case 4: // we are dead here, we check check th timer to respwan again

		spawnTimer -= deltaTime;

		if(spawnTimer < 0)
		{
			respawn();
			sendRespawn();
		}

		break;

	default:
		break;
	}

	updateCameraPosition();

	// we only rotate if are in normal state (not attacking, blocking, etc...)
	switch(gameMgr->getState(myID))
	{
	case 1:
	case 2:

		updatePlayerDirection();

		break;

	case 3:
		break;

	default:
		break;
	}

	updateAnimationChange();

	// test update for AP contorl
	// we start losing AP when we run for a few seconds
	if(apRunLimit > 0 && hasMoved)
	{
		apRunLimit -= deltaTime;
	}
	else if(apRunLimit < 0 && hasMoved)
	{
		// we start losing AP here

		apDecreaseSpeed -= deltaTime;

		if(apDecreaseSpeed < 0)
		{
			int ap = gameMgr->getAP(myID);
			ap--;
			gameMgr->setAP(myID, ap);

			apDecreaseSpeed += apDecreaseSpeedValue;

			if(ap <= 0)
				frezz = true;
		}
	}
	else if(!hasMoved)
	{
		apRunLimit = apRunLimitValue;
	}
	// we start gaining AP when we block for a few seconds
	if(apBlockLimit > 0 && gameMgr->getState(myID) == 6)
	{
		apBlockLimit -= deltaTime;
	}
	else if(apBlockLimit < 0 && gameMgr->getState(myID) == 6)
	{
		// we start gaining AP here

		apIncreaseSpeed -= deltaTime;

		if(apIncreaseSpeed < 0)
		{
			int ap = gameMgr->getAP(myID);
			ap++;
			gameMgr->setAP(myID, ap);

			apIncreaseSpeed += apIncreaseSpeedValue;
		}
	}
	else if(gameMgr->getState(myID) == 7)
	{
		apBlockLimit = apBlockLimitValue;
	}

	// if we are frozen, we need to wait till our AP is more than 20
	if(frezz)
	{
		int ap = gameMgr->getAP(myID);

		frezzIncreaseSpeed -= deltaTime;

		if(frezzIncreaseSpeed < 0)
		{
			ap++;
			gameMgr->setAP(myID, ap);

			frezzIncreaseSpeed += frezzIncreaseSpeedValue;
		}

		if(ap >= 20)
		{
			frezz = false;
		}
	}

	// we check here if we are in or out of the golem area, then fade the music accordinglly
	updateGolemArea(deltaTime);

	// we update fire balls
	updateFireBalls(deltaTime);

	updateInterface();

	// we send our update to the server
	checkAndSendUpdate(deltaTime);

}

void MainClientState::Draw(CStateEngine *engine)
{
	gMgr->drawAll();
}

void MainClientState::creatChatBox(void)
{
	gMgr = GraphicManager::getInstance();

	// draw the chat box area
	// first we get our group x and y. If we move this we move the whole menu
	// the chat box (for now) should cover the lower left area of the screen.

	float screenWidth = game->getScreenWidth();
	float screenHeight = game->getScreenHeight();

	float groupX = 0;
	float groupY = screenHeight * 0.60;
	float groupW = screenWidth * 0.40;
	float groupH = screenHeight * 0.40;

	// history area should be 90% the height of the chat area
	float historyH = 0.90;
	// the chat edit box is 80% of the area width, leaving the rest for the submit button
	float editW = 0.80;

	// draw the history edit box
	chatHistoryEditBoxID = gMgr->createEditBox(groupX, groupY, groupX + groupW, groupY + (groupH * historyH));
	gMgr->setEditBoxTextAlignment(chatHistoryEditBoxID, irr::gui::EGUI_ALIGNMENT::EGUIA_UPPERLEFT, irr::gui::EGUI_ALIGNMENT::EGUIA_LOWERRIGHT);
	gMgr->setEditBoxAutoScroll(chatHistoryEditBoxID, true);
	gMgr->setEditBoxWordWrap(chatHistoryEditBoxID, true);
	gMgr->setEditBoxMultiLine(chatHistoryEditBoxID, true);

	// draw the edit box
	chatEditBoxID = gMgr->createEditBox(groupX, groupY + (groupH * historyH), groupX + (groupW * editW), groupY + groupH);

	// draw the sumbit button
	chatSubmitButtonID = gMgr->createButton(groupX + (groupW * editW), groupY + (groupH * historyH), groupX + groupW, groupY + groupH);
	gMgr->setGUIText(chatSubmitButtonID, L"SEND");
}

bool MainClientState::moveChatToHistory(void)
{
	// frist we check if we have any text in the chat edit box
	core::stringw chatText(gMgr->getGUIText(chatEditBoxID));

	if(chatText == "")
		return false; // we have no text, we leave

	// we have text, first:

	// we clear the chat edit box
	gMgr->setGUIText(chatEditBoxID, L"");

	// we put our name at the beining
	core::stringw fullText = core::stringw(game->getUserName());
	fullText += ": ";
	fullText += chatText;

	// and add to the old history box
	core::stringw oldText(gMgr->getGUIText(chatHistoryEditBoxID));
	oldText += L"\n";
//	oldText += core::stringw(game->getUserName());
//	oldText += ": ";
	oldText += fullText;

	// then we put the text in the histroy box
	gMgr->setGUIText(chatHistoryEditBoxID, oldText.c_str());

	// now we send this chat to the server

	netMgr->clearBitSteram();

	netMgr->writeUCharToBitStream((unsigned char)ID_SEND_CHAT);
	netMgr->writeStringToBitStream((char*)core::stringc(fullText).c_str());

	// now we send the bitstream
	netMgr->sendBitStreamToAll(HIGH_PRIORITY, RELIABLE_ORDERED, 0);

	return true;

}

void MainClientState::addNewUser(unsigned int netID, float x, float y, float z, float yAngle, char *name, unsigned int modelNumber)
{
	float xAngle = 0;
	float zAngle = 0;

	// varibales to get database data
	std::string modelPath;
	std::vector<std::string> texturePathList;
	float yOffset;
	float rotationOffset;
	float scaleOffset;
	int start, end;
	float frame_speed;
	bool loop;

	unsigned int objectID = gameMgr->createGameObject();
	gameMgr->addGameComponent(objectID, ComponentType::NAME_COMP);
	gameMgr->addGameComponent(objectID, ComponentType::BODY_VIEW_COMP);
	gameMgr->addGameComponent(objectID, ComponentType::MOVE_TO_COMP);
	gameMgr->addGameComponent(objectID, ComponentType::MOVE_TOWARD_COMP);
	gameMgr->addGameComponent(objectID, ComponentType::ROTATE_TO_COMP);
	gameMgr->addGameComponent(objectID, ComponentType::HP_COMP);
	gameMgr->addGameComponent(objectID, ComponentType::ACTION_COMP);

	netObjectList[netID] = objectID;

	gameMgr->setPosition(objectID, x, y, z);
	gameMgr->setRotation(objectID, xAngle, yAngle, zAngle);
	gameMgr->setName(objectID, name);
	gameMgr->setBodyViewID(objectID, modelNumber);
	//gameMgr->setRed(objectID, r);
	//gameMgr->setGreen(objectID, g);
	//gameMgr->setBlue(objectID, b);

	getModelAndTextureFromDB(modelNumber, modelPath, texturePathList, yOffset, rotationOffset, scaleOffset);
	getAnimationInfoFromDB(modelNumber, "stand", start, end, frame_speed, loop);

	gameMgr->setYOffset(objectID, yOffset);
	gameMgr->setRotationOffset(objectID, rotationOffset);

	unsigned int modelID = gMgr->createAnimatedMesh((char*)modelPath.c_str());
	int listSize = texturePathList.size();
	for(int i = 0; i < listSize; i++)
	{
		gMgr->setNodeTexture(modelID, i, (char*)texturePathList[i].c_str());
	}
	gameMgr->addResourceID(objectID, "bodyID", modelID);

	unsigned int nameID = gMgr->create3DText((char*)dbMgr->getDataAsString("test_font").c_str(), name);
	gameMgr->addResourceID(objectID, "nameID", nameID);

	gMgr->setAnimationFrameLoop(modelID, start, end);
	gMgr->setAnimationFrameSpeed(modelID, frame_speed);
	gMgr->setNodePosition(modelID, x, gameMgr->getYOffset(objectID), z);
	//gMgr->setNodeScale(modelID, 3.5, 3.5, 3.5);
	gMgr->setNodeScale(modelID, scaleOffset, scaleOffset, scaleOffset);
	gMgr->setNodePosition(nameID, 0, 10 - (gameMgr->getYOffset(objectID)), 0);
	gMgr->setNodeParent(modelID, nameID);

	gameMgr->addResourceID(objectID, "netID", netID);

	// at the end we set the the state for this network object
	// 1 = completely stopped
	// 2 = moving
	// 3 = keep moving. When we reach the end and we didnt get an update, we keep moving in the same direction
	// 4 = stopping
	// 5 = attacking
	// 6 = dead
	// 7 = block
	// 8 = stop block
	gameMgr->setState(objectID, 1);
}

void MainClientState::removeUser(unsigned int netID)
{
	// remove the cube first
	//if(transformList.find(netID) != transformList.end())
	if(netObjectList.find(netID) != netObjectList.end())
	{
		unsigned int objID = netObjectList[netID];
		unsigned int cubeID = gameMgr->getResourceID(objID, "bodyID");

		gMgr->removeNode(cubeID);

		// we check if are locked on to the removed guy
		if(gameMgr->getState(myID) == objID)
		{
			gameMgr->setState(myID, 0);
		}

		netObjectList.erase(netID);
	}
}

void MainClientState::updateCameraLook(void)
{
	hasTurned = false;

	// now we get some stored data to use here.
	float xAngle, yAngle, zAngle;
	gameMgr->getRotation(cameraID, xAngle, yAngle, zAngle);

	// we check the mouse delta position, and rotate the direction
	int mouseX = 0, mouseY = 0;
	gMgr->getDeltaMousePosition(mouseX, mouseY);

	if(isRightClickDown)
	{
		if(mouseX != 0) // we rotate right and left
		{
			yAngle += mouseX; // we add senstivy to slow or speed head movment

			// we fix the angel to stay between 0 and 360
			if(yAngle > 360)
				yAngle -= 360;
			if(yAngle < 0)
				yAngle += 360;

			// we have turned
			hasTurned = true;
		}
		if(mouseY != 0) // we roatate up and down
		{
			xAngle += mouseY; // we add senstivy to slow or speed head movment

			// we check the limits
			if(xAngle > maxXAngle)
				xAngle = maxXAngle;
			if(xAngle < minXAngle)
				xAngle = minXAngle;

			// we have turned
			hasTurned = true;
		}
	}

	gameMgr->setRotation(cameraID, xAngle, yAngle, zAngle);
}

void MainClientState::updatePlayerMovement(float deltaTime)
{
	hasMoved = false;

	// now we get some stored data to use here.
	float x, y, z;
	float xAngle, yAngle, zAngle;
	gameMgr->getPosition(myID, x, y, z);
	gameMgr->getRotation(cameraID, xAngle, yAngle, zAngle);

	// we get the direction we are looking at using the yAngle and xAngel;
	float lookX = xAngle, lookY = yAngle, lookZ = 0;
	gMgr->rotationToDirection(lookX, lookY, lookZ);
	// we normlize
	gMgr->normalizeVector(lookX, lookY, lookZ);

	// we get the forward direction
	float fx = lookX, fy = 0, fz = lookZ;
	// we normilize that too
	gMgr->normalizeVector(fx, fy, fz);

	// we get the side direction, by crossing the forward direcitn with the up(0, 1, 0) direciton
	float sx = fx, sy = fy, sz = fz;
	gMgr->crossProductVector(sx, sy, sz, 0, 1, 0);
	// and don't forget to normlize
	gMgr->normalizeVector(sx, sy, sz);

	// no we get the real driection, by checking what button has been pressed, then adding that to real direction
	float realX = 0, realY = 0, realZ = 0;
	// check the forward direction first
	if(moveForward != 0)
	{
		realX += fx * moveForward;
		realY += fy * moveForward;
		realZ += fz * moveForward;

		// we have moved
		hasMoved = true;
	}
	// then the side direction
	if(moveSide != 0)
	{
		realX += sx * moveSide;
		realY += sy * moveSide;
		realZ += sz * moveSide;

		// we have moved
		hasMoved = true;
	}
	// now we normilize
	gMgr->normalizeVector(realX, realY, realZ);

	if(hasMoved)
	{
		oldHasMoved = true;
		justStopped = false;
	}

	if(!hasMoved && oldHasMoved)
	{
		oldHasMoved = false;
		justStopped = true;
	}

	// move the object
	x += realX * deltaTime * speed;
	z += realZ * deltaTime * speed;
	unsigned int bodyID = gameMgr->getResourceID(myID, "bodyID");
	gMgr->setNodePosition(bodyID, x, gameMgr->getYOffset(myID), z);
	// now we set the whole thing back in the game object
	gameMgr->setPosition(myID, x, y, z);
}

void MainClientState::updateCameraPosition(void)
{
	// we get some stored data to use here.
	float x, y, z;
	float xAngle, yAngle, zAngle;
	gameMgr->getPosition(myID, x, y, z);
	gameMgr->getRotation(cameraID, xAngle, yAngle, zAngle);

	// we get the direction we are looking at using the yAngle and xAngel;
	float lookX = xAngle, lookY = yAngle, lookZ = 0;
	gMgr->rotationToDirection(lookX, lookY, lookZ);
	// we normlize
	gMgr->normalizeVector(lookX, lookY, lookZ);

	// set the camera position and look at direction
	// we muliply the look vector by the cameraFarOut, then we move the camera back
	float cameraX = x + lookX * cameraFarOut * -1, cameraY = 0 + lookY * cameraFarOut * -1, cameraZ = z + lookZ * cameraFarOut * -1;
	gMgr->setCameraPosition(cameraX, cameraY, cameraZ);
	gMgr->setCameraLookAt(x, 0, z);
	gameMgr->setPosition(cameraID, cameraX, cameraY, cameraZ);
}

void MainClientState::updatePlayerDirection(void)
{
	// this part is a bit tricky
	// I need to contorl where the character is facing
	// if we are not locked on to anyone, then we face the same direciton as the camera
	// if we are locked on someone, we face him
	// we only update if the rotaiton if we or the target has moved

	float xAngle, yAngle, zAngle;
	gameMgr->getRotation(cameraID, xAngle, yAngle, zAngle);
	unsigned int bodyID = gameMgr->getResourceID(myID, "bodyID");

	unsigned int targetID = gameMgr->getTarget(myID);
	if(targetID == 0) // no target
	{
		gMgr->setNodeRotation(bodyID, 0, yAngle + gameMgr->getRotationOffset(myID), 0);
		gameMgr->setRotation(myID, xAngle, yAngle, zAngle);
	}
	else // we do have a target
	{
		float startX, startY, startZ;
		float endX, endY, endZ;
		float txAngle, tyAngle, tzAngle;

		gameMgr->getPosition(myID, startX, startY, startZ);
		gameMgr->getPosition(targetID, endX, endY, endZ);
		txAngle = endX - startX;
		tyAngle = endY - startY;
		tzAngle = endZ - startZ;

		gMgr->directionToRotation(txAngle, tyAngle, tzAngle);

		gMgr->setNodeRotation(bodyID, 0, tyAngle + gameMgr->getRotationOffset(myID), 0);
		gameMgr->setRotation(myID, txAngle, tyAngle, tzAngle);

		// we update the turning if either we or the target was moving

		unsigned int targetState = gameMgr->getState(targetID);
		hasTurned = hasMoved || targetState == 2 || targetState == 3 || targetState == 4;

	}
}

void MainClientState::updateAnimationChange(void)
{
	unsigned int bodyID = gameMgr->getResourceID(myID, "bodyID");

	// we check and set animation
	switch(gameMgr->getState(myID))
	{
	case 1: // this is the stopping state

		// we check if we are moving, then we change to running animation

		if(hasMoved)
		{
			int start, end;
			float frame_speed;
			bool loop;

			gameMgr->setState(myID, 2);
			getAnimationInfoFromDB(gameMgr->getBodyViewID(myID), "run", start, end, frame_speed, loop);
			gMgr->setAnimationFrameLoop(bodyID, start, end);
			gMgr->setAnimationFrameSpeed(bodyID, frame_speed);
			gMgr->setAnimationLoop(bodyID, loop);
		}

		break;

	case 2: // this is the moving state

		// we check if we are not stopping, then we change to stand animation

		if(!hasMoved)
		{
			int start, end;
			float frame_speed;
			bool loop;

			gameMgr->setState(myID, 1);
			getAnimationInfoFromDB(gameMgr->getBodyViewID(myID), "stand", start, end, frame_speed, loop);
			gMgr->setAnimationFrameLoop(bodyID, start, end);
			gMgr->setAnimationFrameSpeed(bodyID, frame_speed);
			gMgr->setAnimationLoop(bodyID, loop);
		}

		break;

	case 3: // we check if attack animation has finished
		{

			int start, end;
			float frame_speed;
			bool loop;

			getAnimationInfoFromDB(gameMgr->getBodyViewID(myID), "attack_1", start, end, frame_speed, loop);

			if(gMgr->getAnimationCurrentFrame(bodyID) == end) // we have passed the end of the animation
			{
				// we everything to go back to standing

				getAnimationInfoFromDB(gameMgr->getBodyViewID(myID), "stand", start, end, frame_speed, loop);
				gMgr->setAnimationFrameLoop(bodyID, start, end);
				gMgr->setAnimationFrameSpeed(bodyID, frame_speed);
				gMgr->setAnimationLoop(bodyID, loop);

				gameMgr->setState(myID, 1);
			}

		}
		break;

	case 5: // we wait for blocking to reach its end
		{

			int start, end;
			float frame_speed;
			bool loop;

			getAnimationInfoFromDB(gameMgr->getBodyViewID(myID), "block", start, end, frame_speed, loop);

			if(gMgr->getAnimationCurrentFrame(bodyID) == end) // we have passed the end of the animation
			{
				// we change state to hold block
				gameMgr->setState(myID, 6);
			}

		}
		break;

	case 7: // we stop blocking
		{

			int start, end;
			float frame_speed;
			bool loop;

			getAnimationInfoFromDB(gameMgr->getBodyViewID(myID), "block", start, end, frame_speed, loop);

			if(gMgr->getAnimationCurrentFrame(bodyID) == start) // we have passed the end of the animation
			{
				// we change state back to standing
				getAnimationInfoFromDB(gameMgr->getBodyViewID(myID), "stand", start, end, frame_speed, loop);
				gMgr->setAnimationFrameLoop(bodyID, start, end);
				gMgr->setAnimationFrameSpeed(bodyID, frame_speed);
				gMgr->setAnimationLoop(bodyID, loop);

				gameMgr->setState(myID, 1);
			}

		}
		break;

	default:
		break;
	}
}

void MainClientState::checkAndSendUpdate(float deltaTime)
{
	netUpdateTimer -= deltaTime;

	if(isNetActive && netUpdateTimer < 0 && (hasMoved || hasTurned || justStopped || lockOnChange))
	{
		netUpdateTimer = 0.1;

		// we send the character update here
		netMgr->clearBitSteram();
		float x, y , z;
		float xAngle, yAngle, zAngle;

		// first the time stamp
		netMgr->writeUCharToBitStream((unsigned char)ID_TIMESTAMP);
		netMgr->writeSystemTimeStampToBitStream();

		// we put our update identifer
		netMgr->writeUCharToBitStream((unsigned char)ID_UPDATE);

		// now our ID
		//netMgr->writeUIntToBitStream(myNetID);
		netMgr->writeUIntToBitStream(gameMgr->getResourceID(myID, "netID"));

		// we put the rest of the data

		// first we check if moved
		netMgr->writeBoolToBitStream(hasMoved || justStopped);
		if(hasMoved || justStopped)
		{
			gameMgr->getPosition(myID, x, y, z);

			netMgr->writeFloatToBitStream(x);
			netMgr->writeFloatToBitStream(y);
			netMgr->writeFloatToBitStream(z);

			netMgr->writeBoolToBitStream(justStopped);
		}

		// now we check for turning
		netMgr->writeBoolToBitStream(hasTurned || lockOnChange);
		if(hasTurned || lockOnChange)
		{
			gameMgr->getRotation(myID, xAngle, yAngle, zAngle);

			netMgr->writeFloatToBitStream(yAngle);
		}

		// now we send
		netMgr->sendBitStreamToAll(HIGH_PRIORITY, RELIABLE_ORDERED, 0);

		if(!hasMoved && justStopped)
		{
			justStopped = false;
		}

		hasMoved = false;
		hasTurned = false;
		lockOnChange = false;
	}
}

void MainClientState::updateInterface(void)
{
	core::stringw mouseText = L"";
	mouseText += gameMgr->getName(myID).c_str();
	if(frezz)
	{
		mouseText += L" (frozen)";
	}
	mouseText += L"\nHP: ";
	mouseText += gameMgr->getHP(myID);
	mouseText += L"/";
	mouseText += gameMgr->getMaxHP(myID);
	mouseText += L"\nAP: ";
	mouseText += gameMgr->getAP(myID);
	mouseText += L"/";
	mouseText += gameMgr->getMaxAP(myID);
	if(apRunLimit < 0)
	{
		mouseText += L" (AP down)";
	}
	if(apBlockLimit < 0)
	{
		mouseText += L" (AP up)";
	}
	mouseText += L"\n\n";

	// if we are lock on to someone, we show the HP too
	unsigned int targetID = gameMgr->getTarget(myID);
	if(targetID)
	{
		mouseText += gameMgr->getName(targetID).c_str();
		mouseText += L"\nHP: ";
		mouseText += gameMgr->getHP(targetID);
		mouseText += L"/";
		mouseText += gameMgr->getMaxHP(targetID);
		mouseText += L"\n\n";
	}

	//mouseText += "state = ";
	//mouseText += gameMgr->getState(myID);
	//mouseText += "\nframe: ";
	//mouseText += gMgr->getAnimationCurrentFrame(gameMgr->getResourceID(myID, "bodyID"));
	//mouseText += "/";
	//mouseText += modelDataList[1]->anime_pain_end;

	//if(targetID)
	//{
	//	float range;
	//	float startX, startY, startZ;
	//	float endX, endY, endZ;
	//	float dirX, dirY, dirZ;

	//	gameMgr->getPosition(targetID, startX, startY, startZ);
	//	gameMgr->getPosition(myID, endX, endY, endZ);

	//	dirX = endX - startX;
	//	dirY = endY - startY;
	//	dirZ = endZ - startZ;

	//	range = gMgr->vectorLength(dirX, dirY, dirZ);

	//	mouseText += "\n\nrange = ";
	//	mouseText += range;
	//}

	gMgr->setGUIText(mouseStaticID, mouseText.c_str());
}

void MainClientState::updateNetObjects(float deltaTime)
{

	std::map<unsigned int, unsigned int>::iterator itr;
	for(itr = netObjectList.begin(); itr != netObjectList.end(); itr++)
	{
		unsigned int currentObj = itr->second;
		float x, y, z;

		gameMgr->getPosition(currentObj, x, y, z);

		switch(gameMgr->getState(currentObj))
		{
		case 1: // we are completely stopped
			// do nothing
			break;

		case 2: // we are moving
			{
				// set the data

				float x, y, z;
				float dirX, dirY, dirZ;
				float endX, endY, endZ;

				gameMgr->getPosition(currentObj, x, y, z);
				dirX = gameMgr->getMoveTowardX(currentObj);
				dirY = gameMgr->getMoveTowardY(currentObj);
				dirZ = gameMgr->getMoveTowardZ(currentObj);
				endX = gameMgr->getMoveToX(currentObj);
				endY = gameMgr->getMoveToY(currentObj);
				endZ = gameMgr->getMoveToZ(currentObj);

				// we move forward

				x += deltaTime * dirX * speed;
				y += deltaTime * dirY * speed;
				z += deltaTime * dirZ * speed;

				// we set the game object
				gameMgr->setPosition(currentObj, x, 0, z);
				gMgr->setNodePosition(gameMgr->getResourceID(currentObj, "bodyID"), x, gameMgr->getYOffset(currentObj), z);

				// check if we pass the end point
				// we only need to check one point. if we are there, then all points have been reached
				if(dirX != 0)
				{
					if((dirX > 0 && x > endX) || (dirX < 0 && x < endX)) // have we passed the end point
					{
						gameMgr->setState(currentObj, 3); // we set our object to keep moivng forward
					}
				}
				else if(dirY != 0)
				{
					if((dirY > 0 && y > endY) || (dirY < 0 && y < endY)) // have we passed the end point
					{
						gameMgr->setState(currentObj, 3); // we set our object to keep moivng forward
					}
				}
				else if(dirZ != 0)
				{
					if((dirZ > 0 && z > endZ) || (dirZ < 0 && z < endZ)) // have we passed the end point
					{
						gameMgr->setState(currentObj, 3); // we set our object to keep moivng forward
					}
				}
			}
			break;

		case 3: // we didn't get an update to stop, so we keep moving
			{
				// we keep moivng, no need to check for reaching the end point

				// set the data

				float x, y, z;
				float dirX, dirY, dirZ;
				float endX, endY, endZ;

				gameMgr->getPosition(currentObj, x, y, z);
				dirX = gameMgr->getMoveTowardX(currentObj);
				dirY = gameMgr->getMoveTowardY(currentObj);
				dirZ = gameMgr->getMoveTowardZ(currentObj);
				endX = gameMgr->getMoveToX(currentObj);
				endY = gameMgr->getMoveToY(currentObj);
				endZ = gameMgr->getMoveToZ(currentObj);

				// we move forward

				x += deltaTime * dirX * speed;
				y += deltaTime * dirY * speed;
				z += deltaTime * dirZ * speed;

				// we set the game object
				gameMgr->setPosition(currentObj, x, 0, z);
				gMgr->setNodePosition(gameMgr->getResourceID(currentObj, "bodyID"), x, gameMgr->getYOffset(currentObj), z);

			}
			break;

		case 4: // stopping
			{
				// set the data

				float x, y, z;
				float dirX, dirY, dirZ;
				float endX, endY, endZ;

				gameMgr->getPosition(currentObj, x, y, z);
				dirX = gameMgr->getMoveTowardX(currentObj);
				dirY = gameMgr->getMoveTowardY(currentObj);
				dirZ = gameMgr->getMoveTowardZ(currentObj);
				endX = gameMgr->getMoveToX(currentObj);
				endY = gameMgr->getMoveToY(currentObj);
				endZ = gameMgr->getMoveToZ(currentObj);

				// we move forward

				x += deltaTime * dirX * speed;
				y += deltaTime * dirY * speed;
				z += deltaTime * dirZ * speed;

				// check if we pass the end point
				// we only need to check one point. if we are there, then all points have been reached
				if(dirX != 0)
				{
					if((dirX > 0 && x > endX) || (dirX < 0 && x < endX)) // have we passed the end point
					{
						x = endX;
						y = endY;
						z = endZ;
						gameMgr->setState(currentObj, 1); // we stop
					}
				}
				else if(dirY != 0)
				{
					if((dirY > 0 && y > endY) || (dirY < 0 && y < endY)) // have we passed the end point
					{
						x = endX;
						y = endY;
						z = endZ;
						gameMgr->setState(currentObj, 1); // we stop
					}
				}
				else if(dirZ != 0)
				{
					if((dirZ > 0 && z > endZ) || (dirZ < 0 && z < endZ)) // have we passed the end point
					{
						x = endX;
						y = endY;
						z = endZ;
						gameMgr->setState(currentObj, 1); // we stop
					}
				}

				// we set the game object
				gameMgr->setPosition(currentObj, x, 0, z);
				gMgr->setNodePosition(gameMgr->getResourceID(currentObj, "bodyID"), x, gameMgr->getYOffset(currentObj), z);

				// we set the animation to stop, if we stoped
				if(gameMgr->getState(currentObj) == 1)
				{
					int start, end;
					float frame_speed;
					bool loop;

					unsigned int modelNumber = gameMgr->getBodyViewID(currentObj);
					unsigned int bodyID = gameMgr->getResourceID(currentObj, "bodyID");

					getAnimationInfoFromDB(modelNumber, "stand", start, end, frame_speed, loop);
					gMgr->setAnimationFrameLoop(bodyID, start, end);
					gMgr->setAnimationFrameSpeed(bodyID, frame_speed);
					gMgr->setAnimationLoop(bodyID, loop);
				}
			}
			break;

		case 5: // we are attacking
			{
				int start, end;
				float frame_speed;
				bool loop;

				unsigned int bodyID = gameMgr->getResourceID(currentObj, "bodyID");
				getAnimationInfoFromDB(gameMgr->getBodyViewID(currentObj), "attack_1", start, end, frame_speed, loop);

				if(gMgr->getAnimationCurrentFrame(bodyID) == end) // we have passed the end of the animation
				{
					// we set everything to go back to standing

					getAnimationInfoFromDB(gameMgr->getBodyViewID(currentObj), "stand", start, end, frame_speed, loop);
					gMgr->setAnimationFrameLoop(bodyID, start, end);
					gMgr->setAnimationFrameSpeed(bodyID, frame_speed);
					gMgr->setAnimationLoop(bodyID, loop);

					gameMgr->setState(currentObj, 1);

					// by the end of the animation, we check if were hit
					// first we check if we are in range of the attack
					// then we check if are facing the attack
					float range;
					float startX, startY, startZ;
					float endX, endY, endZ;
					float dirX, dirY, dirZ;
					float xAngle, yAngle, zAngle;
					float dirxAngle, diryAngle, dirzAngle;

					gameMgr->getPosition(currentObj, startX, startY, startZ);
					gameMgr->getPosition(myID, endX, endY, endZ);
					gameMgr->getRotation(currentObj, xAngle, yAngle, zAngle);

					dirX = endX - startX;
					dirY = endY - startY;
					dirZ = endZ - startZ;

					range = gMgr->vectorLength(dirX, dirY, dirZ);

					dirxAngle = dirX;
					diryAngle = dirY;
					dirzAngle = dirZ;

					gMgr->directionToRotation(dirxAngle, diryAngle, dirzAngle);

					// we check range less than 30
					// we check if attacker is facing within 20 angles of our character
					if(range < 30 && yAngle > diryAngle - 10 && yAngle < diryAngle + 10)
					{
						// a hit was made

						// now we check if we are blocking
						// if we are then we check if we are blocking in the right direction
						// after that, we modify the damage by using the attacker's AP

						bool hit = true;

						if(gameMgr->getState(myID) == 5 || gameMgr->getState(myID) == 6) // we are blocking
						{
							float xAngle, yAngle, zAngle;

							// we get our direction, which is the oppiste of the attacker direction
							diryAngle += 180;
							// we fix the angle to be between 0 and 360
							if(diryAngle > 360)
								diryAngle -= 360;

							// we get our yAngle
							gameMgr->getRotation(myID, xAngle, yAngle, zAngle);
							cout << "myID = " << myID << endl;
							cout << "xAngle = " << xAngle << endl;
							cout << "yAngle = " << yAngle << endl;
							cout << "zAngle = " << zAngle << endl;
							cout << "diryAngle = " << diryAngle << endl; 

							// we check if we are facing within 60 angles of the attacker
							if(yAngle > diryAngle - 30 && yAngle < diryAngle + 30)
							{
								// hit was blocked
								hit = false;
								cout << "block success" << endl;
							}
						}

						if(hit)
						{
							int hp = gameMgr->getHP(myID);
							float ap = gameMgr->getActionPower(currentObj);

							gameMgr->setHP(myID, hp - (1 + 40 * ap / 100.0)); // for now attacking cost 40 HP x AP% (100% = full power). We add 1 so to make sure we don't make zero damage
							updateHP();

							if(checkForDeath())
							{
								sendDeath();
							}
						}
					}
				}
			}
			break;

		case 7: // we are blocking
			{
			}
			break;

		case 8: // we are stopping the block
			{
				int start, end;
				float frame_speed;
				bool loop;

				unsigned int bodyID = gameMgr->getResourceID(currentObj, "bodyID");
				getAnimationInfoFromDB(gameMgr->getBodyViewID(currentObj), "block", start, end, frame_speed, loop);

				if(gMgr->getAnimationCurrentFrame(bodyID) == start) // we have passed the end of the animation
				{
					// we set everything to go back to standing

					getAnimationInfoFromDB(gameMgr->getBodyViewID(currentObj), "stand", start, end, frame_speed, loop);
					gMgr->setAnimationFrameLoop(bodyID, start, end);
					gMgr->setAnimationFrameSpeed(bodyID, frame_speed);
					gMgr->setAnimationLoop(bodyID, loop);

					gameMgr->setState(currentObj, 1);
				}
			}
			break;

		default:
			break;
		}
	}
}

unsigned int MainClientState::findNearestObject(void)
{
	unsigned int targetID = 0;
	float distance = 1000000; // we will use this to store the closest player to us

	// first we check if we actually have anyone to lock on too
	if(!netObjectList.empty())
	{
		float x, y, z;
		float ourX, ourY, ourZ;
		gameMgr->getPosition(myID, ourX, ourY, ourZ);

		std::map<unsigned int, unsigned int>::iterator itr;

		for(itr = netObjectList.begin(); itr != netObjectList.end(); itr++)
		{
			unsigned int nextID = itr->second;

			gameMgr->getPosition(nextID, x, y, z);

			float newLength = gMgr->vectorLength(x - ourX, y - ourY, z - ourZ);

			if(newLength < distance)
			{
				targetID = nextID;
				distance = newLength;
			}
		}
	}

	return targetID;
}

void MainClientState::lockOnTarget(unsigned int objectID)
{
	// if we don't have a target, we set one
	// if we already have a target, we check if its not the same as the new target and set it
	unsigned int targetID = gameMgr->getTarget(myID);

	if(targetID != objectID && objectID != 0)
	{
		gameMgr->setTarget(myID, objectID);
		sendLockOnTarget(objectID);
	}
	// if we objectID = 0, then we unlock our target
	else if(targetID != 0 && objectID == 0)
	{
		gameMgr->setTarget(myID, 0);
		sendLockOnTarget(0);
	}
}

void MainClientState::sendLockOnTarget(unsigned int objectID)
{
	netMgr->clearBitSteram();

	netMgr->writeUCharToBitStream((unsigned char) ID_REQUEST_LOCK_ON);
	// we put our net ID
	netMgr->writeUIntToBitStream(gameMgr->getResourceID(myID, "netID"));
	// now we put our target net ID
	netMgr->writeUIntToBitStream(gameMgr->getResourceID(objectID, "netID"));

	// now we send
	netMgr->sendBitStreamToAll(HIGH_PRIORITY, RELIABLE_ORDERED, 0);
}

void MainClientState::updateHP(void)
{
	netMgr->clearBitSteram();

	netMgr->writeUCharToBitStream((unsigned char) ID_UPDATE_HP);
	// we put our net ID
	netMgr->writeUIntToBitStream(gameMgr->getResourceID(myID, "netID"));
	// now we put the new HP
	netMgr->writeIntToBitStream(gameMgr->getHP(myID));

	// now we send
	netMgr->sendBitStreamToAll(HIGH_PRIORITY, RELIABLE_ORDERED, 0);
}

void MainClientState::attack(void)
{
	if(gameMgr->getState(myID) == 1 || gameMgr->getState(myID) == 2)
	{
		// we set the animation

		int start, end;
		float frame_speed;
		bool loop;

		unsigned int bodyID = gameMgr->getResourceID(myID, "bodyID");

		getAnimationInfoFromDB(gameMgr->getBodyViewID(myID), "attack_1", start, end, frame_speed, loop);
		gMgr->setAnimationFrameLoop(bodyID, start, end);
		gMgr->setAnimationFrameSpeed(bodyID, frame_speed);
		gMgr->setAnimationLoop(bodyID, loop);

		// we change our state
		gameMgr->setState(myID, 3);
	}
}

void MainClientState::sendAttack(void)
{
	netMgr->clearBitSteram();

	// we send a timestamp
	netMgr->writeUCharToBitStream((unsigned char)ID_TIMESTAMP);
	netMgr->writeSystemTimeStampToBitStream();

	// now we send the ID_START_ATTACK, our netID, position and direciton (yAngle)
	netMgr->writeUCharToBitStream((unsigned char)ID_START_ATTACK);
	netMgr->writeUIntToBitStream(gameMgr->getResourceID(myID, "netID"));

	float x, y, z, rx, ry, rz;
	gameMgr->getPosition(myID, x, y, z);
	gameMgr->getRotation(myID, rx, ry, rz);

	netMgr->writeFloatToBitStream(x);
	netMgr->writeFloatToBitStream(y);
	netMgr->writeFloatToBitStream(z);
	netMgr->writeFloatToBitStream(ry);

	// we also send our AP to be used to calculate damage
	netMgr->writeUIntToBitStream(gameMgr->getAP(myID));

	// now we send
	netMgr->sendBitStreamToAll(HIGH_PRIORITY, RELIABLE_ORDERED, 0);
}

void MainClientState::block(void)
{
	if(gameMgr->getState(myID) == 1 || gameMgr->getState(myID) == 2)
	{
		// we set the animation

		int start, end;
		float frame_speed;
		bool loop;

		unsigned int bodyID = gameMgr->getResourceID(myID, "bodyID");

		getAnimationInfoFromDB(gameMgr->getBodyViewID(myID), "block", start, end, frame_speed, loop);
		gMgr->setAnimationFrameLoop(bodyID, start, end);
		gMgr->setAnimationFrameSpeed(bodyID, frame_speed);
		gMgr->setAnimationLoop(bodyID, loop);

		// we change our state
		gameMgr->setState(myID, 5);
	}
}

void MainClientState::stopBlock(void)
{
	if(gameMgr->getState(myID) == 6) // we are suppose to be in blockking
	{
		// we set the animation, backward

		int start, end;
		float frame_speed;
		bool loop;

		unsigned int bodyID = gameMgr->getResourceID(myID, "bodyID");

		getAnimationInfoFromDB(gameMgr->getBodyViewID(myID), "block", start, end, frame_speed, loop);
		gMgr->setAnimationFrameLoop(bodyID, start, end);
		gMgr->setAnimationCurrentFrame(bodyID, end);
		gMgr->setAnimationFrameSpeed(bodyID, frame_speed * -1);
		gMgr->setAnimationLoop(bodyID, loop);

		// we change our state
		gameMgr->setState(myID, 7);
	}
}

void MainClientState::sendBlock(void)
{
	netMgr->clearBitSteram();

	// we send a timestamp
	netMgr->writeUCharToBitStream((unsigned char)ID_TIMESTAMP);
	netMgr->writeSystemTimeStampToBitStream();

	// now we send the ID_BLOCK_START, our netID, position and direciton (yAngle)
	netMgr->writeUCharToBitStream((unsigned char)ID_BLOCK_START);
	netMgr->writeUIntToBitStream(gameMgr->getResourceID(myID, "netID"));

	float x, y, z, rx, ry, rz;
	gameMgr->getPosition(myID, x, y, z);
	gameMgr->getRotation(myID, rx, ry, rz);

	netMgr->writeFloatToBitStream(x);
	netMgr->writeFloatToBitStream(y);
	netMgr->writeFloatToBitStream(z);
	netMgr->writeFloatToBitStream(ry);

	// now we send
	netMgr->sendBitStreamToAll(HIGH_PRIORITY, RELIABLE_ORDERED, 0);
}

void MainClientState::sendStopBlock(void)
{
	netMgr->clearBitSteram();

	// we send a timestamp
	netMgr->writeUCharToBitStream((unsigned char)ID_TIMESTAMP);
	netMgr->writeSystemTimeStampToBitStream();

	// now we send the ID_BLOCK_END, and our netID
	netMgr->writeUCharToBitStream((unsigned char)ID_BLOCK_END);
	netMgr->writeUIntToBitStream(gameMgr->getResourceID(myID, "netID"));

	// now we send
	netMgr->sendBitStreamToAll(HIGH_PRIORITY, RELIABLE_ORDERED, 0);
}

bool MainClientState::checkForDeath(void)
{
	bool dead = false;


	// first we check if our HP is the same as our MinHP, and we are not already dead
	// we change our state
	// then we play the animation

	if(gameMgr->getHP(myID) == gameMgr->getMinHP(myID) && gameMgr->getState(myID) != 4)
	{
		int start, end;
		float frame_speed;
		bool loop;

		gameMgr->setState(myID, 4);

		unsigned int bodyViewID = gameMgr->getBodyViewID(myID);
		unsigned int bodyID = gameMgr->getResourceID(myID, "bodyID");

		getAnimationInfoFromDB(bodyViewID, "dead", start, end, frame_speed, loop);
		gMgr->setAnimationFrameLoop(bodyID, start, end);
		gMgr->setAnimationFrameSpeed(bodyID, frame_speed);
		gMgr->setAnimationLoop(bodyID, loop);

		spawnTimer = 5; // stay dead for 5 seconds

		dead = true;
	}

	return dead;
}

void MainClientState::sendDeath(void)
{
	netMgr->clearBitSteram();

	// we send a timestamp
	netMgr->writeUCharToBitStream((unsigned char)ID_TIMESTAMP);
	netMgr->writeSystemTimeStampToBitStream();

	// now we send the ID_DEAD, our netID, position and direciton (yAngle)
	netMgr->writeUCharToBitStream((unsigned char)ID_DEAD);
	netMgr->writeUIntToBitStream(gameMgr->getResourceID(myID, "netID"));

	float x, y, z, rx, ry, rz;
	gameMgr->getPosition(myID, x, y, z);
	gameMgr->getRotation(myID, rx, ry, rz);

	netMgr->writeFloatToBitStream(x);
	netMgr->writeFloatToBitStream(y);
	netMgr->writeFloatToBitStream(z);
	netMgr->writeFloatToBitStream(ry);

	// now we send
	netMgr->sendBitStreamToAll(HIGH_PRIORITY, RELIABLE_ORDERED, 0);
}

void MainClientState::respawn(void)
{
	// we set our position at pos(0,0,0) and the stop state and animation

	int start, end;
	float frame_speed;
	bool loop;

	unsigned int bodyID = gameMgr->getResourceID(myID, "bodyID");
	unsigned int bodyDataID = gameMgr->getBodyViewID(myID);

	gameMgr->setPosition(myID, 0, 0, 0);
	gMgr->setNodePosition(bodyID, 0, gameMgr->getYOffset(myID), 0);

	gameMgr->setState(myID, 1);

	getAnimationInfoFromDB(bodyDataID, "stand", start, end, frame_speed, loop);
	gMgr->setAnimationFrameLoop(bodyID, start, end);
	gMgr->setAnimationFrameSpeed(bodyID, frame_speed);
	gMgr->setAnimationLoop(bodyID, loop);

	// and now we refill our HP
	gameMgr->setHP(myID, gameMgr->getMaxHP(myID));
}

void MainClientState::sendRespawn(void)
{
	netMgr->clearBitSteram();

	// we just need to send ID_RESPAWN. Later we will pass a spwan point ID so we would know where he would respwan
	netMgr->writeUCharToBitStream((unsigned char)ID_RESPAWN);
	netMgr->writeUIntToBitStream(gameMgr->getResourceID(myID, "netID"));
	netMgr->writeUIntToBitStream(gameMgr->getHP(myID));

	// now we send
	netMgr->sendBitStreamToAll(HIGH_PRIORITY, RELIABLE_ORDERED, 0);

	// we also need to clear any target we are locked on too
	// no need to inform anyone, respawning will do that automatically
	gameMgr->setTarget(myID, 0);
}

void MainClientState::updateGolemArea(float deltaTime)
{
	float x, y, z;
	gameMgr->getPosition(myID, x, y, z);

	if(inArena) // if we are in the golem area
	{
		if(z < arenaZLimit - 300) // did we go back to house limit, we add some space in case we have just entered the golem area before
		{
			inArena = false; // we in the house area

			// we start fading out the battel theme
			fadeState = 3;
		}
	}
	else // if are in the house area
	{
		if(z > arenaZLimit) // did we pass the golem limit
		{
			inArena = true; // we in the golem area

			// we start fading out the main theme
			fadeState = 1;
		}
	}

	// now we check the fading
	switch(fadeState) // if 0 then no fading is needed
	{
	case 1: // fade out main

		// we lower the volue

		soundVolume -= soundFadeSpeed * deltaTime;

		// check if we pass the 0
		if(soundVolume < 0) // we change the to battel sound, and change sound state to fade in battel sound
		{
			soundVolume = 0;

			sMgr->playSound(mainThemeChannleID, dbMgr->getDataAsInt("battel_theme_id"));
			sMgr->setVolum(mainThemeChannleID, soundVolume);

			fadeState = 2; // we go to fade in battel sound
		}
		else // still not there, we set the volume
		{
			sMgr->setVolum(mainThemeChannleID, soundVolume);
		}

		break;

	case 2: // fade in battel

		// we increase the volue

		soundVolume += soundFadeSpeed * deltaTime;

		// check if we pass the 1
		if(soundVolume > 1) // we reached the limit, switch to no fade now
		{
			soundVolume = 1;

			sMgr->setVolum(mainThemeChannleID, soundVolume);

			fadeState = 0;
		}
		else // still not there, we set the volume
		{
			sMgr->setVolum(mainThemeChannleID, soundVolume);
		}

		break;

	case 3: // fade out battel

		// we lower the volue

		soundVolume -= soundFadeSpeed * deltaTime;

		// check if we pass the 0
		if(soundVolume < 0) // we change the to main sound, and change sound state to fade in battel sound
		{
			soundVolume = 0;

			sMgr->playSound(mainThemeChannleID, dbMgr->getDataAsInt("main_theme_id"));
			sMgr->setVolum(mainThemeChannleID, soundVolume);

			fadeState = 4; // we go to fade in main sound
		}
		else // still not there, we set the volume
		{
			sMgr->setVolum(mainThemeChannleID, soundVolume);
		}

		break;

	case 4: // fade in main

		// we increase the volue

		soundVolume += soundFadeSpeed * deltaTime;

		// check if we pass the 1
		if(soundVolume > 1) // we reached the limit, switch to no fade now
		{
			soundVolume = 1;

			sMgr->setVolum(mainThemeChannleID, soundVolume);

			fadeState = 0;
		}
		else // still not there, we set the volume
		{
			sMgr->setVolum(mainThemeChannleID, soundVolume);
		}

		break;
	}
}

void MainClientState::shootFireBall(void)
{
	// we check fire ball cool down, and max number of fire balls
	if(fireBallReady < 0 && currentNumberOfFireBalls < maxFireBalls)
	{
		fireBallReady = fireBallCoolDown;

		unsigned int id = prepareFireBall();

		setFireBallSpeed(id);

		fireBallList.push_back(id);
	}
}

unsigned int MainClientState::prepareFireBall(void)
{
	unsigned int id;

	id = gameMgr->createGameObject();
	gameMgr->addGameComponent(id, ComponentType::MOVEMENT_COMP);
	gameMgr->addGameComponent(id, ComponentType::MOVE_TO_COMP);

	// we craete a billboard as head
	unsigned int billboardID;
	billboardID = gMgr->createBillboard();
	gMgr->setBillboardSize(billboardID, 20, 20);
	gMgr->setBillBoardColor(billboardID, 255, 255, 255, 255, 255, 255, 255, 255);
	gMgr->setNodeTexture(billboardID, 0, "ourStuff/fireball.bmp");
	gMgr->setNodeMaterialType(billboardID, 12); // EMT_TRANSPARENT_ADD_COLOR = dark area is transparnt
	gMgr->setNodeMaterialFlag(billboardID, 8, false); // EMF_LIGHTING = control is lighted

	gameMgr->addResourceID(id, "bbID", billboardID);

	// and a particle smoke as tail
	unsigned int partID;
	partID = gMgr->createParticle();
	gMgr->createParticlePointEmitter(partID, 0, 0.006, 0, 50, 100, 255, 255, 255, 255, 255, 255, 255, 255, 5000, 5000, 20, 20, 20, 40, 40);
	gMgr->addParticleFadeOutAffector(partID, 0, 0, 0, 0, 5000);
	gMgr->setNodeTexture(partID, 0, "ourStuff/smoke2.jpg");
	gMgr->setNodeMaterialType(partID, 12); // EMT_TRANSPARENT_ADD_COLOR = dark area is transparnt
	gMgr->setNodeMaterialFlag(partID, 8, false); // EMF_LIGHTING = control is lighted

	gameMgr->addResourceID(id, "tailID", partID);

	gameMgr->setState(id, 1); //1 == just created

	currentNumberOfFireBalls++;

	return id;
}

void MainClientState::setFireBallSpeed(unsigned int id)
{
	// we get player position as start for the fire ball
	// we get the speed
	// we get the direction for the fire ball from the camera
	// we set the speed and final distanation

	float playerX, playerY, playerZ;
	float fireSpeed;
	float dirX, dirY, dirZ;
	float finalX, finalY, finalZ;

	gameMgr->getPosition(myID, playerX, playerY, playerZ);

	// for fire ball speed, we add the player speed if he is moving
	fireSpeed = fireBallSpeed;
	// we check if player is moving
	if(gameMgr->getState(myID) == 2)
	{
		fireSpeed += speed;
	}

	gameMgr->getRotation(cameraID, dirX, dirY, dirZ);
	// we turn into direciton
	gMgr->rotationToDirection(dirX, dirY, dirZ);
	dirY = 0; // because we want the fire ball to fly horizantally
	gMgr->normalizeVector(dirX, dirY, dirZ);

	gameMgr->setSpeedX(id, fireSpeed * dirX);
	gameMgr->setSpeedY(id, fireSpeed * dirY);
	gameMgr->setSpeedZ(id, fireSpeed * dirZ);

	// we use the fire ball life span to figure out how far the fire ball will travel
	finalX = playerX + fireSpeed * dirX * fireBallLifeSpan;
	//finalY = playerY + fireSpeed * dirY * fireBallLifeSpan;
	finalZ = playerZ + fireSpeed * dirZ * fireBallLifeSpan;
	gameMgr->setMoveToX(id, finalX);
	//gameMgr->setMoveToY(id, finalY);
	gameMgr->setMoveToZ(id, finalZ);

	gameMgr->setPosition(id, playerX, playerY ,playerZ);
	gMgr->setNodePosition(gameMgr->getResourceID(id, "bbID"), playerX, playerY + 15, playerZ);
	gMgr->setNodePosition(gameMgr->getResourceID(id, "tailID"), playerX, playerY + 15, playerZ);

	gameMgr->setState(id, 2); // 2 == we are moving
}

void MainClientState::updateFireBalls(float deltaTime)
{
	if(fireBallReady > 0)
	{
		fireBallReady -= deltaTime;
	}

	// we go fire ball by fire ball
	// we move first
	// then we check if we reached our distantion

	int listSize = fireBallList.size();

	for(int i = 0; i < listSize && currentNumberOfFireBalls > 0; i++)
	{
		unsigned int fbID = fireBallList[i];

		if(fbID != 0)
		{
			if(gameMgr->getState(fbID) == 2) // only move ready fireballs
			{
				float startX, startY, startZ, endX, endY, endZ, speedX, speedY, speedZ;

				gameMgr->getPosition(fbID, startX, startY, startZ);
				endX = gameMgr->getMoveToX(fbID);
				endY = gameMgr->getMoveToY(fbID);
				endZ = gameMgr->getMoveToZ(fbID);
				speedX = gameMgr->getSpeedX(fbID);
				speedY = gameMgr->getSpeedY(fbID);
				speedZ = gameMgr->getSpeedZ(fbID);

				startX += speedX * deltaTime;
				startZ += speedZ * deltaTime;

				// we check if reached our distantion
				if((speedX > 0 && startX > endX) || (speedX < 0 && startX < endX) || (speedZ > 0 && startZ > endZ) || (speedZ < 0 && startZ < endZ)) // we reached
				{
					// we delete the game object and its resources
					gMgr->removeNode(gameMgr->getResourceID(fbID, "bbID"));
					gMgr->removeNode(gameMgr->getResourceID(fbID, "tailID"));

					gameMgr->deleteGameObject(fbID);

					//fireBallList.erase(fireBallList.begin() + i);
					fireBallList[i] = 0;
					currentNumberOfFireBalls--;
				}
				else // still haven't reached
				{
					gameMgr->setPosition(fbID, startX, startY, startZ);
					gMgr->setNodePosition(gameMgr->getResourceID(fbID, "bbID"), startX, startY + 15, startZ);
					gMgr->setNodePosition(gameMgr->getResourceID(fbID, "tailID"), startX, startY + 15, startZ);
				}
			}
		}
	}
}

void MainClientState::getModelAndTextureFromDB(unsigned int id, std::string &modelPath,  std::vector<std::string> &texturePathList, float &yOffset, float &rotationOffset, float &scaleOffset)
{
	// get row
	// get model and texture
	std::string rowID;
	std::string path;
	std::vector<std::string> resultList;
	int modelID, textureLayerID;
	int layerSize;
	std::vector<unsigned int> layerList;

	if(dbMgr->searchColumnForUInt("body_list", "id", id, resultList)) // get the rowID
	{
		rowID = resultList[0];

		// we get the id of the model and textureLayerList
		modelID = dbMgr->getRecordAsInt("body_list", "model", (char*)rowID.c_str());
		textureLayerID = dbMgr->getRecordAsInt("body_list", "texture_layer_list", (char*)rowID.c_str());
		yOffset = dbMgr->getRecordAsFloat("body_list", "y_offset", (char*)rowID.c_str());
		rotationOffset = dbMgr->getRecordAsFloat("body_list", "rotation_offset", (char*)rowID.c_str());
		scaleOffset = dbMgr->getRecordAsFloat("body_list", "scale_offset", (char*)rowID.c_str());
	}
	else
	{
	}
	resultList.clear();

	// we get the model path first
	if(dbMgr->searchColumnForString("model_list", "id", "path", resultList)) // get the rowID
	{
		rowID = resultList[0];

		// we get the path here
		path = dbMgr->getRecordAsString("model_list", "path", (char*)rowID.c_str());
		path += "/";
	}
	else
	{
	}
	resultList.clear();

	// now we get the model file
	if(dbMgr->searchColumnForUInt("model_list", "id", modelID, resultList)) // get the rowID
	{
		rowID = resultList[0];

		// we get the full path here
		modelPath = path + dbMgr->getRecordAsString("model_list", "path", (char*)rowID.c_str());
	}
	else
	{
	}
	resultList.clear();

	// now we with the texture layer list
	// some models use more than one layer of texture
	// in our game, we will work with 10 layers
	// we get how many layers this body has
	// then we take each layer's ID
	if(dbMgr->searchColumnForUInt("texture_layer_list", "id", textureLayerID, resultList)) // get the rowID
	{
		rowID = resultList[0];

		// we get the layer size
		layerSize = dbMgr->getRecordAsInt("texture_layer_list", "number_of_layers", (char*)rowID.c_str());

		// now we get each layer's ID
		for(int i = 0; i < layerSize; i++)
		{
			// we get the column name
			std::string columnName = "layer_";
			std::stringstream oss; // we use this to turn int to string
			oss << i;
			columnName += oss.str();

			// now we get the texutre id
			layerList.push_back(dbMgr->getRecordAsUInt("texture_layer_list", (char*)columnName.c_str(), (char*)rowID.c_str()));
		}
	}
	else
	{
	}
	resultList.clear();

	// now we work with texutre, get the path first
	if(dbMgr->searchColumnForString("texture_list", "id", "path", resultList)) // get the rowID
	{
		rowID = resultList[0];

		// we get the path here
		path = dbMgr->getRecordAsString("texture_list", "path", (char*)rowID.c_str());
		path += "/";
	}
	else
	{
	}
	resultList.clear();

	// now we get the texture files
	for(int i = 0; i < layerSize; i++)
	{
		if(dbMgr->searchColumnForUInt("texture_list", "id", layerList[i], resultList)) // get the rowID
		{
			rowID = resultList[0];

			// we get the full path here
			//texturePath = path + dbMgr->getRecordAsString("texture_list", "path", (char*)rowID.c_str());
			std::string texturePath = path + dbMgr->getRecordAsString("texture_list", "path", (char*)rowID.c_str());
			texturePathList.push_back(texturePath);
		}
		else
		{
		}
		resultList.clear();
	}
}

void MainClientState::getAnimationInfoFromDB(unsigned int id, char *animeName, int &start, int &end, float &speed, bool &loop)
{
	// first we get the animation group id from the body list
	// then we get the animatation id from the animation group list, by using [animeName] as the column name
	// then we get the rest of the data from the animation list

	std::string rowID;
	std::vector<std::string> resultList;
	int animeGroupID;
	int animeID;

	if(dbMgr->searchColumnForInt("body_list", "id", id, resultList))
	{
		rowID = resultList[0];

		animeGroupID = dbMgr->getRecordAsInt("body_list", "anime_group", (char*)rowID.c_str());
	}
	else
	{
		// throw something
	}

	// we got the animation group ID
	// we use that plus the animeName = column name

	resultList.clear();
	if(dbMgr->searchColumnForInt("anime_group", "id", animeGroupID, resultList))
	{
		rowID = resultList[0];

		animeID = dbMgr->getRecordAsInt("anime_group", animeName, (char*)rowID.c_str());
	}
	else
	{
		// throw something
	}

	// now we got the animeation id
	// next we get the row ID from the animation list
	// then we fill our data

	resultList.clear();
	if(dbMgr->searchColumnForInt("anime_list", "id", animeID, resultList))
	{
		rowID = resultList[0];

		// now we take what we want
		start = dbMgr->getRecordAsInt("anime_list", "start", (char*)rowID.c_str());
		end = dbMgr->getRecordAsInt("anime_list", "end", (char*)rowID.c_str());
		speed = dbMgr->getRecordAsInt("anime_list", "frame_speed", (char*)rowID.c_str()); // change this to float when we get it to work
		loop = dbMgr->getRecordAsBool("anime_list", "loop", (char*)rowID.c_str());
	}
	else
	{
		// throw something
	}
}
