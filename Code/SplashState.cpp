#include "SplashState.h"
#include "MainMenuState.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

SplashState::SplashState()
{
}

SplashState::~SplashState()
{
}

void SplashState::Init(void)
{
	game = Game::getInstance();
	gMgr = GraphicManager::getInstance();
	iMgr = InputManager::getInstance();
	dbMgr = DataBaseManager::getInstance();
	sMgr = SoundManager::getInstance();

	unsigned int screenWidth = game->getScreenWidth();
	unsigned int screenHeight = game->getScreenHeight();

	// we get what we need from the database
	std::string splash1_path, splash2_path;
	int x1, y1, x2, y2;
	std::string sound1_path, sound2_path;

	std::string rowID;
	std::string path, fullPath; // this is used to get the full path of each file
	std::vector<std::string> resultList;
	int listSize;

	// first we get the path that our splash images are in
	resultList.clear();
	if(dbMgr->searchColumnForString("splash_list", "name", "path", resultList))
	{
		// its always going to be the first result = 0
		path = dbMgr->getRecordAsString("splash_list", "path", (char*)resultList[0].c_str());
		path += "/"; // we add this to the end of the path
	}
	else // we couldn't find the path
	{
		// throw something
	}

	// now we get our splash iamges
	resultList.clear();
	if(dbMgr->searchColumnForString("splash_list", "name", "splash1", resultList))
	{
		// its always going to be the first result = 0
		splash1_path = path + dbMgr->getRecordAsString("splash_list", "path", (char*)resultList[0].c_str());
		x1 = dbMgr->getRecordAsInt("splash_list", "x", (char*)resultList[0].c_str());
		y1 = dbMgr->getRecordAsInt("splash_list", "y", (char*)resultList[0].c_str());
	}
	else // we couldn't find the path
	{
		// throw something
	}
	resultList.clear();
	if(dbMgr->searchColumnForString("splash_list", "name", "splash2", resultList))
	{
		// its always going to be the first result = 0
		splash2_path = path + dbMgr->getRecordAsString("splash_list", "path", (char*)resultList[0].c_str());
		x2 = dbMgr->getRecordAsInt("splash_list", "x", (char*)resultList[0].c_str());
		y2 = dbMgr->getRecordAsInt("splash_list", "y", (char*)resultList[0].c_str());
	}
	else // we couldn't find the path
	{
		// throw something
	}

	// now for the sound files
	// first we get the path that our sound files are in
	resultList.clear();
	if(dbMgr->searchColumnForString("sound_list", "name", "path", resultList))
	{
		// its always going to be the first result = 0
		path = dbMgr->getRecordAsString("sound_list", "path", (char*)resultList[0].c_str());
		path += "/"; // we add this to the end of the path
	}
	else // we couldn't find the path
	{
		// throw something
	}

	// now we get the sound files
	resultList.clear();
	if(dbMgr->searchColumnForString("sound_list", "name", "main_menu_theme", resultList))
	{
		// its always going to be the first result = 0
		sound1_path = path + dbMgr->getRecordAsString("sound_list", "path", (char*)resultList[0].c_str());
	}
	else // we couldn't find the path
	{
		// throw something
	}
	resultList.clear();
	if(dbMgr->searchColumnForString("sound_list", "name", "battel_theme", resultList))
	{
		// its always going to be the first result = 0
		sound2_path = path + dbMgr->getRecordAsString("sound_list", "path", (char*)resultList[0].c_str());
	}
	else // we couldn't find the path
	{
		// throw something
	}

	//imageID = gMgr->createImage((char*)dbMgr->getDataAsString("splash_1").c_str(), dbMgr->getDataAsInt("splash_1_x"), dbMgr->getDataAsInt("splash_1_y")); // this is the flash screen
	imageID = gMgr->createImage((char*)splash1_path.c_str(), x1, y1); // this is the flash screen
	gMgr->setImageColor(imageID, 0, 0, 0, 0);
	//imageID2 = gMgr->createImage((char*)dbMgr->getDataAsString("splash_2").c_str(), dbMgr->getDataAsInt("splash_2_x"), dbMgr->getDataAsInt("splash_2_y"));
	imageID2 = gMgr->createImage((char*)splash2_path.c_str(), x2, y2);
	gMgr->setImageColor(imageID2, 0, 0, 0, 0);
	//fadeID = gMgr->createInOutFader(0, 0, screenWidth, screenHeight); // this our fade in-ou effect

	wait1 = 1;
	fadeInTimer = 2;
	wait2 = 3;
	fadeOutTimer = 2;
	wait3 = 1;

	timer = wait1;
	state = 0;

	goToMainMenu = false;

	// test sound manager
	//unsigned int soundTestID = sMgr->loadSound("leo_warrior_theme.mp3");
	std::string value;
	std::stringstream oss1;
	std::stringstream oss2;
	//unsigned int soundTest1ID = sMgr->loadSound((char*)dbMgr->getDataAsString("main_theme").c_str());
	//unsigned int soundTest2ID = sMgr->loadSound((char*)dbMgr->getDataAsString("battel_theme").c_str());
	unsigned int soundTest1ID = sMgr->loadSound((char*)sound1_path.c_str());
	unsigned int soundTest2ID = sMgr->loadSound((char*)sound2_path.c_str());
	oss1 << soundTest1ID;
	value = oss1.str();
	dbMgr->addData("main_theme_id", value);
	oss2 << soundTest2ID;
	value = oss2.str();
	dbMgr->addData("battel_theme_id", value);

	unsigned int mainThemeChannelID = 0;
	sMgr->playSound(mainThemeChannelID, soundTest1ID);
	sMgr->setVolum(mainThemeChannelID, 1);
	sMgr->setLoopCount(mainThemeChannelID, -1);
}

