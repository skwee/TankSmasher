#include "Resource/ImageManager.hpp"

ImageManager::ImagePtr ImageManager::Get(const std::string& locator){
	auto it = mImages.find(locator);
	if(it == mImages.end()){
		ImagePtr r = Load(locator);
		if(r)
			it = mImages.insert(std::make_pair(locator, r)).first;
		else
			throw TankSmasherException("Cannot load resource " + locator);

		Logger::GetInstance()->Log("Resource " + locator + " loaded");
	}

	ImagePtr resource = it->second;
	return resource;
}

ImageManager::ImagePtr ImageManager::Load(const std::string& locator){
	std::string file;
	file = "content/sprites/" + locator;
	ImagePtr image = ImagePtr(new sf::Image());
	if(!image->LoadFromFile(file))
		return ImagePtr();
	return image;
}