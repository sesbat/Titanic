#pragma once
#include <iostream>
#include <set>
#include <vector>
#include <stack>
#include <SFML/Graphics.hpp>
#include <list>

using namespace std;
using namespace sf;

typedef pair<int, int> Pair;
typedef pair<double, pair<int, int> > pPair;

const int g_maxX = 128;
const int g_maxY = 72;

class Astar
{
public:
	struct Cell {
		int parentX, parentY;
		double f, g, h;
	};

private:
	Astar(const Astar& ref) {}
	Astar& operator=(const Astar& ref) {}
	int dxDir[4] = { 0, 0, 1, -1 };
	int dyDir[4] = { -1, 1, 0, 0 };

	Vector2i startPos;
	list<Vector2f> movePos;
	Vector2i prevPos;

public:
	Astar();
	~Astar();
	bool IsValid(int x, int y);
	bool IsDestination(int x, int y, Pair dest);
	int GetDistance(int x1, int y1, int x2, int y2);
	void TracePath(vector<vector<bool>> map, Cell cellDetails[g_maxY][g_maxX], Pair dest);
	void AstarSearch(vector<vector<bool>> map, Pair start, Pair dest);
	list<Vector2f> GetCoordinate();
	//void DiagonalMove(vector<vector<bool>> map, int x, int y);
};

