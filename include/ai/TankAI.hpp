#ifndef TS_PATH_FINDING_HPP_
#define TS_PATH_FINDING_HPP_

#include "World/GameWorld.hpp"

class TankAI{
public:
	typedef struct{
		int x, y;
		int score;
	}PathNode;

public:

	/*static Tank::MovingDirection GetRandomDirection(Tank::MovingDirection notThis,
		const sf::Vector2f& myLocation = sf::Vector2f(0,0),
		const sf::Vector2f& goal = sf::Vector2f(0,0), bool useGoal = false);*/

	static Tank::MovingDirection GetRandomDirection(Tank::MovingDirection notThis,
		const sf::Vector2f& myLocation, int map[GameWorld::TILES_PER_X][GameWorld::TILES_PER_Y],
		const sf::Vector2f& f1Location, const sf::Vector2f& f2Location);

	static Tank::MovingDirection GetInitialDirection();

	static Tank::MovingDirection GetDirectionToGoal(int map[GameWorld::TILES_PER_X][GameWorld::TILES_PER_Y],
		const sf::Vector2f& me, const sf::Vector2f& goal);

	static bool WantShoot(Tank::MovingDirection dir,
		const sf::Vector2f& me, const sf::Vector2f& f1, const sf::Vector2f& f2,
		const sf::Vector2f& eagle, const sf::Vector2f& enemy);

};

typedef struct Ray_s{
private:
	int x, y, dx, dy;

public:
	Ray_s(int ix, int iy, Tank::MovingDirection d){
		x = static_cast<int>(ix / GameWorld::TILE_SIZE);
		y = static_cast<int>(iy / GameWorld::TILE_SIZE);
		dx = dy = 0;
		if(d == Tank::LEFT) dx = -1;
		else if(d == Tank::RIGHT) dx = +1;
		else if(d == Tank::TOP) dy = -1;
		else if(d == Tank::BOTTOM) dy = +1;
	}

	bool IsDead(){
		return (x < 0 || y < 0 || x >= GameWorld::TILES_PER_X || y >= GameWorld::TILES_PER_Y);
	}

	void Move(){
		x += dx;
		y += dy;
	}

	bool HitPoint(const sf::Vector2f& p){
		int px = static_cast<int>(p.x / GameWorld::TILE_SIZE);
		int py = static_cast<int>(p.y / GameWorld::TILE_SIZE);

		return (x == px && y == py);
	}
}Ray;

#endif