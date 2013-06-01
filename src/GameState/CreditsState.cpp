#include "GameState/CreditsState.hpp"

#include "Game.hpp"

const float CreditsState::UPDATE_TIME = 2.f;
const float CreditsState::WAIT_TIME = 3.f;

CreditsState::CreditsState(){
	FontManager::FontPtr font = GetGlobalContext().fontManager->Get("Vera.ttf;40");

	mText[TEXT_PROGRAMMING].SetText("          Programming\n\nDmitry skwee Kudryavtsev");
	mText[TEXT_PROGRAMMING].SetFont(*font);
	mText[TEXT_PROGRAMMING].SetSize(20);
	mText[TEXT_PROGRAMMING].SetPosition((Game::WINDOW_WIDTH / 2) - 140, (Game::WINDOW_HEIGHT / 2) - 80);
	mText[TEXT_PROGRAMMING].SetBlendMode(sf::Blend::Alpha);

	mText[TEXT_MUSIC].SetText("  Background Music\n\n      Megaminute\n\nComposed By Akcija");
	mText[TEXT_MUSIC].SetFont(*font);
	mText[TEXT_MUSIC].SetSize(20);
	mText[TEXT_MUSIC].SetPosition((Game::WINDOW_WIDTH / 2) - 120, (Game::WINDOW_HEIGHT / 2) - 80);
	mText[TEXT_MUSIC].SetBlendMode(sf::Blend::Alpha);

	mText[TEXT_SOUND].SetText("  Sound Effects\n\nSoundBible.com");
	mText[TEXT_SOUND].SetFont(*font);
	mText[TEXT_SOUND].SetSize(20);
	mText[TEXT_SOUND].SetPosition((Game::WINDOW_WIDTH / 2) - 100, (Game::WINDOW_HEIGHT / 2) - 80);
	mText[TEXT_SOUND].SetBlendMode(sf::Blend::Alpha);

	mText[TEXT_MISC].SetText("        Special Thanks\n\n Unknown Sprite Authors\n\nGameDev.ru Beta Testers");
	mText[TEXT_MISC].SetFont(*font);
	mText[TEXT_MISC].SetSize(20);
	mText[TEXT_MISC].SetPosition((Game::WINDOW_WIDTH / 2) - 140, (Game::WINDOW_HEIGHT / 2) - 80);
	mText[TEXT_MISC].SetBlendMode(sf::Blend::Alpha);

	mTankSmasher.SetText("Tank Smasher");
	mTankSmasher.SetFont(*GetGlobalContext().fontManager->Get("BattleCity.ttf;52"));
	mTankSmasher.SetSize(42);
	mTankSmasher.SetPosition(40, Game::WINDOW_HEIGHT);
	mTankSmasher.SetColor(sf::Color(25, 71, 131));

	mUpdateTime = mWaitTime = mFinalStageTime = 0.f;
	mCurrentText = 0;
	mShowFull = true;
	mStartedFinalStage = mDoneFinalStage = false;
}

CreditsState::~CreditsState(){}

void CreditsState::HandleInput(const sf::Event& evt){
	if(evt.Type == sf::Event::KeyReleased){
		if(evt.Key.Code == sf::Key::Escape) GetGlobalContext().gameStateManager->Pop();
		else if(evt.Key.Code == sf::Key::Space){
			if(mStartedFinalStage) return;
			mCurrentText ++;
			mUpdateTime = mWaitTime = 0.f;
			mShowFull = true;
			if(mCurrentText >= TEXT_MAX){
				mStartedFinalStage = true;
			}
		}
	}
}

void CreditsState::Update(){
	mUpdateTime += mFrameRateDeltaTime;
	if(!mStartedFinalStage){
		if(mUpdateTime >= UPDATE_TIME){
			if(mShowFull){
				mWaitTime += mFrameRateDeltaTime;
				if(mWaitTime >= WAIT_TIME){
					mWaitTime = 0.f;
					mUpdateTime = 0.f;
					mShowFull = false;
				}
			}else{
				mShowFull = true;
				mCurrentText ++;
				if(mCurrentText >= TEXT_MAX){ mStartedFinalStage = true; }
				mUpdateTime = 0.f;
			}
		}
	}else{
		if(mDoneFinalStage){
			GetGlobalContext().gameStateManager->Pop();
		}else{
			if(mTankSmasher.GetPosition().y <= ((Game::WINDOW_HEIGHT / 2) - 50)){
				if(mUpdateTime >= (WAIT_TIME + UPDATE_TIME)) mDoneFinalStage = true;
			}else{
				mTankSmasher.Move(0, -50 * mFrameRateDeltaTime);
				mUpdateTime = 0.f;
			}
		}
	}
}

void CreditsState::Render(sf::RenderWindow& wnd){
	if(mStartedFinalStage){
		wnd.Draw(mTankSmasher);
	}else{
		int alpha = static_cast<int>((mUpdateTime / UPDATE_TIME) * 255);
		if(alpha > 255) alpha = 255;
		if(!mShowFull) alpha = 255 - alpha;

		mText[mCurrentText].SetColor(sf::Color(255, 255, 255, alpha));
		wnd.Draw(mText[mCurrentText]);
	}
}