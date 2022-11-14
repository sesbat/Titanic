#pragma once
#include "../GameObject/Object.h"
enum class UiState
{
	None = -1, Enter, Stay, Down, Up,DownRight, UpRight, Exit
};
class UiObject : public Object
{
public:
protected:
	FloatRect bound;
	UiState btnState;
	bool isEvent;

	bool isClick;
	bool isClickR;

	bool isMove;
	bool isMoveR;
	bool drag;
	bool dragR;
	Vector2f initPos;

	bool isUiView;

public:
	UiObject();
	virtual ~UiObject();
	virtual void Init() override;
	virtual void Release()override;
	virtual void Reset()override;
	virtual void SetPos(const Vector2f& pos)override;
	virtual void Translate(const Vector2f& delta)override;

	void EventClear();

	virtual void StateClear();
	virtual void ColorClear() {}

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);

	virtual bool IsDown() const { return (btnState == UiState::Down) && isEvent; }
	virtual bool IsDownRight() const { return (btnState == UiState::DownRight) && isEvent; }
	virtual bool IsClick() const { return isClick; }
	virtual bool IsClickRight() const { return isClickR; }
	virtual bool IsUp() const { return (btnState == UiState::Up) && isEvent; }
	virtual bool IsUpRight() const { return (btnState == UiState::UpRight) && isEvent; }
	virtual bool IsEnter() const { return (btnState == UiState::Enter) && isEvent; }
	virtual bool IsExit() const { return (btnState == UiState::Exit) && isEvent; }
	virtual bool IsStay() const { return (btnState == UiState::Stay) && isEvent; }
	virtual bool IsNone() const { return (btnState == UiState::None); }
	virtual bool IsDrag() const { return drag; }
	virtual bool IsDragRight() const { return dragR; }
	virtual bool GetEvent() const { return isEvent; }
	virtual UiState GetState() const { return btnState; }
	virtual void SetState(UiState state) { btnState = state; isEvent = true; }
	virtual void SetEvent(bool ev) { isEvent = ev; }
	virtual void SetMove(bool m) { isMove = m; }
	virtual void SetMoveRight(bool m) { isMoveR = m; }
	virtual bool IsMove() { return isMove; }
	virtual bool IsMoveRight() { return isMoveR; }
	virtual void SetUiView(bool state) { isUiView = state; }

	void SetBound(FloatRect b) { bound = b; }
};

