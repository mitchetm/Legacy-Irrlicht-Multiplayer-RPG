#pragma once

template <class T>
class Singleton
{
public:
	static T *getInstance()
	{
		if(!sInstance)
			sInstance = new T();

		return sInstance;
	}

	void killInstance()
	{
		if(sInstance)
			delete sInstance;
	}

protected:
	Singleton()
	{
	}
	virtual ~Singleton()
	{
	}

private:
	static T* sInstance;

};
