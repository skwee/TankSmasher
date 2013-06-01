#ifndef TS_PLAYER_INPUT_MAPPER_HPP_
#define TS_PLAYER_INPUT_MAPPER_HPP_

#include <SFML/Graphics.hpp>

class PlayerInputMapper{
public:
	typedef enum{
		MOVE_FORWARD, MOVE_BACKWARD, MOVE_LEFT, MOVE_RIGHT, SHOOT,
		ACTIONS_MAX
	}Action;

public:
	PlayerInputMapper(){
		::memset(mMap, 0, sizeof(sf::Key::Code) * ACTIONS_MAX);
	}

	inline void Set(sf::Key::Code code, Action action){
		mMap[action] = code;
	}

	inline sf::Key::Code Get(Action action) const{
		return mMap[action];
	}

private:
	sf::Key::Code mMap[ACTIONS_MAX];
};

#endif