#include "Entities/TankSpawn.hpp"
#include "Game.hpp"

const float TankSpawn::ANIMATION_TIME = 0.07f;

TankSpawn::TankSpawn(){
	mDone = mStarted = false;

	mSprite.SetImage(*GetGlobalContext().imageManager->Get("thunder_effect.png"));
	mSprite.SetCenter(WIDTH / 2, HEIGHT / 2);
}

TankSpawn::~TankSpawn(){}

void TankSpawn::Draw(sf::RenderWindow& wnd){
	wnd.Draw(mSprite);
}

void TankSpawn::Spawn(const sf::Vector2f& position){
	mDone = false;
	mStarted = true;
	mCurrentAnimationSequence = 0;
	mAnimationTime = 0.f;
	mSprite.SetPosition(position);
	UpdateSprite();
}

void TankSpawn::Update(float dt){
	mAnimationTime += dt;

	if(mAnimationTime >= ANIMATION_TIME){
		mAnimationTime = 0.f;
		mCurrentAnimationSequence++;

		if(mCurrentAnimationSequence == ANIMATION_MAX){
			mDone = true;
			mStarted = false;
		}else{
			UpdateSprite();
		}
	}
}

void TankSpawn::UpdateSprite(){
	int left = mCurrentAnimationSequence * WIDTH;
	mSprite.SetSubRect(sf::IntRect(left, 0, left + WIDTH, HEIGHT));
}