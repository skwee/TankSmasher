#include "World/GameStats.hpp"
#include "Game.hpp"
#include "Entities/Tank.hpp"

#include <iomanip>

GameStats::GameStats(){
	mTank.SetImage(*GetGlobalContext().imageManager->Get("tanks.png"));
	mTank.SetCenter(0, 0);
	mTank.Rotate(90);
	
	sf::Color c(255, 255, 255);
	FontManager::FontPtr f = GetGlobalContext().fontManager->Get("digital-7.ttf;52");

	mScore[0].SetColor(c);
	mScore[0].SetFont(*f);
	mScore[0].SetSize(20);
	mScore[0].SetPosition(460, 30);
	mScore[0].SetText("score:");

	mScore[1].SetColor(c);
	mScore[1].SetFont(*f);
	mScore[1].SetSize(30);
	mScore[1].SetPosition(460, 50);

	mLives.SetColor(c);
	mLives.SetFont(*f);
	mLives.SetSize(30);

	mCurrentWeapon[0].SetColor(c);
	mCurrentWeapon[0].SetFont(*f);
	mCurrentWeapon[0].SetSize(22);
	mCurrentWeapon[0].SetPosition(460, 300);
	mCurrentWeapon[0].SetText("Weapon:");

	mCurrentWeapon[1].SetColor(c);
	mCurrentWeapon[1].SetFont(*f);
	mCurrentWeapon[1].SetSize(22);
	mCurrentWeapon[1].SetPosition(460, 320);
}

GameStats::~GameStats(){}

void GameStats::Draw(sf::RenderWindow& wnd, const PlayerData& pdata, const EnemyData& edata, int pwtype){
	std::ostringstream ss;

	ss << std::setw(2) << std::setfill('0') << pdata.GetLives();

	mTank.SetPosition(460, 150);
	mTank.SetSubRect(sf::IntRect(0, 0, Tank::WIDHT, Tank::HEIGHT));
	mLives.SetText(ss.str());
	mLives.SetPosition(500, 114);
	wnd.Draw(mTank);
	wnd.Draw(mLives);

	ss.clear();
	ss.str("");
	ss << std::setw(2) << std::setfill('0') << edata.GetLeft();

	mTank.SetPosition(460, 250);
	mTank.SetSubRect(sf::IntRect(0, Tank::TYPE_RED * Tank::HEIGHT, 
		Tank::WIDHT, Tank::TYPE_RED * Tank::HEIGHT + Tank::HEIGHT));
	mLives.SetText(ss.str());
	mLives.SetPosition(500, 214);
	wnd.Draw(mTank);
	wnd.Draw(mLives);

	ss.clear();
	ss.str("");
	ss << std::setw(5) << std::setfill('0') << pdata.GetScore();
	mScore[1].SetText(ss.str());

	wnd.Draw(mScore[0]);
	wnd.Draw(mScore[1]);

	std::string file, name;
	int hw, hh;
	float speed;
	Bullet::GetBulletData(pwtype, file, name, hw, hh, speed);
	
	mCurrentWeapon[1].SetText(name);
	wnd.Draw(mCurrentWeapon[0]);
	wnd.Draw(mCurrentWeapon[1]);
}