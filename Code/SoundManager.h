#pragma once

#include "Singleton.h"

#include "fmod.hpp"
#include "fmod_errors.h"

#include <string>
#include <map>

using namespace std;

class SoundManager : public Singleton<SoundManager>
{

public:

	SoundManager();
	~SoundManager();

	void run(void);
	void tick(void);

	unsigned int loadSound(char* fileName);
	unsigned int loadStream(char* fileName); // same is sound, just

	void playSound(unsigned int channelNumber, unsigned int soundID);
	void deleteSound(unsigned int id);

	void setVolum(unsigned int channelNumber, float volume);
	void setLoopCount(unsigned int channelNumber, int count); // set to -1 to loop forever

	void clearSoundList(void);

private:

	unsigned int ID;

	FMOD::System *system;
	std::map<unsigned int, FMOD::Sound*> soundList;

};
