#include "Entities/Bonus.hpp"
#include "TankSmasherException.hpp"
#include "Game.hpp"

Bonus::Bonus(int type, int px, int py){
	int left;

	mType = type;
	switch(type){
	case TYPE_UPGRADE_WEAPON:
		left = 0;
		break;
	case TYPE_1UP:
		left = WIDHT;
		break;
	case TYPE_GRANADE:
		left = 2 * WIDHT;
		break;
	default:
		throw TankSmasherException("Invalid bonus type!");
	}

	mSprite.SetImage(*GetGlobalContext().imageManager->Get("bonus.png"));
	mSprite.SetPosition(static_cast<float>(px), static_cast<float>(py));
	mSprite.SetCenter(WIDHT / 2, HEIGHT / 2);
	mSprite.SetSubRect(sf::IntRect(left, 0, left + WIDHT, HEIGHT));
}

int Bonus::GetRandomBonusType(){
	int r = ::rand() % 50;
	if(r >= 0 && r <= 30) return TYPE_NONE;
	else if(r > 30 && r <= 35) return TYPE_UPGRADE_WEAPON;
	else if(r > 35 && r <= 40) return TYPE_1UP;
	else if(r > 40 && r <= 50) return TYPE_GRANADE;
	return TYPE_NONE;
}