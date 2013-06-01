#include <sstream>

#include "Game.hpp"
#include "GameState/MainMenuState.hpp"

GlobalContext gGlobalContext;

GlobalContext& GetGlobalContext(){
	return gGlobalContext;
}

Game::GameSettings Game::gGameSettings = { true, true, true };

Game::Game(){
	gGlobalContext.gameStateManager = new GameStateManager();
	gGlobalContext.fontManager = new FontManager();
	gGlobalContext.imageManager = new ImageManager();
	gGlobalContext.soundManager = new SoundManager();

	::srand(time(0));
}

Game::~Game(){
	SafeDelete<SoundManager>(gGlobalContext.soundManager);
	SafeDelete<ImageManager>(gGlobalContext.imageManager);
	SafeDelete<FontManager>(gGlobalContext.fontManager);
	SafeDelete<GameStateManager>(gGlobalContext.gameStateManager);
}

void Game::HandleInput(){
	sf::Event event;

	while(mWindow.GetEvent(event)){
		if(event.Type == sf::Event::Closed){
			mRunning = false;
			break;
		}

		gGlobalContext.gameStateManager->HandleInput(event, mWindow.GetInput());
		if( !gGlobalContext.gameStateManager->IsValid()) break;
	}
}

void Game::Initialize(int argc, char* argv[]){
	sf::VideoMode mode(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, Game::WINDOW_BPP);
	mWindow.Create(mode, "Tank Smasher", sf::Style::Close);
	mWindow.SetFramerateLimit(60);

	gGlobalContext.gameStateManager->Push(MainMenuState::Create());

	bool s = mBackgroundMusic.OpenFromFile("content/sounds/megaminute.ogg");
	if(!s){
		Logger::GetInstance()->Log("Cannot load music");
		throw TankSmasherException("Cannot load music"); 
	}
	mBackgroundMusic.SetLoop(true);
	mBackgroundMusic.SetVolume(100.f);
	if(gGameSettings.MusicOn) mBackgroundMusic.Play();
	else mBackgroundMusic.Pause();
}

void Game::Clear(){
	mBackgroundMusic.Stop();
	if(mWindow.IsOpened()) mWindow.Close();
}

void Game::Run(int argc, char* argv[]){
	Initialize(argc, argv);

	mRunning = gGlobalContext.gameStateManager->IsValid();

	while(mRunning){
		float dt = mWindow.GetFrameTime();
		gGlobalContext.gameStateManager->SetFrameDeltaTime(dt);

		HandleInput();
		if(!gGlobalContext.gameStateManager->IsValid()) mRunning = false;
		if(!mRunning) break;

		gGlobalContext.gameStateManager->Update();

		mWindow.Clear();

		gGlobalContext.gameStateManager->RenderScene(mWindow);

		mWindow.Display();

		sf::Sound::Status s = mBackgroundMusic.GetStatus();
		if(gGameSettings.MusicOn){
			if(s == sf::Sound::Paused) mBackgroundMusic.Play();
		}else{
			if(s == sf::Sound::Playing) mBackgroundMusic.Pause();
		}

		sf::Sleep(0.001f);
	}

	Clear();
}