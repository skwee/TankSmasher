#ifndef TS_FONT_MANAGER_HPP_
#define TS_FONT_MANAGER_HPP_

#include <SFML/Graphics.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

#include "Logger.hpp"

class FontManager{
public:
	static const int DEFAULT_FONT_SIZE;

	typedef boost::shared_ptr<sf::Font> FontPtr;

public:
	FontManager(){
		Logger::GetInstance()->Log("Font Manager Created");
	}

	~FontManager(){
		mFonts.clear();
		Logger::GetInstance()->Log("Font Manager Destroyed");
	}

	FontPtr Get(const std::string& locator);

private:
	FontPtr Load(const std::string& locator);

	std::map<const std::string, FontPtr> mFonts;
};

#endif