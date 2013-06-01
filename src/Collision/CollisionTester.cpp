#include "Collision/CollisionTester.hpp"

#include "Common.hpp"

#include <cmath>

sf::IntRect CollisionTester::GetAABB(const sf::Sprite &object) const{
	sf::Vector2f pos = object.TransformToGlobal(sf::Vector2f(0, 0));
	sf::Vector2f size = object.GetSize();
	float angle = object.GetRotation();

	if(angle == 0.0f){
		return sf::IntRect(
			static_cast<int>(pos.x), static_cast<int>(pos.y),
			static_cast<int>(pos.x + size.x), static_cast<int>(pos.y + size.y)
			);
	}

	sf::Vector2f B(size.x, 0);
	sf::Vector2f C(size.x, size.y);
	sf::Vector2f D(0, size.y);

	B = RotatePoint(B, angle);
	C = RotatePoint(C, angle);
	D = RotatePoint(D, angle);

	int left = static_cast<int>(MinValue(0.0f, B.x, C.x, D.x));
	int top = static_cast<int>(MinValue(0.0f, B.y, C.y, D.y));
	int right = static_cast<int>(MaxValue(0.0f, B.x, C.x, D.x));
	int bottom = static_cast<int>(MaxValue(0.0f, B.y, C.y, D.y));

	sf::IntRect AABB = sf::IntRect(left, top, right, bottom);
	AABB.Offset(static_cast<int>(pos.x), static_cast<int>(pos.y));
	return AABB;
}

bool CollisionTester::PixelPerfectTest(const sf::Sprite &object1, const sf::Sprite &object2, sf::Uint8 alphaLimit) const{
	sf::IntRect object1AABB = GetAABB(object1);
	sf::IntRect object2AABB = GetAABB(object2);

	sf::IntRect intersection;

	if(object1AABB.Intersects(object2AABB, &intersection)){
		if(alphaLimit == 0) return true;

		sf::IntRect o1SubRect = object1.GetSubRect();
		sf::IntRect o2SubRect = object2.GetSubRect();

		sf::Vector2i o1SubRectSize(o1SubRect.GetWidth(), o1SubRect.GetHeight());
		sf::Vector2i o2SubRectSize(o2SubRect.GetWidth(), o2SubRect.GetHeight());

		sf::Vector2f o1v;
		sf::Vector2f o2v;

		for(int i = intersection.Left; i < intersection.Right; ++i){
			for(int j = intersection.Top; j < intersection.Bottom; ++j){
				o1v = object1.TransformToLocal(sf::Vector2f(static_cast<float>(i), static_cast<float>(j)));
				o2v = object2.TransformToLocal(sf::Vector2f(static_cast<float>(i), static_cast<float>(j)));

				if(o1v.x > 0 && o1v.y > 0 && o2v.x > 0 && o2v.y > 0 &&
					o1v.x < o1SubRectSize.x && o1v.y < o1SubRectSize.y &&
					o2v.x < o2SubRectSize.x && o2v.y < o2SubRectSize.y){
						if((object1.GetPixel(static_cast<int>(o1v.x), static_cast<int>(o1v.y)).a > alphaLimit) &&
							(object2.GetPixel(static_cast<int>(o2v.x), static_cast<int>(o2v.y)).a > alphaLimit)){
								return true;
						}
				}
			}
		}
		return false;
	}
	return false;
}

bool CollisionTester::CircleTest(const sf::Sprite &object1, const sf::Sprite &object2) const{
	float radius1 = (object1.GetSize().x + object1.GetSize().y) / 4;
	float radius2 = (object2.GetSize().x + object2.GetSize().y) / 4;
	float xd = object1.GetPosition().x - object2.GetPosition().x;
	float yd = object1.GetPosition().y - object2.GetPosition().y;

	return std::sqrt(xd * xd + yd * yd) <= radius1 + radius2;
}

bool CollisionTester::BoundingBoxTest(const sf::Sprite &object1, const sf::Sprite &object2) const{
	sf::Vector2f A, B, C, BL, TR;
	sf::Vector2f halfSize1 = object1.GetSize();
	sf::Vector2f halfSize2 = object2.GetSize();
	halfSize1.x /= 2;
	halfSize1.y /= 2;
	halfSize2.x /= 2;
	halfSize2.y /= 2;

	float angle = object1.GetRotation() - object2.GetRotation();
	float rad = angle * gDegreesToRadians;
	float cosa = std::cos(rad);
	float sina = std::sin(rad);

	float t, x, a, dx, ext1, ext2;

	C = object2.GetPosition();
	C -= object1.GetPosition();
	C = RotatePoint(C, object2.GetRotation());

	BL = TR = C;
	BL -= halfSize2;
	TR += halfSize2;

	A.x = -halfSize1.y * sina;
	B.x = A.x;
	t = halfSize1.x * cosa;
	A.x += t;
	B.x -= t;

	A.y = halfSize1.y * cosa;
	B.y = A.y;
	t = halfSize1.x * sina;
	A.y += t;
	B.y -= t;

	t = sina * cosa;

	if(t < 0){
		t = A.x;
		A.x = B.x;
		B.x = t;
		t = A.y;
		A.y = B.y;
		B.y = t;
	}

	if(sina < 0){
		B.x = -B.x;
		B.y = -B.y;
	}

	if(B.x > TR.x || B.x > -BL.x) return false;

	if(t == 0){
		ext1 = A.y;
		ext2 = -ext1;
	}else{
		x = BL.x - A.x;
		a = TR.x - A.x;
		ext1 = A.y;
		if(a * x > 0){
			dx = A.x;
			if(x < 0){
				dx -= B.x;
				ext1 -= B.y;
				x = a;
			}else{
				dx += B.x;
				ext1 += B.y;
			}
			ext1 *= x;
			ext1 /= dx;
			ext1 += A.y;
		}
		x = BL.x + A.x;
		a = TR.x + A.x;
		ext2 = -A.y;

		if(a * x > 0){
			dx = -A.x;
			if(x < 0){
				dx -= B.x;
				ext2 -= B.y;
				x = a;
			}else{
				dx += B.x;
				ext2 += B.y;
			}
			ext2 *= x;
			ext2 /= dx;
			ext2 -= A.y;
		}
	}

	return !((ext1 < BL.y && ext2 < BL.y) || (ext1 > TR.y && ext2 > TR.y));
}

float CollisionTester::MinValue(float a, float b, float c, float d) const{
	float min = a;
	min = (b < min ? b : min);
	min = (c < min ? c : min);
	min = (d < min ? d : min);
	return min;
}

float CollisionTester::MaxValue(float a, float b, float c, float d) const{
	float max = a;
	max = (b > max ? b : max);
	max = (c > max ? c : max);
	max = (d > max ? d : max);
	return max;
}

sf::Vector2f CollisionTester::RotatePoint(const sf::Vector2f &point, float angle) const{
	angle = angle * gDegreesToRadians;
	sf::Vector2f rotatedPoint;
	float c = cos(angle);
	float s = sin(angle);

	rotatedPoint.x = point.x * c + point.y * s;
	rotatedPoint.y = -point.x * s + point.y * c;
	return rotatedPoint;
}
