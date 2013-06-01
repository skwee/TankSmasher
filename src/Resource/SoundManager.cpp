#include "Resource/SoundManager.hpp"

SoundManager::SoundPtr SoundManager::Get(const std::string& locator){
	auto it = mSounds.find(locator);
	if(it == mSounds.end()){
		SoundPtr r = Load(locator);
		if(r)
			it = mSounds.insert(std::make_pair(locator, r)).first;
		else
			throw TankSmasherException("Cannot load resource " + locator);

		Logger::GetInstance()->Log("Resource " + locator + " loaded");
	}

	SoundPtr resource = it->second;
	return resource;
}

SoundManager::SoundPtr SoundManager::Load(const std::string& locator){
	std::string file;
	file = "content/sounds/" + locator;
	SoundPtr sound = SoundPtr(new sf::SoundBuffer());
	if(!sound->LoadFromFile(file))
		return SoundPtr();
	return sound;
}