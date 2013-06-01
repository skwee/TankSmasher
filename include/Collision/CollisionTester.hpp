#ifndef TS_COLLISION_TESTER_HPP_
#define TS_COLLISION_TESTER_HPP_

#include <SFML/Graphics.hpp>

class CollisionTester{
public:
	CollisionTester(){}
	~CollisionTester(){}

	sf::IntRect GetAABB(const sf::Sprite& object) const;
	bool PixelPerfectTest(const sf::Sprite& object1, const sf::Sprite& object2, sf::Uint8 alphaLimit = 127) const;
	bool CircleTest(const sf::Sprite& object1, const sf::Sprite& object2) const;
	bool BoundingBoxTest(const sf::Sprite& object1, const sf::Sprite& object2) const;

	bool PointInsideRectangle(int x, int y, int l, int r, int b, int t){
		return x >= l && x <= r && y >= b && y <= t;
	}

private:
	sf::Vector2f RotatePoint(const sf::Vector2f& point, float angle) const;
	float MinValue(float a, float b, float c, float d) const;
	float MaxValue(float a, float b, float c, float d) const;
};

#endif