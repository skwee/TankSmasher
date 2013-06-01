#ifndef TS_CREDITS_STATE_HPP_
#define TS_CREDITS_STATE_HPP_

#include "GameState/IGameState.hpp"

class CreditsState: public IGameState{
public:
	enum{
		TEXT_PROGRAMMING = 0,
		TEXT_MUSIC, TEXT_SOUND, TEXT_MISC,
		TEXT_MAX
	};

	static const float UPDATE_TIME;
	static const float WAIT_TIME;

public:
	static IGameState::ptr Create(){
		return IGameState::ptr(new CreditsState());
	}

	CreditsState();
	~CreditsState();

	void Suspend(){}
	void Resume(){}

	void HandleInput(const sf::Event& evt);
	void HandleAsyncInput(const sf::Input& input){}

	void Update();
	void Render(sf::RenderWindow& wnd);

private:
	sf::String mText[TEXT_MAX];
	sf::String mTankSmasher;

	int mCurrentText;
	float mUpdateTime;
	float mWaitTime;
	float mFinalStageTime;

	bool mShowFull;
	bool mStartedFinalStage;
	bool mDoneFinalStage;
};

#endif