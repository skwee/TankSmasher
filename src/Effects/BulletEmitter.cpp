#include "Effects/BulletEmitter.hpp"
#include "Entities/Tank.hpp"
#include "Game.hpp"

void BulletEmitter::Emit(int type, const sf::Vector2f& pos, const sf::Vector2f& viewVector, int shooter){
	sf::Vector2f fp;

	fp.x = pos.x + (Tank::WIDHT / 2) * viewVector.x;
	fp.y = pos.y - (Tank::HEIGHT / 2) * viewVector.y;

	Bullet::ptr b = Bullet::Create(type, fp, viewVector, shooter);
	mBullets.push_back(b);
}

void BulletEmitter::Update(float dt){
	for(unsigned int i = 0; i < mBullets.size(); ++i){
		if(mBullets[i]->CanDie())
			mBullets.erase(mBullets.begin() + i);
		else{
			if(mBullets[i]->MustDie) mBullets[i]->Dead = true;
			mBullets[i]->Update(dt);
		}
	}
}

void BulletEmitter::Draw(sf::RenderWindow& wnd){
	for(unsigned int i = 0; i < mBullets.size(); ++i){
		if(mBullets[i]->MustDie) continue;
		mBullets[i]->Draw(wnd);
	}
}