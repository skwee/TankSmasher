#include "GameState/OptionsState.hpp"
#include "Game.hpp"

OptionsState::OptionsState(){
	mNormalTextColor = sf::Color(25, 71, 131);
	mSelectedTextColor = sf::Color(255, 255, 255);

	FontManager::FontPtr font = GetGlobalContext().fontManager->Get("BattleCity.ttf;52");

	mTitle.SetText("Options");
	mTitle.SetFont(*font);
	mTitle.SetSize(42);
	mTitle.SetColor(mNormalTextColor);
	mTitle.Move(150, 20);

	mOptions[OPTION_MUSIC].SetText("Music");
	mOptions[OPTION_MUSIC].SetFont(*font);
	mOptions[OPTION_MUSIC].SetSize(24);
	//mOptions[OPTION_MUSIC].SetColor(mNormalTextColor);
	mOptions[OPTION_MUSIC].Move(20, 100);

	//mOptionValues[OPTION_MUSIC].SetText();
	mOptionValues[OPTION_MUSIC].SetFont(*font);
	mOptionValues[OPTION_MUSIC].SetSize(24);
	//mOptionValues[OPTION_MUSIC].SetColor(mNormalTextColor);
	mOptionValues[OPTION_MUSIC].Move(500, 100);

	mOptions[OPTION_SOUND].SetText("Sound");
	mOptions[OPTION_SOUND].SetFont(*font);
	mOptions[OPTION_SOUND].SetSize(24);
	//mOptions[OPTION_MUSIC].SetColor(mNormalTextColor);
	mOptions[OPTION_SOUND].Move(20, 140);

	//mOptionValues[OPTION_MUSIC].SetText();
	mOptionValues[OPTION_SOUND].SetFont(*font);
	mOptionValues[OPTION_SOUND].SetSize(24);
	//mOptionValues[OPTION_MUSIC].SetColor(mNormalTextColor);
	mOptionValues[OPTION_SOUND].Move(500, 140);
}

OptionsState::~OptionsState(){}

void OptionsState::HandleInput(const sf::Event& evt){
	if(evt.Type == sf::Event::KeyReleased){
		if(evt.Key.Code == sf::Key::Escape)
			GetGlobalContext().gameStateManager->Pop();
	}else if(evt.Type == sf::Event::KeyPressed){
		if(evt.Key.Code == sf::Key::Down) mSelectedOption ++;
		else if(evt.Key.Code == sf::Key::Up) mSelectedOption --;
		else if(evt.Key.Code == sf::Key::Return){
			switch(mSelectedOption){
			case OPTION_MUSIC:
				Game::gGameSettings.MusicOn = !Game::gGameSettings.MusicOn;
				break;
			case OPTION_SOUND:
				Game::gGameSettings.SoundOn = !Game::gGameSettings.SoundOn;
			default:
				break;
			}
		}
	}
}

void OptionsState::Update(){
	if(mSelectedOption < 0) mSelectedOption = OPTION_MAX - 1;
	else if(mSelectedOption >= OPTION_MAX) mSelectedOption = 0;

	if(Game::gGameSettings.MusicOn){
		mOptionValues[OPTION_MUSIC].SetText("OFF");
	}else{
		mOptionValues[OPTION_MUSIC].SetText("ON");
	}

	if(Game::gGameSettings.SoundOn){
		mOptionValues[OPTION_SOUND].SetText("OFF");
	}else{
		mOptionValues[OPTION_SOUND].SetText("ON");
	}
}

void OptionsState::Render(sf::RenderWindow& wnd){
	wnd.Draw(mTitle);

	for(int i = 0; i < OPTION_MAX; ++i){
		if(mSelectedOption == i){
			mOptions[i].SetColor(mSelectedTextColor);
			mOptionValues[i].SetColor(mSelectedTextColor);
		}else{
			mOptions[i].SetColor(mNormalTextColor);
			mOptionValues[i].SetColor(mNormalTextColor);
		}
		wnd.Draw(mOptions[i]);
		wnd.Draw(mOptionValues[i]);
	}
}