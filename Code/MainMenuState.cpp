#include "MainMenuState.h"
#include "MainClientState.h"
#include "OurGameMessages.h"

#include <iostream>

using namespace std;

MainMenuState::MainMenuState()
{
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::Init(void)
{
	game = Game::getInstance();
	gMgr = GraphicManager::getInstance();
	iMgr = InputManager::getInstance();
	dbMgr = DataBaseManager::getInstance();
	netMgr = NetworkManager::getInstance();

	//gMgr->test();

	holdingESC = true;

	gMgr->setBackgroundColor(255, 125, 125, 125);

	//// draw some buttons. Only the quit button will quit the game for now
	//float screenWidth = game->getScreenWidth();
	//float screenHeight = game->getScreenHeight();

	//float startScaleW = 0.25; // button width
	//float startScaleH = 0.125; // button height
	//float startX1 = (screenWidth - screenWidth * startScaleW) / 2; // set it in teh middle or screen
	//float startY1 = screenHeight / 2; // set it in the lower half of the screen
	//float startX2 = startX1 + screenWidth * startScaleW ; // button width
	//float startY2 = startY1 + screenHeight * startScaleH; // button height

	//float optionsScaleW = 0.25; // button width
	//float optionsScaleH = 0.125; // button height
	//float optionsX1 = (screenWidth - screenWidth * optionsScaleW) / 2; // set it in teh middle or screen
	//float optionsY1 = startY2 + 10;
	//float optionsX2 = optionsX1 + screenWidth * optionsScaleW ; // button width
	//float optionsY2 = optionsY1 + screenHeight * optionsScaleH; // button height

	//float quitScaleW = 0.25; // button width
	//float quitScaleH = 0.125; // button height
	//float quitX1 = (screenWidth - screenWidth * quitScaleW) / 2; // set it in teh middle or screen
	//float quitY1 = optionsY2 + 10;
	//float quitX2 = quitX1 + screenWidth * quitScaleW ; // button width
	//float quitY2 = quitY1 + screenHeight * quitScaleH; // button height

	//startButtonID = gMgr->createButton(startX1, startY1, startX2, startY2);
	////gMgr->setGUIPosition(startButtonID, startX, startY, screenWidth * startW, screenHeight * startH);
	//gMgr->setGUIText(startButtonID, L"START");

	//optionsButtonID = gMgr->createButton(optionsX1, optionsY1, optionsX2, optionsY2);
	////gMgr->setGUIPosition(optionsButtonID, optionsX, optionsY, screenWidth * optionsW, screenHeight * optionsH);
	//gMgr->setGUIText(optionsButtonID, L"OPITIONS");

	//quitButtonID = gMgr->createButton(quitX1, quitY1, quitX2, quitY2);
	////gMgr->setGUIPosition(quitButtonID, quitX, quitY, screenWidth * quitW, screenHeight * quitH);
	//gMgr->setGUIText(quitButtonID, L"QUIT");



	// we create our main menu
	// for now we can enter the IP and Port to the server
	// only the user name is required. We ignor the password for now

	unsigned int textID = 0; // temp to control static texts

	// some math so to align everything

	float screenWidth = game->getScreenWidth();
	float screenHeight = game->getScreenHeight();

	// first we get our group x and y. If we move this we move the whole menu
	// for now it should be 30% of the screen's width and in the middle. 60% of the screen height and alinged to the bottom
	float groupX = screenWidth * 0.35;
	float groupY = screenHeight * 0.40;
	float groupW = screenWidth * 0.30;
	float groupH = screenHeight * 0.60;

	// we will have 7 lines, 4 edit boxes and 3 buttons
	// That's about 14% height for each. I'll give each one 12% while having a 2% space between them
	// Also, for the edit boxes, they only have 80% width, the rest is for the static text
	float editBoxW = 0.80;
	float editBoxH = 0.14;
	float padding = 0.02;
	int line = 0; // which line are we drawing

	// the uesr name edit box
	userNameID = gMgr->createEditBox(groupX, line * (groupH * editBoxH) + groupY, (groupW * editBoxW) + groupX, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	//gMgr->setGUIFocus(userNameID);
	textID = gMgr->createStaticText((groupW * editBoxW) + groupX,  line * (groupH * editBoxH) + groupY, groupX + groupW, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	gMgr->setGUIText(textID, L"User Name");

	// the password edit box
	line++; // second line
	passwordID = gMgr->createEditBox(groupX, line * (groupH * editBoxH) + groupY, (groupW * editBoxW) + groupX, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	textID = gMgr->createStaticText((groupW * editBoxW) + groupX,  line * (groupH * editBoxH) + groupY, groupX + groupW, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	gMgr->setGUIText(textID, L"Password");
	//gMgr->setEditBoxPasswordBox(textID);

	// the IP edit box
	//line++;
	//ipID = gMgr->createEditBox(groupX, line * (groupH * editBoxH) + groupY, (groupW * editBoxW) + groupX, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	//textID = gMgr->createStaticText((groupW * editBoxW) + groupX,  line * (groupH * editBoxH) + groupY, groupX + groupW, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	//gMgr->setGUIText(textID, L"IP");

	// the IP edit box
	//line++;
	//portID = gMgr->createEditBox(groupX, line * (groupH * editBoxH) + groupY, (groupW * editBoxW) + groupX, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	//textID = gMgr->createStaticText((groupW * editBoxW) + groupX,  line * (groupH * editBoxH) + groupY, groupX + groupW, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	//gMgr->setGUIText(textID, L"Port");

	//////////////////
	//////////////////
	//// we put the color selection part here
	//////////////////
	//////////////////
	//line++;
	//// first the color area
	//colorID = gMgr->createStaticText(groupX, line * (groupH * editBoxH) + groupY, (groupW * 0.25) + groupX, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	//gMgr->setStaticTextBackgroundColor(colorID, 255, 255, 255, 255);
	//// the r edit static box
	//redColorID = gMgr->createEditBox((groupW * 0.25) + groupX, line * (groupH * editBoxH) + groupY, (groupW * (0.25 + 0.25/2)) + groupX, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	//gMgr->setGUIText(redColorID, L"255");
	//textID = gMgr->createStaticText((groupW * (0.25 + 0.25/2)) + groupX, line * (groupH * editBoxH) + groupY, (groupW * (0.25 + 0.25)) + groupX, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	//gMgr->setGUIText(textID, L"Red");
	//// the g edit static box
	//greenColorID = gMgr->createEditBox((groupW * 0.50) + groupX, line * (groupH * editBoxH) + groupY, (groupW * (0.50 + 0.25/2)) + groupX, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	//gMgr->setGUIText(greenColorID, L"255");
	//textID = gMgr->createStaticText((groupW * (0.50 + 0.25/2)) + groupX, line * (groupH * editBoxH) + groupY, (groupW * (0.50 + 0.25)) + groupX, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	//gMgr->setGUIText(textID, L"Green");
	//// the b edit static box
	//blueColorID = gMgr->createEditBox((groupW * 0.75) + groupX, line * (groupH * editBoxH) + groupY, (groupW * (0.75 + 0.25/2)) + groupX, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	//gMgr->setGUIText(blueColorID, L"255");
	//textID = gMgr->createStaticText((groupW * (0.75 + 0.25/2)) + groupX, line * (groupH * editBoxH) + groupY, (groupW * (0.75 + 0.25)) + groupX, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	//gMgr->setGUIText(textID, L"Blue");
	//////////////////
	//////////////////
	//////////////////
	//////////////////
	//////////////////

	// we put our character select spin box
	getCharacterList(); // this gets the characeter's from the database
	line++;
	characterSelectStringID = gMgr->createEditBox(groupX, line * (groupH * editBoxH) + groupY, groupW * 0.5 + groupX, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	gMgr->setGUIText(characterSelectStringID, (char*)selectionList.begin()->second.c_str());
	characterSelectSpinBoxID = gMgr->createSpinBox(groupW * 0.5 + groupX, line * (groupH * editBoxH) + groupY, groupW * 0.75 + groupX, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	gMgr->setSpinBoxRange(characterSelectSpinBoxID, 1, selectionList.size(), 1);
	gMgr->setSpinBoxDecimalPlaces(characterSelectSpinBoxID, 0);
	textID = gMgr->createStaticText(groupW * 0.75 + groupX, line * (groupH * editBoxH) + groupY, groupW + groupX, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	gMgr->setGUIText(textID, L"Character select");

	// the start button
	line++;
	startButtonID = gMgr->createButton(groupX, line * (groupH * editBoxH) + groupY, groupW + groupX, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	gMgr->setGUIText(startButtonID, L"START");

	// the options button
	line++;
	optionsButtonID = gMgr->createButton(groupX, line * (groupH * editBoxH) + groupY, groupW + groupX, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	gMgr->setGUIText(optionsButtonID, L"OPITIONS");

	// the quit button
	line++;
	quitButtonID = gMgr->createButton(groupX, line * (groupH * editBoxH) + groupY, groupW + groupX, (line + 1) * (groupH * editBoxH) + groupY - (groupH * padding));
	gMgr->setGUIText(quitButtonID, L"QUIT");
}

void MainMenuState::DeInit(void)
{
	gMgr->clearGUI();
}

void MainMenuState::Pause(CStateEngine *engine)
{
}

void MainMenuState::Resume(CStateEngine *engine)
{
}

void MainMenuState::HandleEvents(CStateEngine *engine)
{
	// ESC to quit, check first that we weren't holding ESC from another state change
	if(iMgr->getKeyState(KEY_ESCAPE) && !holdingESC)
		engine->Quit();
	else if(!iMgr->getKeyState(KEY_ESCAPE))
		holdingESC = false;

	// gui event test. See if we have clicked the quit button

	if(checkButtonsWereClicked())
		engine->Quit();
	checkEnterWasHitInAnEditBox();
	//checkColorSelectChange();
	checkSpinBoxChange();

	// when done, clear the gui event list so we can check for more next frame
	iMgr->clearGUIEventList();

	// check for network messages. Fix later
	if(handleNetworkMessages())
		engine->RequestChangeState(new MainClientState()); // go to the main client state
}

void MainMenuState::Update(CStateEngine *engine)
{
	// you have to tick this first in Irrlicht to get the new time and to start the drawing for this frame
	gMgr->tick();

	// we get the delta time so we can move objects in the correct speed
	float deltaTime = gMgr->getNewDeltaTime();
}

void MainMenuState::Draw(CStateEngine *engine)
{
	gMgr->drawAll();
}

bool MainMenuState::checkButtonsWereClicked(void)
{
	bool exit = false;

	// check how many buttons were clicked. Then check if the quit button was one of them
	unsigned int numberOfButtonsClicked = iMgr->getNumberOfElementsForThisEvent(irr::gui::EGET_BUTTON_CLICKED);
	if(numberOfButtonsClicked > 0) // then there were button that were clicked
	{
		for(int i = 0; i < numberOfButtonsClicked; i++)
		{
			if(iMgr->getElementFromEventByIndex(irr::gui::EGET_BUTTON_CLICKED, i) == quitButtonID)
				//engine->Quit();
				exit = true;
			else if(iMgr->getElementFromEventByIndex(irr::gui::EGET_BUTTON_CLICKED, i) == startButtonID)
			{
				//checkIfColorChagne(); // fix the color just in case
				addUserNameAndStartConnection();
			}
			else if(iMgr->getElementFromEventByIndex(irr::gui::EGET_BUTTON_CLICKED, i) == optionsButtonID)
			{
				messageBoxID = gMgr->createMessageBox(L"test caption", L"test body",EMBF_OK);
			}
		}
	}

	return exit;
}

void MainMenuState::checkEnterWasHitInAnEditBox(void)
{
	// check if the hit enter in any chat edit box
	unsigned int numberOfEntersHit = iMgr->getNumberOfElementsForThisEvent(irr::gui::EGET_EDITBOX_ENTER);
	if(numberOfEntersHit > 0)
	{
		addUserNameAndStartConnection();
	}
}

//void MainMenuState::checkColorSelectChange(void)
//{
//	// we check if any of the color select edit box were changed, we do this if the focus was taking off of it
//	// we make sure that all are still in the range of 0 to 255
//	// we update the color box
//	// we update the GameManager color cube also
//	unsigned int numberOfTextChanges = iMgr->getNumberOfElementsForThisEvent(irr::gui::EGET_ELEMENT_FOCUS_LOST);
//	if(numberOfTextChanges > 0)
//	{
//		for(int i = 0; i < numberOfTextChanges; i++)
//		{
//			unsigned int colorEditBoxID = iMgr->getElementFromEventByIndex(irr::gui::EGET_ELEMENT_FOCUS_LOST, i);
//
//			if(colorEditBoxID == redColorID || colorEditBoxID == greenColorID || colorEditBoxID == blueColorID)
//			{
//				checkIfColorChagne();
//			}
//		}
//	}
//}

//void MainMenuState::checkIfColorChagne(void)
//{
//	cout << "color check" << endl;
//
//	int r, g, b;
//	game->getCubeColor((unsigned int&)r, (unsigned int&)g, (unsigned int&)b);
//
//	r = atoi(core::stringc(gMgr->getGUIText(redColorID)).c_str());
//
//	// we check if the color is in range, then fix the edit box
//	if(r < 0 || r > 255)
//	{
//		if(r < 0)
//			r = 0;
//		else
//			r = 255;
//
//		core::stringc fixColor = L"";
//		fixColor += r;
//		gMgr->setGUIText(redColorID, core::stringw(fixColor).c_str());
//	}
//
//	g = atoi(core::stringc(gMgr->getGUIText(greenColorID)).c_str());
//
//	// we check if the color is in range, then fix the edit box
//	if(g < 0 || g > 255)
//	{
//		if(g < 0)
//			g = 0;
//		else
//			g = 255;
//
//		core::stringc fixColor = L"";
//		fixColor += g;
//		gMgr->setGUIText(greenColorID, core::stringw(fixColor).c_str());
//	}
//
//	b = atoi(core::stringc(gMgr->getGUIText(blueColorID)).c_str());
//
//	// we check if the color is in range, then fix the edit box
//	if(b < 0 || b > 255)
//	{
//		if(b < 0)
//			b = 0;
//		else
//			b = 255;
//
//		core::stringc fixColor = L"";
//		fixColor += b;
//		gMgr->setGUIText(blueColorID, core::stringw(fixColor).c_str());
//	}
//
//	game->setCubeColor(r, g, b);
//	gMgr->setStaticTextBackgroundColor(colorID, r, g, b, 255);
//}

void MainMenuState::checkSpinBoxChange(void)
{
	unsigned int numberOfSpinBoxesChanged = iMgr->getNumberOfElementsForThisEvent(irr::gui::EGET_SPINBOX_CHANGED);
	if(numberOfSpinBoxesChanged > 0)
	{
		for(int i = 0; i < numberOfSpinBoxesChanged; i++)
		{
			if(iMgr->getElementFromEventByIndex(irr::gui::EGET_SPINBOX_CHANGED, i) == characterSelectSpinBoxID)
			{
				unsigned int value = gMgr->getSpinBoxValue(characterSelectSpinBoxID);
				std::string valueString = selectionList[value];
				gMgr->setGUIText(characterSelectStringID, (char*)valueString.c_str());
			}
		}
	}
}

void MainMenuState::addUserNameAndStartConnection(void)
{
	system("cls");

	// we check first if we have an ip filled first
	//core::stringc ip(gMgr->getGUIText(ipID));
	//if(ip == "") //no ip? then no connection
	//	return;

	core::stringc ip(netMgr->getStoredIP());
	unsigned int port = netMgr->getStoredPort();
	cout << "ip: " << ip.c_str() << endl;

	// check first if we have a name in the user name edit box, if we don't have any, we fill in Guest
	core::stringc userName(gMgr->getGUIText(userNameID));
	if(userName == "")
		userName = L"Guest";

	game->setUserName((char*)userName.c_str());
	game->setBodyViewID(gMgr->getSpinBoxValue(characterSelectSpinBoxID));

	//netMgr->StartClient("127.0.0.1", 25000);
	netMgr->StartClient((char*)ip.c_str(), port);
	cout << "Starting the client" << endl;
}

bool MainMenuState::handleNetworkMessages(void)
{

	bool connected = false;

	while(netMgr->getNewPacket2())
	{
		unsigned char idn;
		unsigned int netID;
		//unsigned int r;
		//unsigned int g;
		//unsigned int b;

		netMgr->fillBitStreamFromPacket();

		idn = netMgr->readUCharFromBitStream();

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
		case ID_DISCONNECTION_NOTIFICATION:
			cout << "A client has disconnected." << endl;
			break;
		case ID_CONNECTION_LOST:
			cout << "A client lost the connection." << endl;
			break;

		case ID_REQUEST_REVISION:

			///////////
			// 2nd step
			///////////

			cout << "Step 2" << endl;

			// The server is asking for the clinet's version number
			// we sent it with the current netID so we would know it us

			netID = netMgr->readUIntFromBitStream();

			netMgr->clearBitSteram();
			netMgr->writeUCharToBitStream((unsigned char)ID_INFO_REVISION);
			netMgr->writeUIntToBitStream(netMgr->getRevisionNumber());
			netMgr->writeUIntToBitStream(netID);

			// we set the last argument to false to send to our client, if its true then it will be send to everyone but our client
			netMgr->sendBitStreamToAll(HIGH_PRIORITY, RELIABLE_ORDERED, 0);

			cout << "Go to step 3" << endl;

			break;

		case ID_REVISION_BAD:

			///////////
			// 4th step
			///////////

			cout << "Step 4" << endl;

			// revision didn't match
			// we disconnect and ask the user to update the clinet

			netMgr->shutDown(); // we close the connection
			messageBoxID = gMgr->createMessageBox(L"Error", L"Client revision did not match. Please update to the correct version",EMBF_OK);

			break;

		case ID_REQUEST_USER_DATA:

			///////////
			// 5th step
			///////////

			cout << "Step 5" << endl;

			// the revision match
			// the server is asking for the user's data, and character data
			// for now, we just send the colors
			// now we also send the model ID. which is 1 for ninja

			//game->setBodyViewID(3);

			netID = netMgr->readUIntFromBitStream();

			//game->getCubeColor(r, g, b);

			netMgr->clearBitSteram();

			netMgr->writeUCharToBitStream((unsigned char)ID_INFO_USER_DATA);
			netMgr->writeUIntToBitStream(netID);
			netMgr->writeStringToBitStream(game->getUserName());
			netMgr->writeUIntToBitStream(game->getBodyViewID());
			//netMgr->writeUIntToBitStream(r);
			//netMgr->writeUIntToBitStream(g);
			//netMgr->writeUIntToBitStream(b);

			netMgr->sendBitStreamToAll(HIGH_PRIORITY, RELIABLE_ORDERED, 0);
			cout << "Go to step 6" << endl;

			break;

		case ID_CHARACTER_READY:

			///////////
			// 7th step
			///////////

			cout << "Step 7" << endl;

			// we got confimation that our character has been created on the server
			// we tell the server that we acknowldeg, and move to the MainClientState

			netID = netMgr->readUIntFromBitStream();

			netMgr->clearBitSteram();

			netMgr->writeUCharToBitStream((unsigned char)ID_CONFIRM_CHARACTER);
			netMgr->writeUIntToBitStream(netID);

			netMgr->sendBitStreamToAll(HIGH_PRIORITY, RELIABLE_ORDERED, 0);
			cout << "Go to step 8" << endl;

			// we move to the MainClinetState
			connected = true;

			break;

		case ID_CONNECTION_ATTEMPT_FAILED: // if trying to connect but no servr resopons

			//netMgr->shutDown();
			messageBoxID = gMgr->createMessageBox(L"Error", L"The server is not responinding. It could be down. Please check the forum for further information.",EMBF_OK);

			break;

		default:
			cout << "Message with identifier " << (int)idn << " has arrived." << endl;
			break;
		}

		netMgr->DestroyPacket2();
	}

	return connected;
}

void MainMenuState::getCharacterList(void)
{
	std::vector<std::string> rowIDList;

	dbMgr->getAllRowNumbersFromTable("body_list", rowIDList);

	int listSize = rowIDList.size();

	for(int i = 0; i < listSize; i++)
	{
		selectionList[dbMgr->getRecordAsUInt("body_list", "id", (char*)rowIDList[i].c_str())] = dbMgr->getRecordAsString("body_list", "name", (char*)rowIDList[i].c_str());
	}
}
