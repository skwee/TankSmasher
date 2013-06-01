#ifndef TS_TANK_UNIT_HPP_
#define TS_TANK_UNIT_HPP_

#include "Entities/Tank.hpp"
#include "Entities/TankExplosion.hpp"
#include "Entities/TankSpawn.hpp"

class TankUnit{
public:
	typedef boost::shared_ptr<TankUnit> ptr;

public:
	static ptr Create(const TankData& data){
		return ptr(new TankUnit(data));
	}

	TankUnit(const TankData& data);
	~TankUnit();

	void Respawn(bool nullize = false);

	void Move(Tank::MovingDirection dir){
		if(mSpawn->IsDone() && !mTank->IsDead()) mTank->Move(dir);
	}

	void StopMoving(){
		if(mSpawn->IsDone() && !mTank->IsDead()) mTank->StopMoving();
	}

	void Draw(sf::RenderWindow& wnd);
	void Update(float dt);

	bool IsDead() const { return mDead; }
	bool IsTankDead() const { return mTank->IsDead(); }

	Tank::ptr GetTank() const { return mTank; }
	TankExplosion::ptr GetExplosion() const { return mExplosion; }
	TankSpawn::ptr GetSpawn() const { return mSpawn; }

	const sf::Sprite& GetSprite() const { return mTank->GetSprite(); }
	sf::Vector2f GetPosition() const { return mTank->GetPosition(); }

	void OnCollideWithTank(){
		mTank->OnCollideWithWorld();
	}

	void OnCollideWithBullet(){
		mTank->Die();
	}

	void OnCollideWithWorld(){
		mTank->OnCollideWithWorld();
	}

	void Shoot(int shooter){
		if(mSpawn->IsDone() && !mExplosion->WasStarted() && !mTank->IsDead())mTank->Shoot(shooter);
	}

	TankData& GetTankData() { return mTankData; }

	void UpgradeWeapon();

	void Die(){
		mTank->Die();
	}

	const sf::Vector2f& GetPreviousPosition() const { return mTank->GetPreviousPosition(); }

private:
	Tank::ptr mTank;
	TankExplosion::ptr mExplosion;
	TankSpawn::ptr mSpawn;
	TankData mTankData;

	bool mDead;
};

#endif