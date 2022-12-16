#include "QuadTree_SFML_DEMO.h"
#include <iostream>
#include <vector>
#include "../GameObject/HitBoxObject.h"
#include "../GameObject/HitBox.h"
#include "../Framework/Utils.h"
#include <list>

//** Rect **//
TreeRect::TreeRect(const TreeRect& other) : TreeRect(other.x, other.y, other.width, other.height) { }
TreeRect::TreeRect(double _x, double _y, double _width, double _height) :
    x(_x),
    y(_y),
    width(_width),
    height(_height) {

}
bool TreeRect::contains(const TreeRect& other) const noexcept {
    if (x > other.x) return false;
    if (y > other.y) return false;
    if (x + width < other.x + other.width) return false;
    if (y + height < other.y + other.height) return false;
    return true; // with in bounds
}
bool TreeRect::intersects(const TreeRect& other) const noexcept {
    if (x > other.x + other.width)  return false;
    if (x + width < other.x)        return false;
    if (y > other.y + other.height) return false;
    if (y + height < other.y)       return false;
    return true; // intersection
}
bool TreeRect::intersects(HitBoxObject& hit)   {
    auto hitBound = hit.GetBottom()->GetHitbox().getGlobalBounds();
    TreeRect other = { hitBound.left, hitBound.top, hitBound.width, hitBound.height };
    if (x > other.x + other.width)  return false;
    if (x + width < other.x)        return false;
    if (y > other.y + other.height) return false;
    if (y + height < other.y)       return false;
    return true; // intersection
}
bool TreeRect::intersects(sf::FloatRect hit) {
    auto hitBound = hit;
    TreeRect other = { hitBound.left, hitBound.top, hitBound.width, hitBound.height };
    if (x > other.x + other.width)  return false;
    if (x + width < other.x)        return false;
    if (y > other.y + other.height) return false;
    if (y + height < other.y)       return false;
    return true; // intersection
}
double TreeRect::getLeft()   const noexcept { return x - (width * 0.5f); }
double TreeRect::getTop()    const noexcept { return y + (height * 0.5f); }
double TreeRect::getRight()  const noexcept { return x + (width * 0.5f); }
double TreeRect::getBottom() const noexcept { return y - (height * 0.5f); }

////** Collidable **//
//Collidable::Collidable(const TreeRect& _bounds, void* _data) :
//    bound(_bounds),
//    data(_data) {
//};
//
//** QuadTree **//
QuadTree::QuadTree() : QuadTree({}, 0, 0) {
}
//QuadTree::QuadTree(const QuadTree& other) : QuadTree(other.bounds, other.capacity, other.maxLevel) { }
QuadTree::QuadTree(const TreeRect& _bound, unsigned _capacity, unsigned _maxLevel) :
    bounds(_bound),
    capacity(_capacity),
    maxLevel(_maxLevel) {

    objects.reserve(_capacity);
    foundObjects.reserve(_capacity);
    shape.setOutlineThickness(1.f);
    shape.setSize(sf::Vector2f((float)bounds.width - shape.getOutlineThickness(), (float)bounds.height));
    shape.setOutlineColor(sf::Color::Black);
    shape.setPosition((float)_bound.x, (float)_bound.y);
    shape.setFillColor(sf::Color(242, 242, 242));
    text.setFillColor(sf::Color(128, 128, 128));
}

// Inserts an object into this quadtree
bool QuadTree::insert(HitBoxObject* obj) {
    if (obj->qt != nullptr) return false;

    if (!isLeaf) {
        // insert object into leaf
        if (QuadTree* child = getChild(*obj))
            return child->insert(obj);
    }

    objects.push_back(obj);
    obj->qt = this;

    // Subdivide if required
    if (isLeaf && level < maxLevel && objects.size() >= capacity) {
        subdivide();
        update(obj);
    }
    return true;
}

void QuadTree::insert(std::vector<Object*>& objs) {

    for (auto& obj : objs)
    {
        insert((HitBoxObject*)obj);
    }
}

// Removes an object from this quadtree
bool QuadTree::remove(HitBoxObject* obj) {
    if (obj->qt == nullptr) return false; // Cannot exist in vector
    if (obj->qt != this) return obj->qt->remove(obj);

    objects.erase(std::find(objects.begin(), objects.end(), obj));
    obj->qt = nullptr;
    discardEmptyBuckets();
    return true;
}

// Removes and re-inserts object into quadtree (for objects that move)
bool QuadTree::update(HitBoxObject* obj) {
    if (!remove(obj)) return false;

    // Not contained in this node -- insert into parent
    //if (parent != nullptr && !bounds.contains(obj->bound))
    //    return parent->insert(obj);
    if (!isLeaf) {
        // Still within current node -- insert into leaf
        if (QuadTree* child = getChild(*obj))
            return child->insert(obj);
    }
    return insert(obj);
}

void QuadTree::update(std::vector<Object*> objs) {

    for (auto& obj : objs)
    {
        update((HitBoxObject*)obj);
    }
}

