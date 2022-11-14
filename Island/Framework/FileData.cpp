#include "FileData.h"

void sf::to_json(json& j, const Vector2f& p)
{
    j = json{ {"x", p.x}, {"y", p.y} };
}

void sf::from_json(const json& j, Vector2f& p)
{
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
}

void ns::to_json(json& j, const RectangleInfo& c)
{
    j = json{ {"size", c.size}, {"pos", c.pos} };
}

void ns::from_json(const json& j, RectangleInfo& c)
{
    j.at("size").get_to(c.size);
    j.at("pos").get_to(c.pos);
}

