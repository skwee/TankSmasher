#ifndef TS_IMAGE_MANAGER_HPP_
#define TS_IMAGE_MANAGER_HPP_

#include <SFML/Graphics.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

#include "Logger.hpp"

class ImageManager{
public:
	typedef boost::shared_ptr<sf::Image> ImagePtr;

public:
	ImageManager(){
		Logger::GetInstance()->Log("Image Manager Created");
	}

	~ImageManager(){
		mImages.clear();
		Logger::GetInstance()->Log("Image Manager Destroyed");
	}

	ImagePtr Get(const std::string& locator);

private:
	ImagePtr Load(const std::string& locator);

	std::map<const std::string, ImagePtr> mImages;
};

#endif