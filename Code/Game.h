#pragma once

#include "Singleton.h"

#include <string>

class Game : public Singleton<Game>
{

public:

	Game();
	~Game();

	void setScreenSize(unsigned width, unsigned int height);
	unsigned int getScreenWidth(void);
	unsigned int getScreenHeight(void);

	void setUserName(char *name);
	char *getUserName(void);

	void setBodyViewID(unsigned int id);
	unsigned int getBodyViewID(void);

	void setCubeColor(unsigned int cubeR, unsigned int cubeG, unsigned int cubeB);
	void getCubeColor(unsigned int &cubeR, unsigned int &cubeG, unsigned int &cubeB);

private:

	unsigned int screenWidth, screenHeight;

	// login info
	std::string userName;
	unsigned int bodyViewID;
	unsigned int r, g, b; // color of cube

};
