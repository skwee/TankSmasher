#include "Entities/Tank.hpp"
#include "Game.hpp"

const float Tank::ANIMATION_TIME = 0.06f;
const float Tank::SHOOT_TIME = 0.2f;

Tank::Tank(){
	mSprite.SetImage(*GetGlobalContext().imageManager->Get("tanks.png"));
	mSprite.SetCenter(WIDHT / 2, HEIGHT / 2);
}

Tank::~Tank(){}

void Tank::Born(const TankData& data){
	if(data.type < 0 || data.type >= TYPE_MAX) throw TankSmasherException("Invalid tank type");
	mTankData = data;

	mDead = false;
	mCurrentAnimationSequence = 0;
	mAnimationTime = 0.f;
	mIsMoving = false;
	mShootTime = SHOOT_TIME;
	mCanShoot = true;
	//mOldPosition = mTankData.position;

	mSprite.SetPosition(sf::Vector2f(0, 0));
	mSprite.SetPosition(mTankData.position);
	mSprite.SetRotation(static_cast<float>(mTankData.viewAngle));
	UpdateSprite(0);
}

void Tank::Draw(sf::RenderWindow& wnd){
	wnd.Draw(mSprite);
}

void Tank::Move(MovingDirection dir){
	mIsMoving = true;
	mTankData.viewAngle = dir;
	mSprite.SetRotation(static_cast<float>(mTankData.viewAngle));
}

void Tank::Update(float dt){
	mShootTime += dt;
	if(mIsMoving){
		mOldPosition = mSprite.GetPosition();

		sf::Vector2f offset;
		offset.x = offset.y = 0;
		float dstep = dt * (gAnimationCorrector / 1.95) * mTankData.speed * MOVE_STEP;
		mAnimationTime += dt;
		int at = 0;

		if(mTankData.viewAngle == LEFT) { offset.x = -dstep; at = 1; }
		else if(mTankData.viewAngle == RIGHT) { offset.x = +dstep; at = 1; }
		else if(mTankData.viewAngle == TOP) { offset.y = -dstep; at = 1; }
		else if(mTankData.viewAngle == BOTTOM) { offset.y = +dstep; at = 1; }

		mSprite.Move(offset);
		UpdateSprite(at);
	}
}

void Tank::Shoot(int shooter){
	if(mDead) return;
	if(mCanShoot/* && mShootTime >= SHOOT_TIME*/){
	//if(mShootTime >= SHOOT_TIME){
		mShootTime = 0.f;
		mCanShoot = false;
		sf::Vector2f view;
		view.x = view.y = 0.f;
		if(mTankData.viewAngle == LEFT) view.x = -1.f;
		else if(mTankData.viewAngle == RIGHT) view.x = 1.f;
		else if(mTankData.viewAngle == TOP) view.y = 1.f;
		else if(mTankData.viewAngle == BOTTOM) view.y = -1.f;

		mTankData.bulletEmitter->Emit(mTankData.bulletType, mSprite.GetPosition(), view, shooter);
	}
	//}
}

void Tank::UpdateSprite(int at){
	if(mAnimationTime >= ANIMATION_TIME){
		mAnimationTime = 0.f;
		mCurrentAnimationSequence += at;
	}

	if(mCurrentAnimationSequence >= ANIMATION_MAX) mCurrentAnimationSequence = 0;
	if(mCurrentAnimationSequence < 0) mCurrentAnimationSequence = ANIMATION_MAX - 1;

	int left = mCurrentAnimationSequence * WIDHT;
	int top = mTankData.type * HEIGHT;
	mSprite.SetSubRect(sf::IntRect(left, top, left + WIDHT, top + HEIGHT));
}