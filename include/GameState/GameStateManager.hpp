#ifndef TS_GAME_STATE_MANAGER_HPP_
#define TS_GAME_STATE_MANAGER_HPP_

#include "GameState/IGameState.hpp"
#include "Logger.hpp"

class GameStateManager{
public:
	GameStateManager(){
		mStates.reserve(10);
		Logger::GetInstance()->Log("Game State Manager Created");
	}

	~GameStateManager(){
		mStates.clear();
		Logger::GetInstance()->Log("Game State Manager Destroyed");
	}

	void Push(const IGameState::ptr state){
		if( !mStates.empty()) mStates.back()->Suspend();
		mStates.push_back(state);
	}

	void Pop(){
		mStates.pop_back();
		if( !mStates.empty()) mStates.back()->Resume();
	}

	void HandleInput(const sf::Event& evt, const sf::Input& input){
		IGameState::ptr state = mStates.back();
		state->HandleInput(evt);
		if(mStates.empty()) return;
		state->HandleAsyncInput(input);
	}

	void Update(){
		mStates.back()->Update();
	}

	void RenderScene(sf::RenderWindow& wnd){
		mStates.back()->Render(wnd);
	}

	void SetFrameDeltaTime(float dt){
		mStates.back()->SetFrameRateDeltaTime(dt);
	}

	bool IsValid() const { return !mStates.empty(); }

private:
	std::vector<IGameState::ptr> mStates;
};

#endif