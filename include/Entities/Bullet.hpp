#ifndef TS_BULLET_HPP_
#define TS_BULLET_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <boost/shared_ptr.hpp>

class Bullet{
public:
	typedef boost::shared_ptr<Bullet> ptr;

	enum{
		MOVE_SPEED = 10
	};

	enum{
		TYPE_SIMPLE = 0, TYPE_COMET, TYPE_NEON, TYPE_MAX
	};

public:
	static ptr Create(int type, const sf::Vector2f& pos, const sf::Vector2f& viewVector, int shooter){
		return ptr(new Bullet(type, pos, viewVector, shooter));
	}

	Bullet(int type, const sf::Vector2f& pos, const sf::Vector2f& viewVector, int shooter);
	~Bullet();

	void Draw(sf::RenderWindow& wnd);
	void Update(float dt);

	const sf::Sprite& GetSprite() const { return mSprite; }

	void Get(int& w, int& h) { w = mHalfWidth; h = mHalfHeight; }

	int GetShooter() const { return mShooter; }

	static void GetBulletData(int type, std::string& file, std::string& name, int& hw, int& hh, float& speed);

	sf::Vector2f GetViewVector() const { return mViewVector; }
	int GetType() const { return mType; }

	bool MustDie;
	bool Dead;
	bool CanDie() { return MustDie && mCanDie; }

private:
	sf::Sprite mSprite;
	sf::Vector2f mViewVector;
	
	sf::Sound mShootSound;

	int mHalfWidth;
	int mHalfHeight;

	int mType;
	float mSpeed;

	int mShooter;

	bool mCanDie;
};

#endif