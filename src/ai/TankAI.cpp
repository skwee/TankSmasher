#include "ai/TankAI.hpp"

/*Tank::MovingDirection TankAI::GetRandomDirection(Tank::MovingDirection notThis, const sf::Vector2f& myLocation,
	const sf::Vector2f& goal, bool useGoal){
		Tank::MovingDirection d;
		if(!useGoal){
			int rd = ::rand() % 4;

			if(rd == 0){
				d = Tank::LEFT;
				if(d == notThis) d = Tank::RIGHT;
			}else if(rd == 1){
				d = Tank::RIGHT;
				if(d == notThis) d = Tank::BOTTOM;
			}else if(rd == 2){
				d = Tank::BOTTOM;
				if(d == notThis) d = Tank::TOP;
			}else if(rd == 3){
				d = Tank::TOP;
				if(d == notThis) d = Tank::LEFT;
			}
		}else{
			float dx = myLocation.x - goal.x;
			float dy = myLocation.y - goal.y;
			bool useSecond = false;
			bool useX = false;

			if(::fabs(dx) > ::fabs(dy)){
				useX = true;
				if(dx < 0.f) d = Tank::RIGHT;
				else d = Tank::LEFT;

				if(d == notThis) useSecond = true;
			}else{
				useX = false;
				if(dy < 0.f) d = Tank::BOTTOM;
				else d = Tank::TOP;

				if(d == notThis) useSecond = true;
			}

			if(useSecond){
				if(useX){
					if(dy < 0.f) d = Tank::BOTTOM;
					else d = Tank::TOP;
				}else{
					if(dx < 0.f) d = Tank::RIGHT;
					else d = Tank::LEFT;
				}
			}
		}

		return d;
}*/

Tank::MovingDirection TankAI::GetInitialDirection(){
	Tank::MovingDirection d;
	int rd = ::rand() % 4;
	if(rd == 0){
		d = Tank::LEFT;
	}else if(rd == 1){
		d = Tank::RIGHT;
	}else if(rd == 2){
		d = Tank::BOTTOM;
	}else if(rd == 3){
		d = Tank::TOP;
	}
	return d;
}

Tank::MovingDirection TankAI::GetRandomDirection(Tank::MovingDirection notThis,
	const sf::Vector2f& myLocation, int map[GameWorld::TILES_PER_X][GameWorld::TILES_PER_Y],
	const sf::Vector2f& f1Location, const sf::Vector2f& f2Location){
		Tank::MovingDirection d;

		int sx = static_cast<int>(myLocation.x / GameWorld::TILE_SIZE);
		int sy = static_cast<int>(myLocation.y / GameWorld::TILE_SIZE);
		if(sx < 0) sx = 0;
		if(sx >= GameWorld::TILES_PER_X) sx = GameWorld::TILES_PER_X - 1;
		if(sy < 0) sy = 0;
		if(sy >= GameWorld::TILES_PER_Y) sy = GameWorld::TILES_PER_Y - 1;
		int sdx, sdy;

		int s1x = static_cast<int>(f1Location.x / GameWorld::TILE_SIZE);
		int s1y = static_cast<int>(f1Location.y / GameWorld::TILE_SIZE);
		int s2x = static_cast<int>(f2Location.x / GameWorld::TILE_SIZE);
		int s2y = static_cast<int>(f2Location.y / GameWorld::TILE_SIZE);

		std::vector<Tank::MovingDirection> dirs;
		dirs.push_back(Tank::LEFT);
		dirs.push_back(Tank::RIGHT);
		dirs.push_back(Tank::BOTTOM);
		dirs.push_back(Tank::TOP);
		std::random_shuffle(dirs.begin(), dirs.end());

		while(!dirs.empty()){
			d = dirs.back();
			dirs.pop_back();

			switch(d){
			case Tank::LEFT:
				sdx = sx - 1;break;
			case Tank::RIGHT:
				sdx = sx + 1;break;
			case Tank::TOP:
				sdy = sy - 1;break;
			case Tank::BOTTOM:
				sdy = sy + 1;break;
			}

			if(sdx < 0 || sdy < 0 || sdx >= GameWorld::TILES_PER_X || sdy >= GameWorld::TILES_PER_Y) continue;
			if(map[sdx][sdy] != GameWorld::TILE_NONE) continue;
			if(((sdx == s1x) && (sdy == s1y)) || ((sdx == s2x) && (sdy == s2y))) continue;
			if((sdy == 0) && ((sdx == 0) || (sdx == (GameWorld::TILES_PER_X / 2) + 1) ||
				(sdx == GameWorld::TILES_PER_X)))
				continue;
			break;
		}

		return d;
}

