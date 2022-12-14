#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "../Framework/FileManager.h"
#include <list>

using namespace std;
using namespace sf;


class Object
{
protected:
	Object(const Object& ref) {}
	Object& operator=(const Object& ref) {}
	Object( Object& ref) {}
	Object& operator=( Object& ref) {}
	int id;
	string name; 
	bool enabled; 

	Vector2f position; 
	float rotation;
	Vector2f scale;   

	bool isUi;
	bool viewIn;

public:
	Object();
	virtual ~Object() {};

	virtual bool GetActive() const;
	virtual void SetActive(bool active);
	
	virtual void Init();
	virtual void Reset();

	virtual void SetPos(const Vector2f& pos);
	virtual void Translate(const Vector2f& delta);
	virtual const Vector2f& GetPos() const;

	virtual FloatRect GetGlobalBound() { return{ 0,0,0,0 }; }
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	void SetUI(bool u) { isUi = u; }
	virtual bool IsInView() { return true; };
	void SetId(int id) { this->id = id; }
	void SetName(string name) { this->name = name; }
	const string& GetName() const { return name; }
	const int& GetId() const { return id; }
};