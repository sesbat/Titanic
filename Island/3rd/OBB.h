#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <cassert>

using namespace sf;
using namespace std;

static const float NORMAL_TOLERANCE = 0.0001f;
using RectVertexArray = array<Vector2f, 4>;

float getLength(const Vector2f& v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

// Returns normalized vector
sf::Vector2f getNormalized(const Vector2f& v)
{
	float length = getLength(v);
	if (length < NORMAL_TOLERANCE) { return Vector2f(); }
	return Vector2f(v.x / length, v.y / length);
}

float dotProduct(const Vector2f& a, const Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

// Returns right hand perpendicular vector
Vector2f getNormal(const Vector2f& v)
{
	return Vector2f(-v.y, v.x);
}

// Find minimum and maximum projections of each vertex on the axis
Vector2f projectOnAxis(const RectVertexArray& vertices, const Vector2f& axis)
{
	float min = numeric_limits<float>::infinity();
	float max = -numeric_limits<float>::infinity();
	for (auto& vertex : vertices) {
		float projection = dotProduct(vertex, axis);
		if (projection < min) { min = projection; }
		if (projection > max) { max = projection; }
	}
	return Vector2f(min, max);
}

// a and b are ranges and it's assumed that a.x <= a.y and b.x <= b.y
bool areOverlapping(const Vector2f& a, const Vector2f& b)
{
	return a.x <= b.y && a.y >= b.x;
}

// a and b are ranges and it's assumed that a.x <= a.y and b.x <= b.y
float getOverlapLength(const Vector2f& a, const Vector2f& b)
{
	if (!areOverlapping(a, b)) { return 0.f; }
	return min(a.y, b.y) - max(a.x, b.x);
}

Vector2f getCenter(const RectangleShape& shape)
{
	const Transform& transform = shape.getTransform();
	FloatRect local = shape.getLocalBounds();
	return transform.transformPoint(local.width / 2.f, local.height / 2.f);
}

RectVertexArray getVertices(const RectangleShape& shape)
{
	RectVertexArray vertices;
	const Transform& transform = shape.getTransform();
	for (size_t i = 0u; i < shape.getPointCount(); ++i) {
		vertices[i] = transform.transformPoint(shape.getPoint(i));
	}
	return vertices;
}

Vector2f getPerpendicularAxis(const RectVertexArray& vertices, size_t index)
{
	assert(index >= 0 && index < 4); // rect has 4 possible axes
	return getNormal(getNormalized(vertices[index + 1] - vertices[index]));
}

// axes for which we'll test stuff. Two for each box, because testing for parallel axes isn't needed
array<Vector2f, 4> getPerpendicularAxes(const RectVertexArray& vertices1, const RectVertexArray& vertices2)
{
	array<Vector2f, 4> axes;

	axes[0] = getPerpendicularAxis(vertices1, 0);
	axes[1] = getPerpendicularAxis(vertices1, 1);

	axes[2] = getPerpendicularAxis(vertices2, 0);
	axes[3] = getPerpendicularAxis(vertices2, 1);
	return axes;
}

// Separating Axis Theorem (SAT) collision test
// Minimum Translation Vector (MTV) is returned for the first Oriented Bounding Box (OBB)
bool testCollision(const RectangleShape& obb1, const RectangleShape& obb2,
	Vector2f& mtv)
{
	RectVertexArray vertices1 = getVertices(obb1);
	RectVertexArray vertices2 = getVertices(obb2);
	RectVertexArray axes = getPerpendicularAxes(vertices1, vertices2);

	// we need to find the minimal overlap and axis on which it happens
	float minOverlap = numeric_limits<float>::infinity();

	for (auto& axis : axes) {
		Vector2f proj1 = projectOnAxis(vertices1, axis);
		Vector2f proj2 = projectOnAxis(vertices2, axis);

		float overlap = getOverlapLength(proj1, proj2);
		if (overlap == 0.f) { // shapes are not overlapping
			mtv = Vector2f();
			return false;
		}
		else {
			if (overlap < minOverlap) {
				minOverlap = overlap;
				mtv = axis * minOverlap;
				// ideally we would do this only once for the minimal overlap
				// but this is very cheap operation
			}
		}
	}

	// need to reverse MTV if center offset and overlap are not pointing in the same direction
	bool notPointingInTheSameDirection = dotProduct(getCenter(obb1) - getCenter(obb2), mtv) < 0;
	if (notPointingInTheSameDirection) { mtv = -mtv; }
	return true;
}