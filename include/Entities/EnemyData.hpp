#ifndef TS_ENEMY_DATA_HPP_
#define TS_ENEMY_DATA_HPP_

#include "TankSmasherException.hpp"
#include "Entities/Tank.hpp"

#include <iostream>

typedef struct{
	bool ChangeDirection;
	Tank::MovingDirection CurrentDirection;
	float WantShootTime;
	float WantChangeDirectionTime;
}EnemyTankData;

class EnemyData{
public:
	enum{
		MAX_ENEMIES = 20
	};

public:
	EnemyData(){
		mEnemies = 0;
		::memset(mEnemiesSequence, 0, sizeof(int) * MAX_ENEMIES);
	}
	~EnemyData(){}

	inline void AddEnemy(int type){
		if(type < Tank::TYPE_GREEN || type >= Tank::TYPE_MAX) throw TankSmasherException("Invalid tank type");
		if(mEnemies + 1 >= MAX_ENEMIES) return;
		mEnemies++;
		mEnemiesSequence[mEnemies] = type;
	}

	inline int GetEnemy(){
		int t = mEnemiesSequence[mEnemies];
		mEnemies--;
		if(mEnemies < 0) mEnemies = 0;
		return t;
	}

	inline short GetLeft() const { return mEnemies; }

	inline bool IsDead() { return mEnemies == 0; }

private:
	int mEnemiesSequence[MAX_ENEMIES];
	short mEnemies;
};

#endif