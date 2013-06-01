#include "Effects/BonusEmitter.hpp"
#include "World/GameWorld.hpp"

const float BonusEmitter::BONUS_TIME = 10.f;
const float BonusEmitter::FLASH_TIME = 0.2f;

BonusEmitter::BonusEmitter(){
	Reset();
}

BonusEmitter::~BonusEmitter(){}

void BonusEmitter::Draw(sf::RenderWindow& wnd){
	if(HaveBonus() && mVisible) mBonus->Draw(wnd);
}

void BonusEmitter::Update(float dt){
	if(HaveBonus()){
		mBonusTime += dt;
		mFlashTime += dt;
		if(mBonusTime >= (BONUS_TIME * 1.5)){
			mBonus = Bonus::ptr();
			mBonusTime = 0.f;
		}else{
			if(mFlashTime >= FLASH_TIME){
				mFlashTime = 0.f;
				mVisible = !mVisible;
			}
		}
	}else{
		mBonusTime += dt;
		float f = static_cast<float>(::rand() % 10);
		if(mBonusTime >= (BONUS_TIME + f)){
			mBonusTime = 0.f;
			mVisible = true;
			SpawnBonus();
		}
	}
}

void BonusEmitter::Reset(){
	mBonus = Bonus::ptr();
	mBonusTime = mFlashTime = 0.f;
	mVisible = false;
}

void BonusEmitter::SpawnBonus(){
	int px = ::rand() % (GameWorld::WORLD_WIDTH - Bonus::WIDHT - GameWorld::RESERVED_AREA);
	int py = ::rand() % (GameWorld::WORLD_HEIGHT - Bonus::HEIGHT - GameWorld::RESERVED_AREA);
	px += GameWorld::RESERVED_AREA + (Bonus::WIDHT / 2);
	py += GameWorld::RESERVED_AREA + (Bonus::HEIGHT / 2);

	int type = Bonus::GetRandomBonusType();
	if(type == Bonus::TYPE_NONE) return;

	mBonus = Bonus::Create(type, px, py);
}