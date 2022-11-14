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
	virtual ~Object();

	virtual bool GetActive() const;
	virtual void SetActive(bool active);
	
	virtual void Init();
	virtual void Release();
	virtual void Reset();

	virtual void SetPos(const Vector2f& pos);
	virtual void Translate(const Vector2f& delta);
	virtual const Vector2f& GetPos() const;

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	void SetUI(bool u) { isUi = u; }
	bool IsInView();
	void SetId(int id) { this->id = id; }
	void SetName(string name) { this->name = name; }
	const string& GetName() const { return name; }
	const int& GetId() const { return id; }
};