#include "Resource/FontManager.hpp"

const int FontManager::DEFAULT_FONT_SIZE = 30;

FontManager::FontPtr FontManager::Get(const std::string& locator){
	auto it = mFonts.find(locator);
	if(it == mFonts.end()){
		FontPtr r = Load(locator);
		if(r)
			it = mFonts.insert(std::make_pair(locator, r)).first;
		else
			throw TankSmasherException("Cannot load resource " + locator);

		Logger::GetInstance()->Log("Resource " + locator + " loaded");
	}

	FontPtr resource = it->second;
	return resource;
}

FontManager::FontPtr FontManager::Load(const std::string& locator){
	std::string file;
	int size;

	std::string::size_type plast = locator.find_last_not_of(";");
	std::string::size_type pfirst = locator.find_first_of(";");
	std::string tmp;

	if((plast == std::string::npos) || (pfirst == std::string::npos)){
		file = locator;
		size = DEFAULT_FONT_SIZE;
	}else{
		file = locator.substr(0, plast - 2);
		tmp = locator.substr(pfirst + 1);
		std::istringstream iss(tmp);
		iss >> size;
	}

	file = "content/fonts/" + file;

	FontPtr font = FontPtr(new sf::Font());
	if( !font->LoadFromFile(file, size))
		return FontPtr();

	return font;
}