// Searches quadtree for objects within the provided boundary and returns them in vector
std::vector<HitBoxObject*>& QuadTree::getObjectsInBound_unchecked( HitBoxObject& bound) {
    foundObjects.clear();
    for (const auto& obj : objects) {
        // Only check for intersection with OTHER boundaries
        foundObjects.push_back(obj);
    }
    if (!isLeaf) {
		// Get objects from leaves
		if (QuadTree* child = getChild(bound))
		{
			child->getObjectsInBound_unchecked(bound);
			foundObjects.insert(foundObjects.end(), child->foundObjects.begin(), child->foundObjects.end());
		}
		else for (QuadTree* leaf : children) {
			if (leaf->bounds.intersects(bound)) {
				leaf->getObjectsInBound_unchecked(bound);
				foundObjects.insert(foundObjects.end(), leaf->foundObjects.begin(), leaf->foundObjects.end());
			}
        }
    }
    return foundObjects;
}

std::vector<HitBoxObject*>& QuadTree::getObjectsInBound_unchecked(sf::FloatRect bound) {
    foundObjects.clear();
    for (const auto& obj : objects) {
        // Only check for intersection with OTHER boundaries
        foundObjects.push_back(obj);
    }
    if (!isLeaf) {
        // Get objects from leaves
        if (QuadTree* child = getChild(bound))
        {
            child->getObjectsInBound_unchecked(bound);
            foundObjects.insert(foundObjects.end(), child->foundObjects.begin(), child->foundObjects.end());
        }
        else for (QuadTree* leaf : children) {
            if (leaf->bounds.intersects(bound)) {
                leaf->getObjectsInBound_unchecked(bound);
                foundObjects.insert(foundObjects.end(), leaf->foundObjects.begin(), leaf->foundObjects.end());
            }
        }
    }
    return foundObjects;
}


std::vector<HitBoxObject*>& QuadTree::getObjectsInBound_unchecked_notParent(HitBoxObject& bound) {
    foundObjects.clear();

    auto bound_leafs = getAllLeaf(bound);
    foundObjects = getObjectsInBound_unchecked(bound);

    for (auto it = foundObjects.begin(); it != foundObjects.end();)
    {
        bool isIntersects = false;
        for (auto& b : bound_leafs)
        {
            HitBoxObject& test = *(*it);
            auto bound = b->GetBound();
            if (bound.intersects(test))
            {
                isIntersects = true;;
            }
        }
        if (!isIntersects)
            it = foundObjects.erase(it);
        else
            it++;
    }
    auto mine = find(foundObjects.begin(), foundObjects.end(), &bound);
    if (mine != foundObjects.end()) foundObjects.erase(mine);
    return foundObjects;
}

std::vector<HitBoxObject*>& QuadTree::getObjectsInBound_unchecked_notParent(sf::FloatRect bound)
{
    foundObjects.clear();

    auto bound_leafs = getAllLeaf(bound);
    foundObjects = getObjectsInBound_unchecked(bound);

    for (auto it = foundObjects.begin(); it != foundObjects.end();)
    {
        bool isIntersects = false;
        for (auto& b : bound_leafs)
        {
            HitBoxObject& test = *(*it);
            auto bound = b->GetBound();
            if (bound.intersects(test))
            {
                isIntersects = true;;
            }
        }
        if (!isIntersects)
            it = foundObjects.erase(it);
        else
            it++;
    }
    return foundObjects;
}

// Returns total children count for this quadtree
unsigned QuadTree::totalChildren() const noexcept {
    unsigned total = 0;
    if (isLeaf) return total;
    for (QuadTree* child : children)
    {
        total += child->totalChildren();
    }
    return 4 + total;
}

// Returns total object count for this quadtree
unsigned QuadTree::totalObjects() const noexcept {
    unsigned total = (unsigned)objects.size();
    if (!isLeaf) {
        for (QuadTree* child : children)
        {
            total += child->totalObjects();
        }
    }
    return total;
}
// Returns total object count for this quadtree
unsigned QuadTree::totalObjects(TreeRect now_bound) const noexcept {
    unsigned total = (unsigned)objects.size();
    if (!isLeaf) {
        for (QuadTree* child : children)
        {
            if ((child != nullptr) && child->GetBound().intersects(now_bound))
            {
                total += child->totalObjects(now_bound);
            }
        }
    }
    return total;
}


void QuadTree::setFont(const sf::Font& font) noexcept {
    text.setFont(font);
    text.setCharacterSize(40 / (level ? level : 1));
    text.setPosition(
        (float)bounds.getRight() - (text.getLocalBounds().width * 0.5f),
        (float)bounds.getTop() - 1 - (text.getLocalBounds().height * 0.5f)
    );
    if (isLeaf) return;
    for (QuadTree* child : children)
        child->setFont(font);
}
void QuadTree::setFillColor(sf::Color color) noexcept {
    shape.setFillColor(color);
}

