#pragma once
#include "Object.h"
#include "../../Framework/Utils.h"

class VertexArrayObj : public Object
{
protected:
	VertexArray vertexArray;
	Texture* texture;
	Origins origin;
	

public:
	VertexArrayObj();
	~VertexArrayObj();

	VertexArray& GetVA();
	void SetTexture(Texture* texture);
	Texture* GetTextrue() const;

	void SetOrigin(Origins origin);
	Origins GetOrigin() const;
	virtual void SetPos(const Vector2f& pos);
	virtual const Vector2f& GetPos() const;
	virtual void Update(float dt);
	virtual void SetActive(bool active);
	virtual void Draw(RenderWindow& window);

	FloatRect GetGlobalBounds() const;
};

