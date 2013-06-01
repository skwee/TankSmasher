#include "World/GameWorld.hpp"
#include "Game.hpp"
#include "ai/TankAI.hpp"

#include <fstream>

const float GameWorld::ENEMY_SPAWN_TIME = 3.f;

GameWorld::GameWorld(const std::string& levelFile){
	mLeftPressed = mRightPressed = mDownPressed = mUpPressed = false;
	sf::Color c(170, 170, 170);

	mReservedArea[0] = sf::Shape::Rectangle(0, 0, 
		WORLD_WIDTH + 2 * RESERVED_AREA, RESERVED_AREA, c);
	mReservedArea[0].EnableFill(true);

	mReservedArea[1] = sf::Shape::Rectangle(0, WORLD_HEIGHT + RESERVED_AREA,
		WORLD_WIDTH + 2 * RESERVED_AREA, WORLD_HEIGHT + 2 * RESERVED_AREA, c);
	mReservedArea[1].EnableFill(true);

	mReservedArea[2] = sf::Shape::Rectangle(0, RESERVED_AREA, RESERVED_AREA, WORLD_HEIGHT + RESERVED_AREA, c);
	mReservedArea[2].EnableFill(true);

	mReservedArea[3] = sf::Shape::Rectangle(WORLD_WIDTH + RESERVED_AREA, RESERVED_AREA,
		WORLD_WIDTH + 2 * RESERVED_AREA, WORLD_HEIGHT + RESERVED_AREA, c);
	mReservedArea[3].EnableFill(true);

	mEnemySpawnTime = 0.f;

	mEagleDead = false;

	mBulletEmitter = BulletEmitter::Create();
	mLevelSwitcher.SwitcherStart();
	LoadWorld(mLevelSwitcher.GetNextLevel());
	SetSpawnPoints(true);
	//GenerateLevel();
	CreateTankUnits();
	CreatePlayerController();

	mSwitchLevelTime = 0.f;

	mEnemy1Data.ChangeDirection = mEnemy2Data.ChangeDirection = mEnemy3Data.ChangeDirection = false;
	mEnemy1Data.CurrentDirection = TankAI::GetInitialDirection();
	mEnemy2Data.CurrentDirection = TankAI::GetInitialDirection();
	mEnemy3Data.CurrentDirection = TankAI::GetInitialDirection();
	mEnemy1Data.WantShootTime = mEnemy2Data.WantShootTime = mEnemy3Data.WantShootTime = 0.f;
	mEnemy1Data.WantChangeDirectionTime = mEnemy2Data.WantChangeDirectionTime = mEnemy3Data.WantChangeDirectionTime
		= 0.f;
}

GameWorld::~GameWorld(){}

void GameWorld::Draw(sf::RenderWindow& wnd){
	if(mLevelSwitcher.SwitcherActive() && !mLevelSwitcher.SwitcherDone()){
		mLevelSwitcher.Draw(wnd);
		return;
	}
	mTankUnits[TANK_UNIT_P]->Draw(wnd);
	mTankUnits[TANK_UNIT_E1]->Draw(wnd);
	mTankUnits[TANK_UNIT_E2]->Draw(wnd);
	mTankUnits[TANK_UNIT_E3]->Draw(wnd);

	wnd.Draw(mReservedArea[0]);
	wnd.Draw(mReservedArea[1]);
	wnd.Draw(mReservedArea[2]);
	wnd.Draw(mReservedArea[3]);

	mGameStats.Draw(wnd, mPlayerData, mEnemyData, mTankUnits[TANK_UNIT_P]->GetTankData().bulletType);

	for(int j = 0; j < TILES_PER_Y; ++j){
		for(int i = 0; i < TILES_PER_X; ++i){
			if(mTileMapMask[i][j] != 0){
				wnd.Draw(mTiles[i][j]);
			}
		}
	}

	mBonusEmitter.Draw(wnd);
	mBulletEmitter->Draw(wnd);
}

void GameWorld::HandleInput(bool paused, bool gameOver, const sf::Event& evt){
	if(mLevelSwitcher.SwitcherActive() && !mLevelSwitcher.SwitcherDone()) return;
	if(!paused && !gameOver){
		if(evt.Type == sf::Event::KeyPressed){
			if(evt.Key.Code == sf::Key::Left){
				mLeftPressed = true;
				mRightPressed = mUpPressed = mDownPressed = false;
			}else if(evt.Key.Code == sf::Key::Right){
				mRightPressed = true;
				mLeftPressed = mUpPressed = mDownPressed = false;
			}else if(evt.Key.Code == sf::Key::Up){
				mUpPressed = true;
				mLeftPressed = mRightPressed = mDownPressed = false;
			}else if(evt.Key.Code == sf::Key::Down){
				mDownPressed = true;
				mLeftPressed = mRightPressed = mUpPressed = false;
			}
		}
	}
}

