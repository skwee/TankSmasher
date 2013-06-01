#ifndef TS_TANK_EXPLOSION_HPP_
#define TS_TANK_EXPLOSION_HPP_

#include <SFML/Graphics.hpp>
#include <boost/shared_ptr.hpp>

class TankExplosion{
public:
	typedef boost::shared_ptr<TankExplosion> ptr;

	enum{
		WIDTH = 32,
		HEIGHT = 32,
		ANIMATION_MAX = 16
	};

	static const float ANIMATION_TIME;

public:
	static ptr Create(){
		return ptr(new TankExplosion());
	}

	TankExplosion();
	~TankExplosion();

	void Draw(sf::RenderWindow& wnd);

	void Update(float dt);
	void Spawn(const sf::Vector2f& position);

	void Reset();

	bool IsDone() const { return mDone; }
	bool WasStarted() const { return mStarted; }

private:
	void UpdateSprite();

	sf::Sprite mSprite;

	bool mDone;
	bool mStarted;

	int mCurrentAnimationSequence;
	float mAnimationTime;
};

#endif