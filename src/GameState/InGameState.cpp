#include "GameState/InGameState.hpp"
#include "GameState/CreditsState.hpp"
#include "Game.hpp"

InGameState::InGameState(){
	mPaused = mGameWon = mGameOver = false;

	mMiscString.SetFont(*GetGlobalContext().fontManager->Get("BattleCity.ttf;52"));
	mMiscString.SetSize(52);
	mMiscString.SetColor(sf::Color(25, 71, 131));
	mMiscString.Move(60, 200);

	mGameWorld = new GameWorld("demo");
}

InGameState::~InGameState(){
	SafeDelete<GameWorld>(mGameWorld);
}

void InGameState::Suspend(){}
void InGameState::Resume(){}

void InGameState::HandleInput(const sf::Event& evt){
	if(evt.Type == sf::Event::KeyReleased){
		if(evt.Key.Code == sf::Key::Escape){
			GetGlobalContext().gameStateManager->Pop();
		}else if(evt.Key.Code == sf::Key::P && ! mGameOver){
			mPaused = !mPaused;
		}
	}
	if(evt.Type == sf::Event::LostFocus)
		mPaused = true;

	mGameWorld->HandleInput(mPaused, mGameOver, evt);
}

void InGameState::HandleAsyncInput(const sf::Input& input){
	if(mGameWon) return;
	mGameWorld->HandleAsyncInput(input, mPaused, mGameOver);
}

void InGameState::Update(){
	mGameWorld->Update(mFrameRateDeltaTime, mPaused, mGameOver, mGameWon);
	if(mGameWon){
		GetGlobalContext().gameStateManager->Pop();
		GetGlobalContext().gameStateManager->Push(CreditsState::Create());
	}
}

void InGameState::Render(sf::RenderWindow& wnd){
	mGameWorld->Draw(wnd);

	if(mGameOver){
		mMiscString.SetText("Game Over");
		wnd.Draw(mMiscString);
	}else if(mGameWon){
		mMiscString.SetText("You Win!");
		wnd.Draw(mMiscString);
	}else if(mPaused){
		mMiscString.SetText("Paused");
		wnd.Draw(mMiscString);
	}
}