void GameWorld::HandleAsyncInput(const sf::Input& input, bool paused, bool gameOver){
	if(mLevelSwitcher.SwitcherActive() && !mLevelSwitcher.SwitcherDone()) return;
	if(!paused && !gameOver){
		if(!mTankUnits[TANK_UNIT_P]->IsDead()){
			if(input.IsKeyDown(mPlayerInput.Get(PlayerInputMapper::MOVE_FORWARD)) && mUpPressed)
				mTankUnits[TANK_UNIT_P]->Move(Tank::TOP);
			else if(input.IsKeyDown(mPlayerInput.Get(PlayerInputMapper::MOVE_BACKWARD)) && mDownPressed)
				mTankUnits[TANK_UNIT_P]->Move(Tank::BOTTOM);
			else if(input.IsKeyDown(mPlayerInput.Get(PlayerInputMapper::MOVE_LEFT)) && mLeftPressed)
				mTankUnits[TANK_UNIT_P]->Move(Tank::LEFT);
			else if(input.IsKeyDown(mPlayerInput.Get(PlayerInputMapper::MOVE_RIGHT)) && mRightPressed)
				mTankUnits[TANK_UNIT_P]->Move(Tank::RIGHT);
			else
				mTankUnits[TANK_UNIT_P]->StopMoving();

			if(input.IsKeyDown(mPlayerInput.Get(PlayerInputMapper::SHOOT)))
				mTankUnits[TANK_UNIT_P]->Shoot(TANK_UNIT_P);
		}
	}
}

void GameWorld::Update(float dt, bool paused, bool& gameOver, bool& gameWon){
	if(mLevelSwitcher.SwitcherActive() && !mLevelSwitcher.SwitcherDone()){
		mLevelSwitcher.Update(dt);
		return;
	}
	if(!paused && !gameWon){
		mEnemySpawnTime += dt;

		if(mTankUnits[TANK_UNIT_P]->IsDead()){
			if(!gameOver){
				mPlayerData.DecLive();
				if(mPlayerData.IsDead()){
					gameOver = true;
					return;
				}else{
					mTankUnits[TANK_UNIT_P]->Respawn(true);
				}
			}
		}

		if(mTankUnits[TANK_UNIT_E1]->IsDead() && 
			mTankUnits[TANK_UNIT_E2]->IsDead() &&
			mTankUnits[TANK_UNIT_E3]->IsDead() && mEnemyData.IsDead() && !gameOver){
				mSwitchLevelTime += dt;
				if(mSwitchLevelTime >= 4.f){
					mSwitchLevelTime = 0.f;
					if(mLevelSwitcher.HaveMoreLevels()){
						mLevelSwitcher.SwitcherStart();
						LoadWorld(mLevelSwitcher.GetNextLevel());
						SetSpawnPoints(false);
						mTankUnits[TANK_UNIT_P]->Respawn(false);
						mTankUnits[TANK_UNIT_E1]->Respawn(true);
						mTankUnits[TANK_UNIT_E2]->Respawn(true);
						mTankUnits[TANK_UNIT_E3]->Respawn(true);
						mBulletEmitter->ClearAll();
						mBonusEmitter.Reset();
					}else{
						gameWon = true;
						return;
					}
				}
		}

		if(mEnemySpawnTime >= ENEMY_SPAWN_TIME){
			mEnemySpawnTime = 0.f;
			if(!mEnemyData.IsDead()){
				if(mTankUnits[TANK_UNIT_E1]->IsDead()){
					mEnemy1Data.WantShootTime = 0.f;
					mEnemy1Data.WantChangeDirectionTime = 0.f;
					mTankUnits[TANK_UNIT_E1]->GetTankData().type = mEnemyData.GetEnemy();
					mTankUnits[TANK_UNIT_E1]->Respawn(true);
					mEnemy1Data.CurrentDirection = TankAI::GetInitialDirection();
				}else if(mTankUnits[TANK_UNIT_E2]->IsDead()){
					mEnemy2Data.WantShootTime = 0.f;
					mEnemy2Data.WantChangeDirectionTime = 0.f;
					mTankUnits[TANK_UNIT_E2]->GetTankData().type = mEnemyData.GetEnemy();
					mTankUnits[TANK_UNIT_E2]->Respawn(true);
					mEnemy2Data.CurrentDirection = TankAI::GetInitialDirection();
				}else if(mTankUnits[TANK_UNIT_E3]->IsDead()){
					mEnemy3Data.WantShootTime = 0.f;
					mEnemy3Data.WantChangeDirectionTime = 0.f;
					mTankUnits[TANK_UNIT_E3]->GetTankData().type = mEnemyData.GetEnemy();
					mTankUnits[TANK_UNIT_E3]->Respawn(true);
					mEnemy3Data.CurrentDirection = TankAI::GetInitialDirection();
				}
			}
		}

		UpdateEnemies(dt);

		CheckCollisionWithWorld();
		CheckCollisionWithTanks();

		mTankUnits[TANK_UNIT_P]->Update(dt);
		mTankUnits[TANK_UNIT_E1]->Update(dt);
		mTankUnits[TANK_UNIT_E2]->Update(dt);
		mTankUnits[TANK_UNIT_E3]->Update(dt);

		mBonusEmitter.Update(dt);
		mBulletEmitter->Update(dt);

		if(mEagleDead) gameOver = true;
	}
}

