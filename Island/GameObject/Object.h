#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "../Framework/FileManager.h"

#include <list>

using namespace std;
using namespace sf;


struct HitBox
{
	Vector2f initPos;
	Shape* shape;
	HitBox()
		:shape(nullptr) {}
	void SetPos(Vector2f pos)
	{
		shape->setPosition(pos + initPos);
	}
};

class Object
{
protected:
	int id;
	string name; 
	bool enabled; 

	Vector2f position; 
	float rotation;
	Vector2f scale;   

	vector<HitBox> hitBoxs;
	HitBox bottomHitBox;
	bool hitDraw;
public:
	Object();
	virtual ~Object();

	virtual bool GetActive() const;
	virtual void SetActive(bool active);
	
	virtual void Init();
	virtual void Release();
	virtual void Reset();

	virtual void SetPos(const Vector2f& pos);
	virtual void Translate(const Vector2f& delta);
	virtual const Vector2f& GetPos() const;
	void AddHitBox(string path);
	virtual const vector<HitBox>* GetHitBox();
	virtual const HitBox& GetHitBoxBottom();

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	void SetName(string name) { this->name = name; }
	const string& GetName() const { return name; }
};