Tank::MovingDirection TankAI::GetDirectionToGoal(int map[GameWorld::TILES_PER_X][GameWorld::TILES_PER_Y], const sf::Vector2f& me, const sf::Vector2f& goal){
	PathNode source, destination;
	source.x = static_cast<int>((me.x - GameWorld::RESERVED_AREA) / GameWorld::TILE_SIZE);
	source.y = static_cast<int>((me.y - GameWorld::RESERVED_AREA) / GameWorld::TILE_SIZE);
	source.score = 0;
	destination.x = static_cast<int>((goal.x - GameWorld::RESERVED_AREA) / GameWorld::TILE_SIZE);
	destination.y = static_cast<int>((goal.y - GameWorld::RESERVED_AREA) / GameWorld::TILE_SIZE);
	destination.score = 0;

	int path[GameWorld::TILES_PER_X][GameWorld::TILES_PER_Y];
	::memset(path, 0, sizeof(int) * GameWorld::TILES_PER_X * GameWorld::TILES_PER_Y);

	std::vector<PathNode> queue;
	queue.push_back(destination);

	while(1){
		PathNode n = queue.back();
		queue.pop_back();

		if(n.x == source.x && n.y == source.y) break;

		PathNode a;

		for(int i = -1; i <= 1; ++i){
			for(int j = -1; j <= 1; ++j){
				if(i == 0 && j == 0) continue;
				a.x = n.x + i;
				a.y = n.y + j;
				a.score = n.score + 1;

				if(a.x < 0 || a.y < 0 || a.x >= GameWorld::TILES_PER_X || a.y >= GameWorld::TILES_PER_Y)
					continue;

				if(a.x == destination.x && a.y == destination.y) continue;

				if(path[a.x][a.y] > 0){
					if(a.score > path[a.x][a.y]) path[a.x][a.y] = a.score;
					continue;
				}

				if(map[a.x][a.y] != GameWorld::TILE_NONE && map[a.x][a.y] != GameWorld::TILE_WALL && map[a.x][a.y] != GameWorld::TILE_GRASS)
					continue;

				path[a.x][a.y] = a.score;
				queue.push_back(a);
			}
		}

		if(queue.empty()) break;
	}

	PathNode max;
	max.score = 0;

	PathNode a;
	for(int i = -1; i <= 1; ++i){
		for(int j = -1; j <= 1; ++j){
			if(i == 0 && j == 0) continue;
			a.x = source.x + i;
			a.y = source.y + j;
			if(a.x < 0 || a.y < 0 || a.x >= GameWorld::TILES_PER_X || a.y >= GameWorld::TILES_PER_Y)
				continue;

			a.score = path[a.x][a.y];

			if(a.score > max.score) max = a;
		}
	}

	int idx = max.x - source.x;
	int idy = max.y - source.y;

	if(idx < 0) return Tank::LEFT;
	if(idx > 0) return Tank::RIGHT;
	if(idy < 0) return Tank::TOP;
	if(idy > 0) return Tank::BOTTOM;

	return GetInitialDirection();
}

bool TankAI::WantShoot(Tank::MovingDirection dir, const sf::Vector2f& me, const sf::Vector2f& f1, 
	const sf::Vector2f& f2, const sf::Vector2f& eagle, const sf::Vector2f& enemy){
		Ray ray(static_cast<int>(me.x), static_cast<int>(me.y), dir);
		int score = rand() % 20 - 10;

		while(!ray.IsDead()){
			/*if(ray.HitPoint(f1)) score -= 200;
			if(ray.HitPoint(f2)) score -= 200;*/
			if(ray.HitPoint(enemy)) score += 100;
			if(ray.HitPoint(eagle)) score += 200;
			ray.Move();
		}

		return (score > 0);
}