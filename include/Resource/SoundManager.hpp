#ifndef TS_SOUND_MANAGER_HPP_
#define TS_SOUND_MANAGER_HPP_

#include <SFML/Audio.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

#include "Logger.hpp"

class SoundManager{
public:
	typedef boost::shared_ptr<sf::SoundBuffer> SoundPtr;

public:
	SoundManager(){
		Logger::GetInstance()->Log("Sound Manager Created");
	}

	~SoundManager(){
		mSounds.clear();
		Logger::GetInstance()->Log("Sound Manager Destroyed");
	}

	SoundPtr Get(const std::string& locator);

private:
	SoundPtr Load(const std::string& locator);

	std::map<const std::string, SoundPtr> mSounds;
};

#endif