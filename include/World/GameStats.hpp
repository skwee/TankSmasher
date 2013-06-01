#ifndef TS_GAME_STATS_HPP_
#define TS_GAME_STATS_HPP_

#include <SFML/Graphics.hpp>
#include "Entities/PlayerData.hpp"
#include "Entities/EnemyData.hpp"

class GameStats{
public:
	GameStats();
	~GameStats();

	void Draw(sf::RenderWindow& wnd, const PlayerData& pdata, const EnemyData& edata, int pwtype);

private:
	sf::String mScore[2];
	sf::String mLives;
	sf::Sprite mTank;
	sf::String mCurrentWeapon[2];
};

#endif