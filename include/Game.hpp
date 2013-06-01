#ifndef TS_GAME_HPP_
#define TS_GAME_HPP_

#include <SFML/Graphics.hpp>

#include "TankSmasherException.hpp"
#include "GameState/GameStateManager.hpp"
#include "Resource/FontManager.hpp"
#include "Resource/ImageManager.hpp"
#include "Resource/SoundManager.hpp"

typedef struct{
	GameStateManager* gameStateManager;
	FontManager* fontManager;
	ImageManager* imageManager;
	SoundManager* soundManager;
}GlobalContext;

GlobalContext& GetGlobalContext();

class Game{
public:
	enum{
		WINDOW_WIDTH = 576,
		WINDOW_HEIGHT = 440,
		WINDOW_BPP = 32
	};

	typedef struct{
		bool ShowFps;
		bool MusicOn;
		bool SoundOn;
	}GameSettings;

	static GameSettings gGameSettings;

public:
	Game();
	~Game();

	void Run(int argc, char* argv[]);

private:
	void Initialize(int argc, char* argv[]);
	void Clear();

	void HandleInput();

	sf::RenderWindow mWindow;
	bool mRunning;

	sf::Music mBackgroundMusic;
};

#endif