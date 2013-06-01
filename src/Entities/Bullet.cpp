#include "Entities/Bullet.hpp"
#include "Game.hpp"
#include "World/GameWorld.hpp"

void Bullet::GetBulletData(int type, std::string& file, std::string& name, int& hw, int& hh, float& speed){
	switch(type){
	case TYPE_SIMPLE:
		file = "blue-round.png";
		name = "nap bullet";
		hw = 6;
		hh = 6;
		speed = 1.5f;
		break;
	case TYPE_COMET:
		file = "purple-comet.png";
		name = "Comet";
		hw = 16;
		hh = 8;
		speed = 2.0f;
		break;
	case TYPE_NEON:
		file = "neon.png";
		name = "Neon Laser";
		hw = 23;
		hh = 8;
		speed = 3.0f;
		break;
	default:
		throw TankSmasherException("Invalid bullet type");
	}
}

Bullet::Bullet(int type, const sf::Vector2f& pos, const sf::Vector2f& viewVector, int shooter){
	mType = type;
	mViewVector = viewVector;
	mShooter = shooter;
	Dead = MustDie = mCanDie = false;

	std::string name, file;
	Bullet::GetBulletData(type, file, name, mHalfWidth, mHalfHeight, mSpeed);

	mSprite.SetImage(*GetGlobalContext().imageManager->Get(file));
	mSprite.SetCenter(static_cast<float>(mHalfWidth), static_cast<float>(mHalfHeight));
	mSprite.SetPosition(pos);

	float angle = 0.f;
	if(mViewVector.x > 0.f) angle = 0.f;
	else if(mViewVector.x < 0.f) angle = 180.0f;
	else if(mViewVector.y > 0.f) angle = 90.f;
	else if(mViewVector.y < 0.f) angle = 270.f;
	mSprite.SetRotation(angle);

	mShootSound.SetVolume(50.f);
	mShootSound.SetLoop(false);
	mShootSound.SetBuffer(*GetGlobalContext().soundManager->Get("blast.wav"));
	if(Game::gGameSettings.SoundOn && (mShooter == GameWorld::TANK_UNIT_P)) mShootSound.Play();
}

Bullet::~Bullet(){}

void Bullet::Draw(sf::RenderWindow& wnd){
	if(MustDie) return;
	wnd.Draw(mSprite);
}

void Bullet::Update(float dt){
	if(MustDie){
		if(mShootSound.GetStatus() == sf::Sound::Stopped) mCanDie = true;
		return;
	}
	float dstep = gAnimationCorrector * MOVE_SPEED * dt * mSpeed;
	sf::Vector2f offset;

	offset.x = dstep * mViewVector.x;
	offset.y = -dstep * mViewVector.y;
	mSprite.Move(offset);
}