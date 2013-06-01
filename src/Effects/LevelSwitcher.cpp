#include "Effects/LevelSwitcher.hpp"
#include "Game.hpp"

const float LevelSwitcher::DISPLAY_TIME = 1.f;
const float LevelSwitcher::FADEOUT_TIME = 1.f;

LevelSwitcher::LevelSwitcher(){
	mLevels.push_back("level7");
	mLevels.push_back("level6");
	mLevels.push_back("level5");
	mLevels.push_back("level4");
	mLevels.push_back("level3");
	mLevels.push_back("level2");
	mLevels.push_back("level1");

	mStarted = mDone = mDisplaying = false;

	mFadeOutTime = mDisplayTime = 0.f;
	mCurrentStage = 0;
}

LevelSwitcher::~LevelSwitcher(){
	mLevels.clear();
}

void LevelSwitcher::SwitcherStart(){
	mStarted = true;
	mDone = false;
	mDisplaying = false;
	mDisplayTime = mFadeOutTime = 0.f;
	mCurrentStage ++;
}

void LevelSwitcher::Update(float dt){
	if(mDisplaying){
		mDisplayTime += dt;
		if(mDisplayTime >= DISPLAY_TIME){
			mDone = true;
		}
	}else{
		mFadeOutTime += dt;
		if(mFadeOutTime >= FADEOUT_TIME){
			mDisplaying = true;
		}
	}
}

void LevelSwitcher::Draw(sf::RenderWindow& wnd){
	sf::Color c(170, 170, 170);

	if(mDisplaying){
		sf::Shape s = sf::Shape::Rectangle(0, 0, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, c);
		sf::String str;

		std::ostringstream ss;
		ss << "Stage " << mCurrentStage;

		str.SetFont(*GetGlobalContext().fontManager->Get("digital-7.ttf;52"));
		str.SetText(ss.str());
		str.SetPosition(Game::WINDOW_WIDTH / 2 - 80, Game::WINDOW_HEIGHT / 2 - 40);
		str.SetColor(sf::Color(0, 0, 0));
		str.SetSize(40);

		wnd.Draw(s);
		wnd.Draw(str);
	}else{
		int hh = Game::WINDOW_HEIGHT / 2;
		sf::Shape rtop, rbottom;
		float p = mFadeOutTime / FADEOUT_TIME;
		if(p > 1.f) p = 1.f;

		rtop = sf::Shape::Rectangle(0, 0, Game::WINDOW_WIDTH, hh * p, c);
		rbottom = sf::Shape::Rectangle(0, Game::WINDOW_HEIGHT, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT - hh * p, c);

		wnd.Draw(rtop);
		wnd.Draw(rbottom);
	}
}