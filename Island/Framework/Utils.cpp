#include "Utils.h"
#include "../3rd/OBB.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "../GameObject/HitBox.h"
random_device Utils::rd;
mt19937 Utils::gen(Utils::rd());

void Utils::SetOrigin(Text& obj, Origins origin)
{
	SetOrigin(obj, origin, obj.getLocalBounds());
}

void Utils::SetOrigin(Sprite& obj, Origins origin)
{
	SetOrigin(obj, origin, obj.getLocalBounds());
}

void Utils::SetOrigin(Shape& obj, Origins origin)
{
	SetOrigin(obj, origin, obj.getLocalBounds());
}

void Utils::SetOrigin(Transformable& obj, Origins origin, FloatRect rect)
{
	Vector2f originPos(rect.width, rect.height);
	originPos.x *= ((int)origin % 3) * 0.5f;
	originPos.y *= ((int)origin / 3) * 0.5f;
	obj.setOrigin(originPos);
}

int Utils::RandomRange(int min, int maxExclude)
{
	return (gen() % (maxExclude - min)) + min;
}

float Utils::RandomRange(float min, float max)
{
	return min + static_cast<float>(rand()) * static_cast<float>(max - min) / RAND_MAX;;
}
float Utils::RandomRange() 
{
	return RandomRange(0.f, 1.f);
 }

Vector2f Utils::RandAreaPoint()
{
	return RandomRange() * RandContact(1.f);
}

Vector2f Utils::RandContact(float rad)
{
	float radian = RandomRange(0.f, M_PI * 2);
	return Vector2f({ cos(radian),sin(radian) });
}

float Utils::SqrMagnitude(const Vector2f& vec)
{
	return vec.x * vec.x + vec.y * vec.y;
}

float Utils::Magnitude(const Vector2f& vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

float Utils::Dot(const Vector2f& a, const Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

Vector2f Utils::GetNormal(const Vector2f& vec)
{
	return Vector2f(-vec.y, vec.x);
}

Vector2f Utils::Normalize(const Vector2f& vec)
{
	float mag = Magnitude(vec);
	if (mag == 0.f)
		return vec;
	return vec / mag;
}
float Utils::Distance(const Vector2f& vec1, const Vector2f& vec2)
{
	return Magnitude(vec1 - vec2);
}

float Utils::Angle(const Vector2f& start, const Vector2f& end)
{

	return Angle(Normalize(end - start));
}

float Utils::Angle(const Vector2f& dir)
{
	return atan2(dir.y, dir.x) * (180.f / M_PI);
}

float Utils::Clamp(float value, float min, float max)
{
	return (value < min) ? min : ((value > max) ? max : value);
}

bool Utils::IsRange(float row, float height, float value)
{
	return value >= row && value <= height;
}

bool Utils::IsRange(const FloatRect rect, const Vector2f value)
{
	return IsRange(rect.left, rect.left + rect.width, value.x) 
		&& IsRange(rect.top, rect.top + rect.height, value.y);
}

bool Utils::OBB(const RectangleShape& obb1, const RectangleShape& obb2)
{
	Vector2f MTV;
	return testCollision(obb1, obb2, MTV);
}

bool Utils::OBB(const vector<HitBox*> obb1, const HitBox* obb2)
{
	Vector2f MTV;
	bool isHit = false;
	for (auto& hit : obb1)
	{
		if (OBB(hit->GetHitbox(), obb2->GetHitbox()))
		{
			return true;
		}
	}
	return false;
}
bool Utils::OBB( FloatRect& obb1,  HitBox& obb2)
{
	Vector2f MTV;
	bool isHit = false;
	RectangleShape rect;
	rect.setPosition(obb1.left, obb1.top);
	rect.setSize({ obb1.width, obb1.height });
	return testCollision(rect, obb2.GetHitbox(), MTV);
	return false;
}

bool Utils::LineRect(Vector2f startPos, Vector2f nextPos, RectangleShape hitObject)
{
	// check if the line has hit any of the rectangle's sides
	// uses the Line/Line function below
	auto bounds = hitObject.getGlobalBounds();
	float rx = bounds.left;
	float ry = bounds.top;
	float rw = bounds.width;
	float rh = bounds.height;
	bool left = Lineline(startPos, nextPos, rx, ry, rx, ry + rh);
	bool right = Lineline(startPos, nextPos, rx + rw, ry, rx + rw, ry + rh);
	bool top = Lineline(startPos, nextPos, rx, ry, rx + rw, ry);
	bool bottom = Lineline(startPos, nextPos, rx, ry + rh, rx + rw, ry + rh);

	// if ANY of the above are true, the line
	// has hit the rectangle
	if (left || right || top || bottom) {
		return true;
	}
	return false;
}

bool Utils::Lineline(Vector2f startPos, Vector2f nextPos, float x3, float y3, float x4, float y4)
{
	// calculate the direction of the lines
	float uA = ((x4 - x3) * (startPos.y - y3) - (y4 - y3) * (startPos.x - x3)) /
		((y4 - y3) * (nextPos.x - startPos.x) - (x4 - x3) * (nextPos.y - startPos.y));
	float uB = ((nextPos.x - startPos.x) * (startPos.y - y3) - (nextPos.y - startPos.y) * (startPos.x - x3)) /
		((y4 - y3) * (nextPos.x - startPos.x) - (x4 - x3) * (nextPos.y - startPos.y));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
	{
		return true;
	}
	return false;
}


