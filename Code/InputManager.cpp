#include "InputManager.h"
#include <iostream>

InputManager *Singleton<InputManager>::sInstance = 0;

InputManager::InputManager()
{
	// set all key to false (press up)
	for(int i = 0; i < irr::KEY_KEY_CODES_COUNT; i++)
		keyList[i] = false;
}

InputManager::~InputManager()
{
}

bool InputManager::OnEvent(const irr::SEvent &evn)
{
	if(evn.EventType == irr::EET_KEY_INPUT_EVENT) // keyboard event
	{
		keyList[evn.KeyInput.Key] = evn.KeyInput.PressedDown;
//		return true;
	}
	else if(evn.EventType == irr::EET_MOUSE_INPUT_EVENT) // mouse event
	{
		keyList[irr::KEY_LBUTTON] = evn.MouseInput.isLeftPressed();
		keyList[irr::KEY_RBUTTON] = evn.MouseInput.isRightPressed();
		keyList[irr::KEY_MBUTTON] = evn.MouseInput.isMiddlePressed();
//		return true;
	}
	else if(evn.EventType == irr::EET_GUI_EVENT) // gui event
	{
		unsigned int eventType = evn.GUIEvent.EventType; // what event got fired
		unsigned int elementID = evn.GUIEvent.Caller->getID(); // what element fired the event

		// now we store the event and the element so we can it later

		// check first if we have the event in the list. If not, add it first

		if(guiEventList.find(eventType) == guiEventList.end()) // the event is not in the list, add it
		{
			guiEventList[eventType] = std::vector<unsigned int>();
		}

		// now we add the element to the event list

		guiEventList[eventType].push_back(elementID);
//		return true;
	}

	return false;
}

bool InputManager::getKeyState(unsigned int key)
{
	return keyList[key];
}

unsigned int InputManager::getNumberOfElementsForThisEvent(unsigned int eventType)
{
	// first check if the even type is in the list. If not, then there are no elements store
	if(guiEventList.find(eventType) == guiEventList.end()) // the event is not in the list, add it
	{
		return 0; // no elements
	}

	// get number of elements for this event
	return guiEventList[eventType].size();
}

unsigned int InputManager::getElementFromEventByIndex(unsigned int eventType, unsigned int index)
{
	// TODO: check first if the event exists. If not, throw and exception
	// here I'm assuming you already checked

	// the first is the event type, the second is the index of the element that fired this event
	return guiEventList[eventType][index];
}

void InputManager::clearGUIEventList(void)
{
	guiEventList.clear();
}
