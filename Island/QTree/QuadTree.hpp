#pragma once
#include "Common.hpp"
#include "QuadTreeNode.hpp"


class Object;

class QuadTree
{
public:
    explicit QuadTree(sf::FloatRect bounds,
                      size_t maxLevel, size_t maxObjects);

    size_t countObjects() const;

    void insert(std::vector<Object*> const& ptrArray);

    void clear();

    void drawBorders(sf::RenderTarget& t) const;

    std::vector<Object*> getCollisionable(sf::FloatRect const& bounds) const;
private:
    sf::FloatRect const mBounds;
    size_t const mMaxLevel, mMaxObjects;

    QuadTreeNode mRoot;
};
