#include "Entities/TankExplosion.hpp"
#include "Game.hpp"

const float TankExplosion::ANIMATION_TIME = 0.08f;

TankExplosion::TankExplosion(){
	Reset();
	mSprite.SetImage(*GetGlobalContext().imageManager->Get("explosion.png"));
	mSprite.SetCenter(WIDTH / 2, HEIGHT / 2);
	UpdateSprite();
}

TankExplosion::~TankExplosion(){}

void TankExplosion::Draw(sf::RenderWindow& wnd){
	wnd.Draw(mSprite);
}

void TankExplosion::Reset(){
	mDone = mStarted = false;
	mCurrentAnimationSequence = 0;
	mAnimationTime = 0.f;
}

void TankExplosion::Spawn(const sf::Vector2f& position){
	Reset();
	mStarted = true;
	mSprite.SetPosition(position);
	UpdateSprite();
}

void TankExplosion::Update(float dt){
	mAnimationTime += dt;

	if(mAnimationTime >= ANIMATION_TIME){
		mAnimationTime = 0.f;
		mCurrentAnimationSequence ++;

		if(mCurrentAnimationSequence == ANIMATION_MAX)
			mDone = true;
		else
			UpdateSprite();
	}
}

void TankExplosion::UpdateSprite(){
	int left = mCurrentAnimationSequence * WIDTH;
	mSprite.SetSubRect(sf::IntRect(left, 0, left + WIDTH, HEIGHT));
}