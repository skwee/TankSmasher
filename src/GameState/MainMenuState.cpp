#include "GameState/MainMenuState.hpp"
#include "GameState/InGameState.hpp"
#include "GameState/OptionsState.hpp"
#include "GameState/CreditsState.hpp"
#include "Game.hpp"

//std::string MainMenuState::gMenuNames[] = { "Play", "Options", "Help", "About", "Exit" };
std::string MainMenuState::gMenuNames[] = { "Play", "Options", "Credits", "Exit" };

MainMenuState::MainMenuState(){
	mNormalTextColor = sf::Color(25, 71, 131);
	mSelectedTextColor = sf::Color(255, 255, 255);

	FontManager::FontPtr font = GetGlobalContext().fontManager->Get("BattleCity.ttf;52");

	mTitle.SetText("Tank Smasher");
	mTitle.SetFont(*font);
	mTitle.SetSize(42);
	mTitle.SetColor(mNormalTextColor);
	mTitle.Move(40, 20);

	mAuthor[0].SetText("By Dmitry a.k.a. skwee");
	mAuthor[0].SetFont(*font);
	mAuthor[0].SetSize(14);
	mAuthor[0].SetColor(mNormalTextColor);
	mAuthor[0].Move(150, 80);

	mAuthor[1].SetText("For Battle City Contest on GameDev.ru");
	mAuthor[1].SetFont(*font);
	mAuthor[1].SetSize(14);
	mAuthor[1].SetColor(mNormalTextColor);
	mAuthor[1].Move(40, 100);

	for(int i = 0; i < MENU_MAX; ++i){
		mMenu[i].SetText(gMenuNames[i]);
		mMenu[i].SetFont(*font);
		mMenu[i].SetSize(24);
		mMenu[i].Move(200.f, 180.f + (i * 34));
	}

	mCopy.SetText("All Rights Reserved to Dmitry skwee Kudryavtsev 2010(c) s.kwee357@gmail.com");
	mCopy.SetFont(*GetGlobalContext().fontManager->Get("Vera.ttf;40"));
	mCopy.SetSize(11);
	mCopy.SetColor(mSelectedTextColor);
	mCopy.Move(50, Game::WINDOW_HEIGHT - 15);

	mSelectedOption = 0;
}

MainMenuState::~MainMenuState(){}

void MainMenuState::HandleInput(const sf::Event& evt){
	if(evt.Type == sf::Event::KeyReleased){
		if(evt.Key.Code == sf::Key::Escape) GetGlobalContext().gameStateManager->Pop();
	}else if(evt.Type == sf::Event::KeyPressed){
		if(evt.Key.Code == sf::Key::Up){
			mSelectedOption --;
		}else if(evt.Key.Code == sf::Key::Down){
			mSelectedOption ++;
		}else if(evt.Key.Code == sf::Key::Return){
			switch(mSelectedOption){
			case MENU_PLAY:
				GetGlobalContext().gameStateManager->Push(InGameState::Create());
				break;
			case MENU_OPTIONS:
				GetGlobalContext().gameStateManager->Push(OptionsState::Create());
				break;
			case MENU_CREDITS:
				GetGlobalContext().gameStateManager->Push(CreditsState::Create());
				break;
			/*case MENU_ABOUT:
				break;*/
			case MENU_EXIT:
				GetGlobalContext().gameStateManager->Pop();
				break;
			}
		}
	}
}

void MainMenuState::Update(){
	if(mSelectedOption >= MENU_MAX) mSelectedOption = 0;
	else if(mSelectedOption < 0) mSelectedOption = MENU_MAX - 1;
}

void MainMenuState::Render(sf::RenderWindow& wnd){
	wnd.Draw(mTitle);
	wnd.Draw(mAuthor[0]);
	wnd.Draw(mAuthor[1]);

	for(int i = 0; i < MENU_MAX; ++i){
		if(mSelectedOption == i) mMenu[i].SetColor(mSelectedTextColor);
		else mMenu[i].SetColor(mNormalTextColor);
		wnd.Draw(mMenu[i]);
	}

	wnd.Draw(mCopy);
}