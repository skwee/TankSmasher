#ifndef TS_OPTIONS_STATE_HPP_
#define TS_OPTIONS_STATE_HPP_

#include "GameState/IGameState.hpp"

class OptionsState: public IGameState{
public:
	enum{
		OPTION_MUSIC = 0, OPTION_SOUND,
		OPTION_MAX
	};

public:
	static IGameState::ptr Create(){
		return IGameState::ptr(new OptionsState());
	}

	OptionsState();
	~OptionsState();

	void Suspend(){}
	void Resume(){}

	void HandleInput(const sf::Event& evt);
	void HandleAsyncInput(const sf::Input& input){}

	void Update();
	void Render(sf::RenderWindow& wnd);

private:
	sf::Color mNormalTextColor;
	sf::Color mSelectedTextColor;

	short mSelectedOption;
	sf::String mOptions[OPTION_MAX];
	sf::String mOptionValues[OPTION_MAX];
	sf::String mTitle;
};

#endif