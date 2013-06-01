#ifndef TS_BULLET_EMITTER_HPP_
#define TS_BULLET_EMITTER_HPP_

#include "Entities/Bullet.hpp"

class BulletEmitter{
public:
	typedef boost::shared_ptr<BulletEmitter> ptr;

public:
	static ptr Create(){
		return ptr(new BulletEmitter());
	}

	BulletEmitter(){
		mBullets.reserve(30);
	}

	~BulletEmitter(){
		mBullets.clear();
	}

	const std::vector<Bullet::ptr>& GetBullets() const { return mBullets; }

	void Emit(int type, const sf::Vector2f& pos, const sf::Vector2f& viewVector, int shooter);

	void Update(float dt);
	void Draw(sf::RenderWindow& wnd);

	void ClearAll(){
		mBullets.clear();
	}

private:
	std::vector<Bullet::ptr> mBullets;
};

#endif