#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <map>

using namespace sf;
using namespace std;

enum class Axis
{
	Horizontal,
	Vertical,
};

struct AxisInfo
{
	Axis axis;
	list<Keyboard::Key> positives;
	list<Keyboard::Key> negatives;

	float sensi;
	float value;
};

// ����ƽ �Լ��� �̿��ؼ� Ű���� �� ���콺 �Է��� �˻��� �� �ִ� Ŭ����

class InputMgr
{
private:
	static map<Axis, AxisInfo> axisInfoMap;

	static list<Keyboard::Key> downList; // 
	static list<Keyboard::Key> ingList;	 // 
	static list<Keyboard::Key> upList;	 // 

	static list<Mouse::Button> downMouse; // 
	static list<Mouse::Button> ingMouse;  // 
	static list<Mouse::Button> upMouse;	  // 

	static Vector2f mousePos;

public:
	static void Init();
	static void Clear();
	static void Update(float dt);
	static void ProcessInput(Event& ev);

	static bool GetKeyDown(Keyboard::Key key);
	static bool GetKey(Keyboard::Key key);
	static bool GetKeyUp(Keyboard::Key key);

	static bool GetMouseButtonDown(Mouse::Button key);
	static bool GetMouseButton(Mouse::Button key);
	static bool GetMouseButtonUp(Mouse::Button key);

	static const Vector2f& GetMousePos();

	static float GetAxis(Axis axis);
	static float GetAxisRaw(Axis axis);
};
