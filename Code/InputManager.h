#pragma once

#include "Singleton.h"
#include <irrlicht.h>

#include <map>
#include <vector>

class InputManager : public irr::IEventReceiver, public Singleton<InputManager>
{

public:

	InputManager();
	~InputManager();

	virtual bool OnEvent(const irr::SEvent &evn);

	bool getKeyState(unsigned int key);

	unsigned int getNumberOfElementsForThisEvent(unsigned int eventType); // how many elements fired this event. 0 = no element
	unsigned int getElementFromEventByIndex(unsigned int eventType, unsigned int index); // get element by index
	void clearGUIEventList(void);

private:

	bool keyList[irr::KEY_KEY_CODES_COUNT];

	// GUI event handel
	// this is where we store all the GUI events (like what button got cliked, etc...)
	// the first part in the map is what GUI event
	// the second part in the map is a list of gui elements that got the event
	// this list get cleared/cleaned every frame so we can get new gui events next frame
	std::map<unsigned int, std::vector<unsigned int>> guiEventList;


};
