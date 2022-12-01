#pragma once
#include "UiObject.h"

class Fade
    :public UiObject
{
private:
    bool isFadein;
    bool isFadeOut;
    bool isStarted;

    RectangleShape rect;

    float alpha;
    float goal;

public:
    Fade();
    virtual ~Fade() {  }

    void SetFadeOut();
    void SetFadeIn();
    void SetSkip();

    void SetRect(const Vector2f& size);

    virtual void Update(float dt);
    virtual void Draw(RenderWindow& window);

    const RectangleShape& GetRect() { return rect; }

    bool DoFade(float dt);
};