void GameWorld::LoadWorld(const std::string& filename){
	std::ifstream file;
	file.open("content/maps/" + filename + ".tsmap");
	if(!file.is_open()){
		throw TankSmasherException("Cannot load map file " + filename);
	}

	ImageManager::ImagePtr tiles = GetGlobalContext().imageManager->Get("world-tiles.png");

	int tmp;
	for(int j = 0; j < TILES_PER_Y; ++j){
		for(int i = 0; i < TILES_PER_X; ++i){
			if(file.eof()){
				file.clear();
				throw TankSmasherException("Bad or corrupted map file: " + filename);
			}

			file >> tmp;
			if(tmp == 0){
				mTileMapMask[i][j] = 0;
			}else{
				if(tmp < 0 || tmp >= TILE_MAX){
					file.close();
					throw TankSmasherException("Bad or corrupted map file (unsupported tiles): " + filename);
				}
				mTileMapMask[i][j] = tmp;
				int tindex = (tmp - 1) * TILE_SIZE;
				mTiles[i][j].SetImage(*tiles);
				mTiles[i][j].SetCenter(0, 0);
				mTiles[i][j].SetPosition(static_cast<float>(RESERVED_AREA + i * TILE_SIZE),
					static_cast<float>(RESERVED_AREA + j * TILE_SIZE));
				mTiles[i][j].SetSubRect(sf::IntRect(tindex, 0, tindex + TILE_SIZE, TILE_SIZE));
			}
		}
	}

	file.close();

	CreateEagleAndProtection(true);
}

void GameWorld::CreateEagleAndProtection(bool eagle){
	int x, y;
	x = TILES_PER_X / 2;
	y = TILES_PER_Y - 1;
	if(eagle){
		mTileMapMask[x][y] = TILE_EAGLE;
		mTiles[x][y].SetImage(*GetGlobalContext().imageManager->Get("eagle-sprite.png"));
		mTiles[x][y].SetCenter(0, 0);
		mTiles[x][y].SetPosition(static_cast<float>(RESERVED_AREA + x * TILE_SIZE),
			static_cast<float>(RESERVED_AREA + y * TILE_SIZE));
		mTiles[x][y].SetSubRect(sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE));
		mEaglePosition = mTiles[x][y].GetPosition();
	}

	ImageManager::ImagePtr tiles = GetGlobalContext().imageManager->Get("world-tiles.png");
	int tindex = (TILE_WALL - 1) * TILE_SIZE;
	int half = TILE_SIZE / 2;
	sf::Vector2f p;
	sf::IntRect r;
	r.Left = tindex;
	r.Top = 0;
	r.Right = tindex + TILE_SIZE;
	r.Bottom = TILE_SIZE;

	//top
	mTileMapMask[x][y-1] = TILE_WALL;
	mTiles[x][y-1].SetImage(*tiles);
	mTiles[x][y-1].SetCenter(0, 0);
	p.x = static_cast<float>(RESERVED_AREA + (x) * TILE_SIZE);
	p.y = static_cast<float>(RESERVED_AREA + (y-1) * TILE_SIZE);
	p.y += half;
	mTiles[x][y-1].SetPosition(p);
	mTiles[x][y-1].SetSubRect(sf::IntRect(r.Left, r.Top + half, r.Right, r.Bottom));

	//left
	mTileMapMask[x-1][y] = TILE_WALL;
	mTiles[x-1][y].SetImage(*tiles);
	mTiles[x-1][y].SetCenter(0, 0);
	p.x = static_cast<float>(RESERVED_AREA + (x-1) * TILE_SIZE);
	p.y = static_cast<float>(RESERVED_AREA + y * TILE_SIZE);
	p.x += half;
	mTiles[x-1][y].SetPosition(p);
	mTiles[x-1][y].SetSubRect(sf::IntRect(r.Left + half, r.Top, r.Right, r.Bottom));

	//left top
	mTileMapMask[x-1][y-1] = TILE_WALL;
	mTiles[x-1][y-1].SetImage(*tiles);
	mTiles[x-1][y-1].SetCenter(0, 0);
	p.x = static_cast<float>(RESERVED_AREA + (x-1) * TILE_SIZE);
	p.y = static_cast<float>(RESERVED_AREA + (y-1) * TILE_SIZE);
	p.x += half;
	p.y += half;
	mTiles[x-1][y-1].SetPosition(p);
	mTiles[x-1][y-1].SetSubRect(sf::IntRect(r.Left + half, r.Top + half, r.Right, r.Bottom));

	//right
	mTileMapMask[x+1][y] = TILE_WALL;
	mTiles[x+1][y].SetImage(*tiles);
	mTiles[x+1][y].SetCenter(0, 0);
	p.x = static_cast<float>(RESERVED_AREA + (x+1) * TILE_SIZE);
	p.y = static_cast<float>(RESERVED_AREA + y * TILE_SIZE);
	//p.x += half;
	mTiles[x+1][y].SetPosition(p);
	mTiles[x+1][y].SetSubRect(sf::IntRect(r.Left, r.Top, r.Right - half, r.Bottom));
	
	//right top
	mTileMapMask[x+1][y-1] = TILE_WALL;
	mTiles[x+1][y-1].SetImage(*tiles);
	mTiles[x+1][y-1].SetCenter(0, 0);
	p.x = static_cast<float>(RESERVED_AREA + (x+1) * TILE_SIZE);
	p.y = static_cast<float>(RESERVED_AREA + (y-1) * TILE_SIZE);
	//p.x -= half;
	p.y += half;
	mTiles[x+1][y-1].SetPosition(p);
	mTiles[x+1][y-1].SetSubRect(sf::IntRect(r.Left, r.Top, r.Right - half, r.Bottom - half));
}

