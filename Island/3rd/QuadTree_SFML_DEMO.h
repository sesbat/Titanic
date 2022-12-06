#pragma once
#include <any>
#include <vector>
#include <sstream>
#include <algorithm>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class HitBoxObject;
class Object;
struct TreeRect {
    double x, y, width, height;

    bool contains(const TreeRect &other) const noexcept;
    bool intersects(const TreeRect &other) const noexcept;
    bool intersects(HitBoxObject& hit);
    bool intersects(sf::FloatRect hit);
    double getLeft() const noexcept;
    double getTop() const noexcept;
    double getRight() const noexcept;
    double getBottom() const noexcept;

    TreeRect(const TreeRect&);
    TreeRect(double _x = 0, double _y = 0, double _width = 0, double _height = 0);
};

//struct Collidable {
//    friend class QuadTree;
//public:
//    TreeRect bound;
//    void* data;
//
//    Collidable(const TreeRect &_bounds = {}, void* _data = {});
//private:
//    QuadTree *qt = nullptr;
//    Collidable(const Collidable&) = delete;
//};

class QuadTree
{
public:
    QuadTree(const TreeRect &_bound, unsigned _capacity, unsigned _maxLevel);

    bool insert(HitBoxObject*obj);
    void insert(std::vector<Object*>& obj);
    bool remove(HitBoxObject*obj);
    bool update(HitBoxObject*obj);
    void update(std::vector<Object*> objs);
    std::vector<HitBoxObject*> &getObjectsInBound_unchecked( HitBoxObject &bound);
    std::vector<HitBoxObject*>& getObjectsInBound_unchecked(sf::FloatRect bound);
    std::vector<HitBoxObject*>& getObjectsInBound_unchecked_notParent(HitBoxObject& bound);
    std::vector<HitBoxObject*>& getObjectsInBound_unchecked_notParent(sf::FloatRect bound);
    unsigned totalChildren() const noexcept;
    unsigned totalObjects() const noexcept;
    unsigned totalObjects(TreeRect now_bound) const noexcept;
    void setFont(const sf::Font &font) noexcept;
    void setFillColor(sf::Color color) noexcept;
    void draw(sf::RenderTarget &canvas) noexcept;
    void clear() noexcept;
    QuadTree *getLeaf( HitBoxObject&bound);
    std::vector<QuadTree*> getAllLeaf(HitBoxObject& bound);
    std::vector<QuadTree*> getAllLeaf(sf::FloatRect bound);
    const TreeRect& GetBound() { return bounds; }
    std::vector<HitBoxObject*>& getObjects() { return objects; }
    
    QuadTree* getChild(sf::FloatRect& hit) noexcept;
    
    ~QuadTree();
private:
	QuadTree();
	QuadTree(const QuadTree& ref) {}
	QuadTree& operator=(const QuadTree& ref) {}
	QuadTree(QuadTree& ref) {}
	QuadTree& operator=(QuadTree& ref) {}
    bool      isLeaf = true;
    unsigned  level  = 0;
    unsigned  capacity;
    unsigned  maxLevel;
    TreeRect      bounds;
    QuadTree* parent = nullptr;
    QuadTree* children[4] = { nullptr, nullptr, nullptr, nullptr };
    sf::Text  text;
    sf::RectangleShape	     shape;
    std::vector<HitBoxObject*> objects, foundObjects;

    void subdivide();
    void discardEmptyBuckets();
    inline QuadTree *getChild( HitBoxObject &hit)  noexcept;
};
