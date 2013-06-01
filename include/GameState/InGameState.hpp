#ifndef TS_IN_GAME_STATE_HPP_
#define TS_IN_GAME_STATE_HPP_

#include "GameState/IGameState.hpp"
#include "World/GameWorld.hpp"

class InGameState: public IGameState{
public:
	enum{
		NPC_MAX = 3,
		PLAYER_MAX = 1
	};

public:
	static IGameState::ptr Create(){
		return IGameState::ptr(new InGameState());
	}

	InGameState();
	~InGameState();

	void Suspend();
	void Resume();

	void HandleInput(const sf::Event& evt);
	void HandleAsyncInput(const sf::Input& input);

	void Update();
	void Render(sf::RenderWindow& wnd);

private:
	bool mPaused;
	bool mGameOver;
	bool mGameWon;
	sf::String mMiscString;

	GameWorld* mGameWorld;
};

#endif