void GameWorld::GenerateLevel(){
	//clear the world
	for(int j = 0; j < TILES_PER_Y; ++j){
		for(int i = 0; i < TILES_PER_X; ++i){
			mTileMapMask[i][j] = TILE_NONE;
		}
	}

	//generate random unpassable areas
	/*int iterations = (rand() % 5) + 5;
	while(iterations-- != 0){
		int tile = (iterations % 2) ? TILE_WATER : TILE_GLASS;
		int y = rand() % TILES_PER_Y;
		int x;
		for(x = 0; x < TILES_PER_X; ++x){
			if(rand() % 2) mTileMapMask[x][y] = tile;
			else mTileMapMask[x][y] = TILE_NONE;
		}
	}*/

	int iterations = 16;
	while(iterations-- != 0){
		char axis = (iterations % 2) ? 'x' : 'y';
		axis = 'y';
		int div = 0;
		if(axis == 'x') div = rand() % TILES_PER_Y;
		else div = rand() % TILES_PER_X;

		/*int t = rand() % (TILE_MAX - 1);
		t += 1;*/

		int tr = rand() % 5;
		int t = TILE_NONE;

		if(tr == 0) t = TILE_WATER;
		else if(tr == 1) t = TILE_GLASS;
		else t = TILE_WALL;

		int tiles = (axis == 'x') ? TILES_PER_Y : TILES_PER_X;
		for(int i = 0; i < tiles; ++i){
			if(axis == 'x') mTileMapMask[div][i] = t;
			else mTileMapMask[i][div] = t;
		}

		int kf = rand() % 6 + 1;
		while(kf-- != 0){
			int idx = rand() % tiles;
			if(axis == 'x') mTileMapMask[div][idx] = TILE_NONE;
			else mTileMapMask[idx][div] = TILE_NONE;
		}
	}

	//clear sides
	for(int i = 0; i < TILES_PER_X; ++i){
		mTileMapMask[i][0] = mTileMapMask[i][TILES_PER_Y - 1] = TILE_NONE;
		mTileMapMask[i][1] = mTileMapMask[i][TILES_PER_Y - 2] = TILE_NONE;
	}
	/*for(int i = 0; i < TILES_PER_Y; ++i){
		//mTileMapMask[TILES_PER_X - 1][i] = TILE_NONE;
		//mTileMapMask[1][i] = TILE_NONE;
	}*/
	

	ImageManager::ImagePtr tiles = GetGlobalContext().imageManager->Get("world-tiles.png");

	for(int j = 0; j < TILES_PER_Y; ++j){
		for(int i = 0; i < TILES_PER_X; ++i){
			if(mTileMapMask[i][j] != TILE_NONE){
				int tindex = (mTileMapMask[i][j] - 1) * TILE_SIZE;
				mTiles[i][j].SetImage(*tiles);
				mTiles[i][j].SetPosition(static_cast<float>(RESERVED_AREA + i * TILE_SIZE),
					static_cast<float>(RESERVED_AREA + j * TILE_SIZE));
				mTiles[i][j].SetCenter(0, 0);
				mTiles[i][j].SetSubRect(sf::IntRect(tindex, 0, tindex + TILE_SIZE, TILE_SIZE));
			}
		}
	}
}

