#ifndef TS_BONUS_EMITTER_HPP_
#define TS_BONUS_EMITTER_HPP_

#include "Entities/Bonus.hpp"

class BonusEmitter{
public:
	static const float BONUS_TIME;
	static const float FLASH_TIME;

	BonusEmitter();
	~BonusEmitter();

	bool HaveBonus() const { return !(mBonus == Bonus::ptr()); }

	Bonus::ptr GetBonus() const { return mBonus; }

	void Draw(sf::RenderWindow& wnd);

	void Update(float dt);

	void Reset();

private:
	void SpawnBonus();

	Bonus::ptr mBonus;
	float mBonusTime;
	float mFlashTime;
	bool mVisible;
};

#endif