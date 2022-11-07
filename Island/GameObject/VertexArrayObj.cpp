#include "VertexArrayObj.h"

VertexArrayObj::VertexArrayObj()
    :texture(nullptr), origin(Origins::TL)
{
}

VertexArrayObj::~VertexArrayObj()
{

}

void VertexArrayObj::SetPos(const Vector2f& pos)
{
    Vector2f delta = pos - position;
    position = pos;
    for (int i = 0; i < vertexArray.getVertexCount(); i++)
    {
        vertexArray[i].position += delta;
    }
    position = pos;
}

const Vector2f& VertexArrayObj::GetPos() const
{
    return position;
}

VertexArray& VertexArrayObj::GetVA()
{
    return vertexArray;
}

void VertexArrayObj::SetTexture(Texture* texture)
{
    this->texture = texture;
}

Texture* VertexArrayObj::GetTextrue() const
{
    return texture;
}

void VertexArrayObj::SetOrigin(Origins origin)
{
    FloatRect rect = vertexArray.getBounds();

	Vector2f prevPos(
		rect.width  * ((int)this->origin % 3) * 0.5,
		rect.height * ((int)this->origin / 3) * 0.5f);
	Vector2f newPos(
		rect.width  * ((int)origin % 3) * 0.5,
		rect.height * ((int)origin / 3) * 0.5f);

    Translate(prevPos - newPos);
    this->origin = origin;
}

Origins VertexArrayObj::GetOrigin() const
{
    return origin;
}

void VertexArrayObj::Update(float dt)
{
    if (enabled)
    {
    }
}
void VertexArrayObj::SetActive(bool active)
{
    Object::SetActive(active);
}
void VertexArrayObj::Draw(RenderWindow& window)
{
    if (enabled)
    {
        window.draw(vertexArray, texture);
    }
}

FloatRect VertexArrayObj::GetGlobalBounds() const
{
    return  vertexArray.getBounds();
}
