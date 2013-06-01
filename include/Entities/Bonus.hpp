#ifndef TS_BONUS_HPP_
#define TS_BONUS_HPP_

#include <SFML/Graphics.hpp>
#include <boost/shared_ptr.hpp>

class Bonus{
public:
	enum{
		TYPE_NONE = 0,
		TYPE_UPGRADE_WEAPON,
		TYPE_1UP, TYPE_GRANADE,
		TYPE_MAX
	};

	enum{
		WIDHT = 24, HEIGHT = 24
	};

	typedef boost::shared_ptr<Bonus> ptr;

public:
	static ptr Create(int type, int px, int py){
		return ptr(new Bonus(type, px, py));
	}

	static int GetRandomBonusType();

	Bonus(int type, int px, int py);
	~Bonus(){}

	void Draw(sf::RenderWindow& wnd){
		wnd.Draw(mSprite);
	}

	const sf::Sprite& GetSprite() const { return mSprite; }
	int GetType() const { return mType; }

private:
	sf::Sprite mSprite;
	int mType;
};

#endif