void QuadTree::draw(sf::RenderTarget& canvas) noexcept {
    setFont(*text.getFont());

    if (!objects.empty())
        shape.setFillColor(sf::Color(255,255,255,40));
    else
        shape.setFillColor(sf::Color(242, 242, 242,30));

    canvas.draw(shape);
    if (!isLeaf) {
        for (QuadTree* child : children)
            child->draw(canvas);
    }
    else {
        std::stringstream ss;
        ss << level;
        text.setString(ss.str());
        canvas.draw(text);
    }
}

// Removes all objects and children from this quadtree
void QuadTree::clear() noexcept {
    if (!objects.empty()) {
        for (auto&& obj : objects)
        {
            obj->qt = nullptr;
        }
        objects.clear();
    }

    if (!isLeaf) {
        for (QuadTree* child : children)
            child->clear();
        isLeaf = true;
    }
}

// Subdivides into four quadrants
void QuadTree::subdivide() {
    double width = bounds.width * 0.5f;
    double height = bounds.height * 0.5f;
    double x = 0, y = 0;
    for (unsigned i = 0; i < 4; ++i) {
        switch (i) {
        case 0: x = bounds.x + width; y = bounds.y; break; // Top right
        case 1: x = bounds.x;         y = bounds.y; break; // Top left
        case 2: x = bounds.x;         y = bounds.y + height; break; // Bottom left
        case 3: x = bounds.x + width; y = bounds.y + height; break; // Bottom right
        }
        children[i] = new QuadTree({ x, y, width, height }, capacity, maxLevel);
        children[i]->level = level + 1;
        children[i]->parent = this;
    }
    isLeaf = false;
}

// Discards buckets if all children are leaves and contain no objects
void QuadTree::discardEmptyBuckets() {
    if (!objects.empty()) return;
    if (!isLeaf) {
        for (QuadTree* child : children)
            if (!child->isLeaf || !child->objects.empty())
                return;
    }
    if (clear(), parent != nullptr)
        parent->discardEmptyBuckets();
}

QuadTree* QuadTree::getLeaf( HitBoxObject& bound) {
    QuadTree* leaf = this;
    if (!isLeaf) {
        if (QuadTree* child = getChild(bound))
            leaf = child->getLeaf(bound);
    }
    return leaf;
}

std::vector<QuadTree*> QuadTree::getAllLeaf(HitBoxObject& bound) {

    std::vector<QuadTree*> leafs;
    QuadTree* leaf = this;
    if (!isLeaf)
    {
        for (auto& childe : children)
        {
            if (childe->bounds.intersects(bound))
            {
                auto childLeaf = childe->getAllLeaf(bound);
                leafs.insert(leafs.end(), childLeaf.begin(), childLeaf.end());
            }
        }
    }
    else
    {
        leafs.push_back(this);
    }
    return leafs;
}
std::vector<QuadTree*> QuadTree::getAllLeaf(sf::FloatRect bound) {

    std::vector<QuadTree*> leafs;
    QuadTree* leaf = this;
    if (!isLeaf)
    {
        for (auto& childe : children)
        {
            if (childe->bounds.intersects(bound))
            {
                auto childLeaf = childe->getAllLeaf(bound);
                leafs.insert(leafs.end(), childLeaf.begin(), childLeaf.end());
            }
        }
    }
    else
    {
        leafs.push_back(this);
    }
    return leafs;
}
// Returns child that contains the provided boundary
QuadTree* QuadTree::getChild( HitBoxObject& hit)   noexcept {

    auto hitBound = hit.GetBottom()->GetHitbox().getGlobalBounds();
    TreeRect bound = { hitBound.left, hitBound.top, hitBound.width, hitBound.height };

    bool left = bound.x + bound.width < bounds.getRight();
    bool right = bound.x > bounds.getRight();

    if (bound.y + bound.height < bounds.getTop()) {
        if (left)  return children[1]; // Top left
        if (right) return children[0]; // Top right
    }
    else if (bound.y > bounds.getTop()) {
        if (left)  return children[2]; // Bottom left
        if (right) return children[3]; // Bottom right
    }
    return nullptr; // Cannot contain boundary -- too large
}

QuadTree* QuadTree::getChild(sf::FloatRect& hit)   noexcept {

    auto hitBound = hit;
    TreeRect bound = { hitBound.left, hitBound.top, hitBound.width, hitBound.height };

    bool left = bound.x + bound.width < bounds.getRight();
    bool right = bound.x > bounds.getRight();

    if (bound.y + bound.height < bounds.getTop()) {
        if (left)  return children[1]; // Top left
        if (right) return children[0]; // Top right
    }
    else if (bound.y > bounds.getTop()) {
        if (left)  return children[2]; // Bottom left
        if (right) return children[3]; // Bottom right
    }
    return nullptr; // Cannot contain boundary -- too large
}
QuadTree::~QuadTree() {
    clear();
    if (children[0]) 
        delete children[0];
    if (children[1])
        delete children[1];
    if (children[2]) 
        delete children[2];
    if (children[3]) 
        delete children[3];
}