void SplashState::DeInit(void)
{
	gMgr->clearGUI();
}

void SplashState::Pause(CStateEngine *engine)
{
}

void SplashState::Resume(CStateEngine *engine)
{
}

void SplashState::HandleEvents(CStateEngine *engine)
{

	// ESC in case we want to skip the splash screen
	if(iMgr->getKeyState(KEY_ESCAPE))
		goToMainMenu = true;

	if(goToMainMenu)
		//engine->Quit();
		engine->RequestChangeState(new MainMenuState()); // when done we go to the main menu


}

void SplashState::Update(CStateEngine *engine)
{
	float fade = 0;

	// you have to tick this first in Irrlicht to get the new time and to start the drawing for this frame
	gMgr->tick();

	timer -= gMgr->getNewDeltaTime();
	//cout << "timer = " << timer << endl;

	switch(state)
	{
	case 0: // here we wait a bit before we show the splash screen

		if(timer < 0)
		{
			timer = fadeInTimer;
			state = 1;
		}

		break;

	case 1: // here we start to fade in

		fade = (fadeInTimer - timer) / fadeInTimer;
		fade *= 255;
		gMgr->setImageColor(imageID, fade, fade, fade, fade);

		if(timer < 0)
		{
			timer = wait2;
			state = 2;
		}

		break;

	case 2: // here we wait a bit so we can see the splash screen

		if(timer < 0)
		{
			timer = fadeOutTimer;
			state = 3;
		}

		break;

	case 3: // here we start to fade out

		fade = timer / fadeOutTimer;
		fade *= 255;
		gMgr->setImageColor(imageID, fade, fade, fade, fade);

		if(timer < 0)
		{
			timer = wait3;
			state = 4;
		}

		break;

	case 4: // here we wait a bit before we change states

		if(timer < 0)
		{
			state = 5;
		}

		break;
		
			case 5: // here we wait a bit before we show the splash screen

		if(timer < 0)
		{
			timer = fadeInTimer;
			state = 6;
		}

		break;

	case 6: // here we start to fade in

		fade = (fadeInTimer - timer) / fadeInTimer;
		fade *= 255;
		gMgr->setImageColor(imageID2, fade, fade, fade, fade);

		if(timer < 0)
		{
			timer = wait2;
			state = 7;
		}

		break;

	case 7: // here we wait a bit so we can see the splash screen

		if(timer < 0)
		{
			timer = fadeOutTimer;
			state = 8;
		}

		break;

	case 8: // here we start to fade out

		fade = timer / fadeOutTimer;
		fade *= 255;
		gMgr->setImageColor(imageID2, fade, fade, fade, fade);

		if(timer < 0)
		{
			timer = wait3;
			state = 9;
		}

		break;

	case 9: // here we wait a bit before we change states

		if(timer < 0)
		{
			goToMainMenu = true;
		}

		break;

	default:
		break;
	}

}

void SplashState::Draw(CStateEngine *engine)
{
	gMgr->drawAll();
}