void GameWorld::SetSpawnPoints(bool firstTime){
	//Set spawn points
	mTankData[TANK_UNIT_P].position.x = ((TILES_PER_X / 2) - 1) * TILE_SIZE - 4;
	mTankData[TANK_UNIT_P].position.y = (TILES_PER_Y) * TILE_SIZE - 6;
	mTankData[TANK_UNIT_P].viewAngle = 90;
	mTankData[TANK_UNIT_P].speed = 1.f;

	if(firstTime){
		mTankData[TANK_UNIT_P].bulletEmitter = mBulletEmitter;
		mTankData[TANK_UNIT_P].bulletType = Bullet::TYPE_SIMPLE;
		mTankData[TANK_UNIT_P].type = Tank::TYPE_GREEN;
	}

	mTankData[TANK_UNIT_E1].position.x = 29;
	mTankData[TANK_UNIT_E1].position.y = 29;
	mTankData[TANK_UNIT_E1].viewAngle = 270;
	mTankData[TANK_UNIT_E1].speed = 1.f;
	mTankData[TANK_UNIT_E1].bulletEmitter = mBulletEmitter;
	mTankData[TANK_UNIT_E1].bulletType = Bullet::TYPE_SIMPLE;

	mTankData[TANK_UNIT_E2].position.x = ((TILES_PER_X / 2) + 1) * TILE_SIZE - 4;
	mTankData[TANK_UNIT_E2].position.y = 29;
	mTankData[TANK_UNIT_E2].viewAngle = 270;
	mTankData[TANK_UNIT_E2].speed = 1.f;
	mTankData[TANK_UNIT_E2].bulletEmitter = mBulletEmitter;
	mTankData[TANK_UNIT_E2].bulletType = Bullet::TYPE_SIMPLE;

	mTankData[TANK_UNIT_E3].position.x = (TILES_PER_X) * TILE_SIZE - 5;
	mTankData[TANK_UNIT_E3].position.y = 29;
	mTankData[TANK_UNIT_E3].viewAngle = 270;
	mTankData[TANK_UNIT_E3].speed = 1.f;
	mTankData[TANK_UNIT_E3].bulletEmitter = mBulletEmitter;
	mTankData[TANK_UNIT_E3].bulletType = Bullet::TYPE_SIMPLE;

	int enemies = (rand() % 17) + 3;
	while(enemies-- != 0){
		int e = (rand() % (Tank::TYPE_MAX - 1)) + 1;
		mEnemyData.AddEnemy(e);
	}

	mTankData[TANK_UNIT_E1].type = mEnemyData.GetEnemy();
	mTankData[TANK_UNIT_E2].type = mEnemyData.GetEnemy();
	mTankData[TANK_UNIT_E3].type = mEnemyData.GetEnemy();
}

void GameWorld::CreateTankUnits(){
	mTankUnits[TANK_UNIT_P] = TankUnit::Create(mTankData[TANK_UNIT_P]);
	mTankUnits[TANK_UNIT_P]->Respawn();

	mTankUnits[TANK_UNIT_E1] = TankUnit::Create(mTankData[TANK_UNIT_E1]);
	mTankUnits[TANK_UNIT_E1]->Respawn();

	mTankUnits[TANK_UNIT_E2] = TankUnit::Create(mTankData[TANK_UNIT_E2]);
	mTankUnits[TANK_UNIT_E2]->Respawn();

	mTankUnits[TANK_UNIT_E3] = TankUnit::Create(mTankData[TANK_UNIT_E3]);
	mTankUnits[TANK_UNIT_E3]->Respawn();
}

void GameWorld::CreatePlayerController(){
	mPlayerInput.Set(sf::Key::Down, PlayerInputMapper::MOVE_BACKWARD);
	mPlayerInput.Set(sf::Key::Up, PlayerInputMapper::MOVE_FORWARD);
	mPlayerInput.Set(sf::Key::Left, PlayerInputMapper::MOVE_LEFT);
	mPlayerInput.Set(sf::Key::Right, PlayerInputMapper::MOVE_RIGHT);
	mPlayerInput.Set(sf::Key::Space, PlayerInputMapper::SHOOT);
}

