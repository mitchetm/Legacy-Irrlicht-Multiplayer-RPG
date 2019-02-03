#include "SoundManager.h"
#include <iostream>

SoundManager *Singleton<SoundManager>::sInstance = 0;

using namespace std;

void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
		string err = "FMOD error! (";
		err += result;
		err += ") ";
		err += FMOD_ErrorString(result);

		throw((char*)err.c_str());
    }
}

SoundManager::SoundManager()
{
	ID = 0;
}

SoundManager::~SoundManager()
{
	FMOD_RESULT result;

	clearSoundList();

	result = system->close();
	ERRCHECK(result);
	result = system->release();
	ERRCHECK(result);
}

void SoundManager::run(void)
{
	FMOD_RESULT result;
	unsigned int version;

	/*
	Create a System object and initialize.
	*/

	result = FMOD::System_Create(&system);
	ERRCHECK(result);

	result = system->getVersion(&version);
	ERRCHECK(result);

	if (version < FMOD_VERSION)
	{
		string err = "Error!  You are using an old version of FMOD";
		err += version;
		err += ".  This program requires ";
		err += FMOD_VERSION;

		throw((char*)err.c_str());
	}

	result = system->init(32, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result);
}

void SoundManager::tick(void)
{
	system->update();
}

unsigned int SoundManager::loadSound(char* fileName)
{
	FMOD::Sound *sound;
	FMOD_RESULT result;

	result = system->createSound(fileName, FMOD_HARDWARE, 0, &sound);
	ERRCHECK(result);

	ID++;

	soundList[ID] = sound;
	sound->setMode(FMOD_LOOP_NORMAL);

	return ID;
}

unsigned int SoundManager::loadStream(char* fileName)
{
	FMOD::Sound *sound;
	FMOD_RESULT result;

	result = system->createStream(fileName, FMOD_HARDWARE, 0, &sound);
	ERRCHECK(result);

	ID++;

	soundList[ID] = sound;
	sound->setMode(FMOD_LOOP_NORMAL);

	return ID;
}

void SoundManager::playSound(unsigned int channelNumber, unsigned int soundID)
{
	FMOD_RESULT result;
	FMOD::Channel *channel;

	system->getChannel(channelNumber, &channel);
	cout << "channel " << channelNumber << " - Sound " << soundID << endl; 
	channel->stop();

	if(soundList.find(soundID) != soundList.end())
	{
		result = system->playSound(FMOD_CHANNEL_REUSE ,soundList[soundID], false, &channel);
		ERRCHECK(result);
	}
	else
	{
		// throw something
	}
}

void SoundManager::deleteSound(unsigned int id)
{
	FMOD_RESULT result;

	if(soundList.find(id) != soundList.end())
	{
		result = soundList[id]->release();
		ERRCHECK(result);

		soundList.erase(id);
	}
	else
	{
		// throw something
	}
}

void SoundManager::setVolum(unsigned int channelNumber, float volume)
{
	FMOD::Channel *channel;

	system->getChannel(channelNumber, &channel);

	channel->setVolume(volume);
}

void SoundManager::setLoopCount(unsigned int channelNumber, int count)
{
	FMOD::Channel *channel;

	system->getChannel(channelNumber, &channel);

	channel->setLoopCount(count);
}

void SoundManager::clearSoundList(void)
{
	std::map<unsigned int, FMOD::Sound*>::iterator itr;

	for(itr = soundList.begin(); itr != soundList.end(); itr++)
		itr->second->release();

	soundList.clear();
}
