#ifndef TS_MAIN_MENU_HPP_
#define TS_MAIN_MENU_HPP_

#include "GameState/IGameState.hpp"

class MainMenuState: public IGameState{
public:
	enum{
		MENU_PLAY = 0,
		MENU_OPTIONS,
		MENU_CREDITS,
		/*MENU_ABOUT,*/
		MENU_EXIT,
		MENU_MAX
	};

	static std::string gMenuNames[MENU_MAX];

public:
	static IGameState::ptr Create(){
		return IGameState::ptr(new MainMenuState());
	}

	MainMenuState();
	~MainMenuState();

	void Suspend(){}
	void Resume(){}

	void HandleInput(const sf::Event& evt);
	void HandleAsyncInput(const sf::Input& input){}

	void Update();
	void Render(sf::RenderWindow& wnd);

private:
	sf::Color mNormalTextColor;
	sf::Color mSelectedTextColor;

	sf::String mTitle;
	sf::String mAuthor[2];
	sf::String mMenu[MENU_MAX];
	sf::String mCopy;

	char mSelectedOption;
};

#endif