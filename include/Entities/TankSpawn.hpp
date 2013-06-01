#ifndef TS_TANK_SPAWN_HPP_
#define TS_TANK_SPAWN_HPP_

#include <SFML/Graphics.hpp>
#include <boost/shared_ptr.hpp>

class TankSpawn{
public:
	typedef boost::shared_ptr<TankSpawn> ptr;

	enum{
		WIDTH = 32,
		HEIGHT = 32,
		ANIMATION_MAX = 8
	};

	static const float ANIMATION_TIME;

public:
	static ptr Create(){
		return ptr(new TankSpawn());
	}

	TankSpawn();
	~TankSpawn();

	void Draw(sf::RenderWindow& wnd);
	void Update(float dt);

	void Spawn(const sf::Vector2f& position);

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