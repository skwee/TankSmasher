#include "Entities/TankUnit.hpp"

TankUnit::TankUnit(const TankData& data){
	mTankData = data;
	mTank = Tank::Create();
	mExplosion = TankExplosion::Create();
	mSpawn = TankSpawn::Create();
}

TankUnit::~TankUnit(){}

void TankUnit::Respawn(bool nullize){
	if(nullize){
		mTankData.bulletType = Bullet::TYPE_SIMPLE;
		mTankData.speed = 1.f;
	}
	mSpawn->Spawn(mTankData.position);
	mTank->Born(mTankData);
	mExplosion->Reset();
	mDead = false;
}

void TankUnit::Draw(sf::RenderWindow& wnd){
	if(mSpawn->WasStarted() && !mSpawn->IsDone()){
		mSpawn->Draw(wnd);
	}else{
		if(!mTank->IsDead()){
			mTank->Draw(wnd);
		}else{
			if(!mDead) mExplosion->Draw(wnd);
		}
	}
}

void TankUnit::Update(float dt){
	if(mDead) return;
	if(mSpawn->WasStarted() && !mSpawn->IsDone()){
		mSpawn->Update(dt);
	}else{
		if(mTank->IsDead()){
			if(mExplosion->WasStarted()){
				if(mExplosion->IsDone()) mDead = true;
				else mExplosion->Update(dt);
			}else{
				mExplosion->Spawn(mTank->GetPosition());
			}
		}else{
			mTank->Update(dt);
		}
	}
}

void TankUnit::UpgradeWeapon(){
	switch(mTankData.bulletType){
	case Bullet::TYPE_SIMPLE:
		mTankData.bulletType = Bullet::TYPE_COMET;
		mTank->UpgradeWeapon(Bullet::TYPE_COMET);
		break;
	case Bullet::TYPE_COMET:
		mTankData.bulletType = Bullet::TYPE_NEON;
		mTank->UpgradeWeapon(Bullet::TYPE_NEON);
		break;
	}
}