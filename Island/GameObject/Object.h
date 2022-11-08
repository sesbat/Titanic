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
	string name; //다른 오브젝트랑 중복 가능
	bool enabled; //활성화 상태냐 비활성화 상태냐 그것이 문제로다

	Vector2f position; //실제로 사용하지 않을수도 있지만 가지고 있어야 함.
	float rotation;
	Vector2f scale;    //부모자식 간의 관계가 있을수도 있지만 그건 일단 우리가 컨테이너로 만들어서 하자

	map<string, vector<HitBox>> allHitBoxs;
	vector<HitBox> hitBoxs;
	HitBox bottomHitBox;
	map<string, HitBox> allBottom;
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
	virtual void AddHitBox(vector<ns::RectangleInfo>& hit, Vector2f pos);
	virtual void AddHitBox(RectangleShape hit, Vector2f pos, bool isBottom = false);
	void AddHitBox(Shape* hitbox, Vector2f initPos);
	void AddHitBox(HitBox hit);
	virtual const vector<HitBox>* GetHitBox();
	virtual const HitBox& GetHitBoxBottom();

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	void SetName(string name) { this->name = name; }
	const string& GetName() const { return name; }
};