void GameWorld::CheckCollisionWithWorld(){
	const std::vector<Bullet::ptr> bullets = mBulletEmitter->GetBullets();

	//check tanks with world bounds and bonus
	int hw = Tank::WIDHT / 2;
	int hh = Tank::HEIGHT / 2;
	for(int i = 0; i< TANK_UNIT_MAX; ++i){
		if(mTankUnits[i]->IsDead() || mTankUnits[i]->IsTankDead()) continue;
		sf::Vector2f p = mTankUnits[i]->GetPosition();

		//world bounds
		if(((p.x - hw) <= RESERVED_AREA) ||
			((p.x + hw) >= (WORLD_WIDTH + RESERVED_AREA)) ||
			((p.y - hh) <= RESERVED_AREA) ||
			((p.y + hh) >= (WORLD_HEIGHT + RESERVED_AREA))){
				mTankUnits[i]->OnCollideWithWorld();
				if(i == TANK_UNIT_E1) mEnemy1Data.ChangeDirection = true;
				else if(i == TANK_UNIT_E2) mEnemy2Data.ChangeDirection = true;
				else if(i == TANK_UNIT_E3) mEnemy3Data.ChangeDirection = true;
		}

		//bonus
		if(mBonusEmitter.HaveBonus()){
			if(mCollisionTester.PixelPerfectTest(mTankUnits[i]->GetSprite(), mBonusEmitter.GetBonus()->GetSprite())){
				SetBonusToTank(i, mBonusEmitter.GetBonus()->GetType());
				mBonusEmitter.Reset();
			}
		}
	}

	//check bullets with world bounds, tanks and bullets
	for(unsigned int i = 0; i < bullets.size(); ++i){
		if(bullets[i]->MustDie) continue;
		for(unsigned int j = 0; j < bullets.size(); ++j){
			if(i == j) continue;
			if(bullets[j]->MustDie) continue;
			if(mCollisionTester.PixelPerfectTest(bullets[i]->GetSprite(), bullets[j]->GetSprite())){
				bullets[i]->MustDie = true;
				bullets[j]->MustDie = true;
				break;
			}
		}

		if(bullets[i]->MustDie) continue;
		sf::Vector2f p = bullets[i]->GetSprite().GetPosition();
		bullets[i]->Get(hw, hh);

		//world bounds
		if(((p.x - hw) <= RESERVED_AREA) ||
			((p.x + hw) >= (WORLD_WIDTH + RESERVED_AREA)) ||
			((p.y - hh) <= RESERVED_AREA) ||
			((p.y + hh) >= (WORLD_HEIGHT + RESERVED_AREA))){
				bullets[i]->MustDie = true;
				continue;
		}

		//tanks
		for(int t = 0; t < TANK_UNIT_MAX; ++t){
			if(mTankUnits[t]->IsDead() || mTankUnits[t]->IsTankDead() || 
				(mTankUnits[t]->GetSpawn()->WasStarted() && !mTankUnits[t]->GetSpawn()->IsDone())) continue;

			if(mCollisionTester.PixelPerfectTest(bullets[i]->GetSprite(), mTankUnits[t]->GetSprite())){
				int shooter = bullets[i]->GetShooter();
				if(t == shooter) continue;
				if(shooter == TANK_UNIT_P){
					if(t != TANK_UNIT_P) mPlayerData.AddScore(PlayerData::SCORE_FOR_TANK_HIT);
					bullets[i]->MustDie = true;
					mTankUnits[t]->OnCollideWithBullet();
				}else{
					if(t == TANK_UNIT_P){
						bullets[i]->MustDie = true;
						mTankUnits[t]->OnCollideWithBullet();
					}
				}
			}
		}
	}

	//check world with others
	for(int iy = 0; iy < TILES_PER_Y; ++iy){
		for(int ix = 0; ix < TILES_PER_X; ++ix){
			if(mTileMapMask[ix][iy] == 0) continue;
			if(mTileMapMask[ix][iy] == TILE_GRASS) continue;

			//check with tanks
			for(int i = 0; i < TANK_UNIT_MAX; ++i){
				if(mTankUnits[i]->IsDead()) continue;
				if(mCollisionTester.PixelPerfectTest(mTankUnits[i]->GetSprite(), mTiles[ix][iy])){
					mTankUnits[i]->OnCollideWithWorld();
					if(i == TANK_UNIT_E1) mEnemy1Data.ChangeDirection = true;
					else if(i == TANK_UNIT_E2) mEnemy2Data.ChangeDirection = true;
					else if(i == TANK_UNIT_E3) mEnemy3Data.ChangeDirection = true;
				}
			}

			//check with bullets
			for(unsigned int i = 0; i < bullets.size(); ++i){
				if(bullets[i]->MustDie) continue;
				if(mCollisionTester.PixelPerfectTest(bullets[i]->GetSprite(), mTiles[ix][iy])){
					if(mTileMapMask[ix][iy] == TILE_WATER) continue;
					if(mTileMapMask[ix][iy] == TILE_EAGLE){
						mEagleDead = true;
						bullets[i]->MustDie = true;
						mTiles[ix][iy].SetSubRect(sf::IntRect(TILE_SIZE, 0, 2 * TILE_SIZE, TILE_SIZE));
						mTankUnits[TANK_UNIT_P]->OnCollideWithBullet();
						return;
					}
					if(mTileMapMask[ix][iy] == TILE_GLASS){
						int t = bullets[i]->GetType();
						if(t == Bullet::TYPE_COMET || t == Bullet::TYPE_NEON){
							sf::Vector2f v = bullets[i]->GetViewVector();
							sf::IntRect r = mTiles[ix][iy].GetSubRect();
							sf::Vector2f p = mTiles[ix][iy].GetPosition();

							if(static_cast<int>(v.x) == 1){
								r.Left += TILE_SIZE / 4;
								p.x += TILE_SIZE / 4;
							}else if(static_cast<int>(v.x) == -1){
								r.Right -= TILE_SIZE / 4;
							}else if(static_cast<int>(v.y) == -1){
								r.Top += TILE_SIZE / 4;
								p.y += TILE_SIZE / 4;
							}else if(static_cast<int>(v.y) == 1){
								r.Bottom -= TILE_SIZE / 4;
							}

							if(r.Left >= r.Right) mTileMapMask[ix][iy] = 0;
							else if(r.Top >= r.Bottom) mTileMapMask[ix][iy] = 0;
							else{
								mTiles[ix][iy].SetSubRect(r);
								mTiles[ix][iy].SetPosition(p);
							}
						}
					}else if(mTileMapMask[ix][iy] == TILE_GRASS){
						if(bullets[i]->GetType() == Bullet::TYPE_NEON){
							sf::Vector2f v = bullets[i]->GetViewVector();
							sf::IntRect r = mTiles[ix][iy].GetSubRect();
							sf::Vector2f p = mTiles[ix][iy].GetPosition();

							if(static_cast<int>(v.x) == 1){
								r.Left += TILE_SIZE / 4;
								p.x += TILE_SIZE / 4;
							}else if(static_cast<int>(v.x) == -1){
								r.Right -= TILE_SIZE / 4;
							}else if(static_cast<int>(v.y) == -1){
								r.Top += TILE_SIZE / 4;
								p.y += TILE_SIZE / 4;
							}else if(static_cast<int>(v.y) == 1){
								r.Bottom -= TILE_SIZE / 4;
							}

							if(r.Left >= r.Right) mTileMapMask[ix][iy] = 0;
							else if(r.Top >= r.Bottom) mTileMapMask[ix][iy] = 0;
							else{
								mTiles[ix][iy].SetSubRect(r);
								mTiles[ix][iy].SetPosition(p);
							}
						}
					}else if(mTileMapMask[ix][iy] == TILE_WALL){
						sf::Vector2f v = bullets[i]->GetViewVector();
						sf::IntRect r = mTiles[ix][iy].GetSubRect();
						sf::Vector2f p = mTiles[ix][iy].GetPosition();

						if(static_cast<int>(v.x) == 1){
							r.Left += TILE_SIZE / 4;
							p.x += TILE_SIZE / 4;
						}else if(static_cast<int>(v.x) == -1){
							r.Right -= TILE_SIZE / 4;
						}else if(static_cast<int>(v.y) == -1){
							r.Top += TILE_SIZE / 4;
							p.y += TILE_SIZE / 4;
						}else if(static_cast<int>(v.y) == 1){
							r.Bottom -= TILE_SIZE / 4;
						}

						if(r.Left >= r.Right) mTileMapMask[ix][iy] = 0;
						else if(r.Top >= r.Bottom) mTileMapMask[ix][iy] = 0;
						else{
							mTiles[ix][iy].SetSubRect(r);
							mTiles[ix][iy].SetPosition(p);
						}
					}

					if(bullets[i]->GetType() != Bullet::TYPE_NEON){
						bullets[i]->MustDie = true;
					}else{
						if(!(::rand() % 4)) bullets[i]->MustDie = true;
					}
				}
			}
		}
	}

	for(unsigned int i = 0; i < bullets.size(); ++i){
		if(bullets[i]->MustDie && !bullets[i]->Dead)
			mTankUnits[bullets[i]->GetShooter()]->GetTank()->EnableShoot();
	}
}

