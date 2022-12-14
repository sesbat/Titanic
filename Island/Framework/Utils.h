#pragma once
#include <random>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class HitBox;
enum class Origins
{
	TL,	// 0
	TC,
	TR,
	ML,
	MC,
	MR,
	BL,
	BC,
	BR,	// 8
};

class Utils
{
private:
	static random_device rd;
	static mt19937 gen;

public:
	static void SetOrigin(Text& obj, Origins origin);
	static void SetOrigin(Sprite& obj, Origins origin);
	static void SetOrigin(Shape& obj, Origins origin);
	static void SetOrigin(Transformable& obj, Origins origin, FloatRect rect);
	
	static int RandomRange(int min, int maxExclude);
	static float RandomRange(float min, float max);
	static float RandomRange();
	static Vector2f RandAreaPoint();
	static Vector2f RandAreaPoint(float rad);
	static Vector2f RandContact(float rad);

	static float SqrMagnitude(const Vector2f& vec);
	static float Magnitude(const Vector2f& vec);
	static float Dot(const Vector2f& a, const Vector2f& b);
	static Vector2f GetNormal(const Vector2f& vec);

	static Vector2f Normalize(const Vector2f& vec);
	static float Distance(const Vector2f& vec1, const Vector2f& vec2);
	static float Angle(const Vector2f& start, const Vector2f& end);
	static float Angle(const Vector2f& dir);

	static float Clamp(float value, float min, float max);
	static bool IsRange(float row, float height, float value);
	static bool IsRange(const FloatRect rect, const Vector2f value);

	static bool OBB(const RectangleShape& obb1, const RectangleShape& obb2);
	static bool OBB(const vector<HitBox*> obb1, const HitBox* obb2);
	static bool OBB( FloatRect& obb1,  HitBox& obb2);
	static bool LineRect(Vector2f startPos, Vector2f nextPos, RectangleShape hitObject);
	static bool Lineline(Vector2f startPos, Vector2f nextPos, float x3, float y3, float x4, float y4);
};

