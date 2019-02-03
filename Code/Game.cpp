#include "Game.h"

Game *Singleton<Game>::sInstance = 0;

Game::Game()
{
	screenWidth = 0;
	screenHeight = 0;

	r = 255;
	g = 255;
	b = 255;
}

Game::~Game()
{
}

void Game::setScreenSize(unsigned width, unsigned int height)
{
	screenWidth = width;
	screenHeight = height;
}

unsigned int Game::getScreenWidth(void)
{
	return screenWidth;
}

unsigned int Game::getScreenHeight(void)
{
	return screenHeight;
}

void Game::setUserName(char *name)
{
	userName = name;
}

char *Game::getUserName(void)
{
	return (char*)userName.c_str();
}

void Game::setBodyViewID(unsigned int id)
{
	bodyViewID = id;
}

unsigned int Game::getBodyViewID(void)
{
	return bodyViewID;
}

void Game::setCubeColor(unsigned int cubeR, unsigned int cubeG, unsigned int cubeB)
{
	r = cubeR;
	g = cubeG;
	b = cubeB;
}

void Game::getCubeColor(unsigned int &cubeR, unsigned int &cubeG, unsigned int &cubeB)
{
	cubeR = r;
	cubeG = g;
	cubeB = b;
}