void GameWorld::CheckCollisionWithTanks(){
	for(int i = 0; i < TANK_UNIT_MAX; ++i){
		for(int j = 0; j < TANK_UNIT_MAX; ++j){
			if(i == j) continue;
			if(mTankUnits[i]->IsTankDead() || mTankUnits[j]->IsTankDead()) continue;

			if(mCollisionTester.PixelPerfectTest(mTankUnits[i]->GetSprite(), mTankUnits[j]->GetSprite())){
				/*if(i == TANK_UNIT_P || j == TANK_UNIT_P){
					mPlayerData.AddScore(PlayerData::SCORE_FOR_TANK_HIT);
				}*/

				mTankUnits[i]->OnCollideWithTank();
				mTankUnits[j]->OnCollideWithTank();
				if(i == TANK_UNIT_E1 || j == TANK_UNIT_E1) mEnemy1Data.ChangeDirection = true;
				if(i == TANK_UNIT_E2 || j == TANK_UNIT_E2) mEnemy2Data.ChangeDirection = true;
				if(i == TANK_UNIT_E3 || j == TANK_UNIT_E3) mEnemy3Data.ChangeDirection = true;
			}
		}
	}
}

void GameWorld::SetBonusToTank(int tank, int bonus){
	int s = 1;
	if(bonus == Bonus::TYPE_UPGRADE_WEAPON){
		mTankUnits[tank]->UpgradeWeapon();
		s = 2;
	}else if(bonus == Bonus::TYPE_1UP){
		if(tank == TANK_UNIT_P){
			mPlayerData.IncLive();
		}else{
			int e = (rand() % (Tank::TYPE_MAX - 1)) + 1;
			mEnemyData.AddEnemy(e);
		}
		s = 100;
	}else if(bonus == Bonus::TYPE_GRANADE){
		if(tank == TANK_UNIT_P){
			if(!mTankUnits[TANK_UNIT_E1]->IsDead()) mTankUnits[TANK_UNIT_E1]->Die();
			if(!mTankUnits[TANK_UNIT_E2]->IsDead()) mTankUnits[TANK_UNIT_E2]->Die();
			if(!mTankUnits[TANK_UNIT_E3]->IsDead()) mTankUnits[TANK_UNIT_E3]->Die();
			mPlayerData.AddScore(PlayerData::SCORE_FOR_TANK_HIT * 3);
		}else{
			if(!mTankUnits[TANK_UNIT_P]->IsDead()) mTankUnits[TANK_UNIT_P]->Die();
		}
		s = 10;
	}

	if(tank == TANK_UNIT_P){
		mPlayerData.AddScore(PlayerData::SCORE_FOR_BONUS * s);
	}
}

