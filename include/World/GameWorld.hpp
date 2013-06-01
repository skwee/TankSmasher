#ifndef GAME_WORLD_HPP_
#define GAME_WORLD_HPP_

#include <SFML/Graphics.hpp>

#include "io/PlayerInputMapper.hpp"
#include "Entities/TankUnit.hpp"
#include "Collision/CollisionTester.hpp"
#include "Entities/PlayerData.hpp"
#include "Entities/EnemyData.hpp"
#include "World/GameStats.hpp"
#include "Effects/BulletEmitter.hpp"
#include "Effects/LevelSwitcher.hpp"
#include "Effects/BonusEmitter.hpp"

class GameWorld{
public:
	enum{
		RESERVED_AREA = 12,
		TILE_SIZE = 32,
		TILES_PER_X = 13,
		TILES_PER_Y = 13,
		WORLD_WIDTH = TILES_PER_X * TILE_SIZE,
		WORLD_HEIGHT = TILES_PER_Y * TILE_SIZE
	};

	enum{
		NPC_1 = 0, NPC_2, NPC_3, NPC_MAX
	};

	enum{
		TANK_UNIT_P = 0, TANK_UNIT_E1, TANK_UNIT_E2, TANK_UNIT_E3, TANK_UNIT_MAX
	};

	enum{
		TILE_NONE = 0, TILE_GLASS, TILE_WALL, TILE_WATER, TILE_GRASS, TILE_EAGLE, TILE_MAX
	};

	static const float ENEMY_SPAWN_TIME;

public:
	GameWorld(const std::string& levelFile);
	~GameWorld();

	void Draw(sf::RenderWindow& wnd);

	void HandleInput(bool paused, bool gameOver, const sf::Event& evt);
	void HandleAsyncInput(const sf::Input& input, bool paused, bool gameOver);
	void Update(float dt, bool paused, bool& gameOver, bool& gameWon);

private:
	void LoadWorld(const std::string& filename);
	void GenerateLevel();
	void SetSpawnPoints(bool firstTime);
	void CreateEagleAndProtection(bool eagle);

	void CreateTankUnits();
	void CreatePlayerController();

	void CheckCollisionWithWorld();
	void CheckCollisionWithTanks();

	void SetBonusToTank(int tank, int bonus);

	void UpdateEnemies(float dt);
	void EnemyThink(int enemy);

	sf::Shape mReservedArea[4];
	PlayerInputMapper mPlayerInput;
	PlayerData mPlayerData;
	EnemyData mEnemyData;

	TankUnit::ptr mTankUnits[TANK_UNIT_MAX];

	GameStats mGameStats;
	CollisionTester mCollisionTester;

	int mTileMapMask[TILES_PER_X][TILES_PER_Y];
	sf::Sprite mTiles[TILES_PER_X][TILES_PER_Y];

	TankData mTankData[TANK_UNIT_MAX];

	BulletEmitter::ptr mBulletEmitter;

	EnemyTankData mEnemy1Data;
	EnemyTankData mEnemy2Data;
	EnemyTankData mEnemy3Data;

	float mEnemySpawnTime;
	float mSwitchLevelTime;

	bool mEagleDead;
	sf::Vector2f mEaglePosition;

	LevelSwitcher mLevelSwitcher;
	BonusEmitter mBonusEmitter;

	bool mLeftPressed;
	bool mRightPressed;
	bool mUpPressed;
	bool mDownPressed;
};

#endif