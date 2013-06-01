#ifndef TS_TANK_HPP_
#define TS_TANK_HPP_

#include <SFML/Graphics.hpp>
#include <boost/shared_ptr.hpp>

#include "Effects/BulletEmitter.hpp"

typedef struct{
	sf::Vector2f position;
	int type;
	float speed;
	int viewAngle;
	int bulletType;
	BulletEmitter::ptr bulletEmitter;
}TankData;

class Tank{
public:
	typedef boost::shared_ptr<Tank> ptr;

	enum{
		WIDHT = 32,
		HEIGHT = 32,
		ANIMATION_MAX = 8,
		MOVE_STEP = 8
	};

	enum{
		TYPE_GREEN = 0,
		TYPE_RED,
		TYPE_YELLOW,
		TYPE_PURPLE,
		TYPE_BLUE,
		TYPE_CYAN,
		TYPE_ORANGE,
		TYPE_PINK,
		TYPE_MAX
	};

	typedef enum{
		NONE = -1, LEFT = 180, RIGHT = 0, TOP = 90, BOTTOM = 270
	}MovingDirection;

	static const float ANIMATION_TIME;
	static const float SHOOT_TIME;

public:
	static ptr Create(){
		return ptr(new Tank());
	}

	Tank();
	~Tank();

	void Draw(sf::RenderWindow& wnd);
	void Update(float dt);

	void Born(const TankData& data);
	void Die() { mDead = true; }
	bool IsDead() const { return mDead; }

	sf::Vector2f GetPosition() const { return mSprite.GetPosition(); }
	const sf::Sprite& GetSprite() const { return mSprite; }

	void OnCollideWithWorld() { mSprite.SetPosition(mOldPosition); }

	void Move(MovingDirection dir);
	void StopMoving() { mIsMoving = false; }

	void Shoot(int shooter);

	void UpgradeWeapon(int w) { mTankData.bulletType = w; }

	const sf::Vector2f& GetPreviousPosition() const { return mOldPosition; }

	void EnableShoot() { mCanShoot = true; }

private:
	void UpdateSprite(int at);

	sf::Sprite mSprite;
	sf::Vector2f mOldPosition;

	TankData mTankData;
	bool mDead;

	int mCurrentAnimationSequence;
	float mAnimationTime;

	float mShootTime;

	bool mIsMoving;
	bool mCanShoot;
};

#endif