void GameWorld::UpdateEnemies(float dt){
	if(!mTankUnits[TANK_UNIT_E1]->IsTankDead()){
		mEnemy1Data.WantShootTime += dt;
		mEnemy1Data.WantChangeDirectionTime += dt;
		EnemyThink(TANK_UNIT_E1);
	}

	if(!mTankUnits[TANK_UNIT_E2]->IsTankDead()){
		mEnemy2Data.WantShootTime += dt;
		mEnemy2Data.WantChangeDirectionTime += dt;
		EnemyThink(TANK_UNIT_E2);
	}

	if(!mTankUnits[TANK_UNIT_E3]->IsTankDead()){
		mEnemy3Data.WantShootTime += dt;
		mEnemy3Data.WantChangeDirectionTime += dt;
		EnemyThink(TANK_UNIT_E3);
	}
}

void GameWorld::EnemyThink(int enemy){
        EnemyTankData* e = 0;
	sf::Vector2f goal = sf::Vector2f(0, 0);
	sf::Vector2f me = mTankUnits[enemy]->GetPosition();
	sf::Vector2f f1, f2;
	bool useGoal = false;

	if(enemy == TANK_UNIT_E1){
		e = &mEnemy1Data;
		//goal = mEaglePosition;
		//useGoal = true;
		f1 = mTankUnits[TANK_UNIT_E2]->GetPosition();
		f2 = mTankUnits[TANK_UNIT_E3]->GetPosition();
		//if(mEagleDead) useGoal = false;
	}else if(enemy == TANK_UNIT_E2){
		e = &mEnemy2Data;
		/*if(mBonusEmitter.HaveBonus()){
			goal = mBonusEmitter.GetBonus()->GetSprite().GetPosition();
			useGoal = true;
		}else{
			goal = mTankUnits[TANK_UNIT_P]->GetPosition();
			useGoal = true;
		}*/
		f1 = mTankUnits[TANK_UNIT_E1]->GetPosition();
		f2 = mTankUnits[TANK_UNIT_E3]->GetPosition();
	}else{
		e = &mEnemy3Data;
		/*if(mBonusEmitter.HaveBonus()){
			goal = mBonusEmitter.GetBonus()->GetSprite().GetPosition();
			useGoal = true;
		}else{
			goal = mEaglePosition;
			useGoal = true;
			if(mEagleDead) useGoal = false;
		}*/
		f1 = mTankUnits[TANK_UNIT_E2]->GetPosition();
		f2 = mTankUnits[TANK_UNIT_E1]->GetPosition();
	}

	//if(useGoal && ::rand() % 100 == 0) useGoal = false;

	bool canChange = true;
	if(e->ChangeDirection){
		//useGoal = false;
		e->CurrentDirection = TankAI::GetRandomDirection(e->CurrentDirection, me, mTileMapMask, f1, f2);
		e->ChangeDirection = false;
		canChange = false;
	}

	if(canChange){
		if(e->WantChangeDirectionTime >= 1.5f){
			if(::rand() % 12 == 0){				
				e->CurrentDirection = TankAI::GetRandomDirection(e->CurrentDirection, me, mTileMapMask, f1, f2);
			}
			e->WantChangeDirectionTime = 0.f;
		}
	}

	mTankUnits[enemy]->Move(e->CurrentDirection);

	if(e->WantShootTime >= 0.9f && (rand() % 10) == 0){
		e->WantShootTime = 0.f;
		mTankUnits[enemy]->Shoot(enemy);
	}else{
		//e->WantShootTime = 0.f;
	}
	/*if(TankAI::WantShoot(e->CurrentDirection, me, f1, f2, mEaglePosition, mTankUnits[TANK_UNIT_P]->GetPosition())){
		mTankUnits[enemy]->Shoot(enemy);
	}*/
}
