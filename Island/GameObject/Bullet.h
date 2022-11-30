#pragma once
#include "SpriteObject.h"

class Scene;
class HitBox;
class Enemy;
class Player;


class Bullet :public SpriteObject
{
private:
	Scene* scene;
	list<Enemy*>* enemies;
	Player* player;

	Vector2f dir;
	float speed;
	float range;

	Vector2f startPos;
	Vector2f nextPos;

	bool isplayer;
	bool showLine;
public:
	Bullet();
	~Bullet();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void Reset();
	virtual void Release();

	void Fire(const Vector2f& pos, const Vector2f& dir, float speed, float range, bool isplayer);

	bool EqualFloat(Vector2f a, Vector2f b);
	bool EqualFloat(float a, float b);

	void SetPlayer(Player* player){this->player = player;}
	void SetEnemyList(list<Enemy*>* list) { enemies = list; }

	bool LineRect(Vector2f bulletpos, Vector2f bulletPrevPos, RectangleShape hitObject);
	bool Lineline(Vector2f bulletpos, Vector2f bulletPrevPos, float x3, float y3, float x4, float y4);
	void Collision();
};

