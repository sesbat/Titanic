#include "Astar.h"

Astar::Astar()
{
}

Astar::~Astar()
{
}

bool Astar::IsValid(int x, int y)
{
	if (x < 0 || y < 0 || x >= g_maxX || y >= g_maxY)
		return false;
	return true;
}

bool Astar::IsDestination(int x, int y, Pair dest)
{
	if (x == dest.first && y == dest.second)
		return true;
	return false;
}

int Astar::GetDistance(int x1, int y1, int x2, int y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void Astar::TracePath(vector<vector<bool>> map, Cell cellDetails[g_maxY][g_maxX], Pair dest)
{
	int x = dest.first;
	int y = dest.second;

	stack<Pair> path;

	while (!(cellDetails[y][x].parentX == x && cellDetails[y][x].parentY == y)) {
		path.push(make_pair(x, y));

		int tempX = cellDetails[y][x].parentX;
		int tempY = cellDetails[y][x].parentY;
		x = tempX;
		y = tempY;
	}
	movePos.clear();
	path.push(make_pair(x, y));
	prevPos = { path.top().first ,path.top().second };
	while (!path.empty()) {
		pair<int, int> p = path.top();
		//prevPos = { p.first ,p.second };
		if (prevPos.x != p.first && prevPos.y != p.second)
		{
			DiagonalMove(map, p.first, p.second);
		}
		movePos.push_back({ ((float)p.first * 60.f) + 30.f ,((float)p.second * 60.f) + 20.f });
		prevPos = { p.first ,p.second };
		path.pop();

		cout << "->(" << p.first << "," << p.second << ")" << "\n";
	}
	if (!movePos.empty())
	{
		movePos.pop_front();
	}
	cout << "\n";
}

void Astar::AstarSearch(vector<vector<bool>> map, Pair start, Pair dest)
{
	//isGreedObject = map;
	if (false == IsValid(start.first, start.second))
		return;
	if (false == IsValid(dest.first, dest.second))
		return;

	if (true == map[start.first][start.second] ||
		true == map[dest.first][dest.second])
		return;

	if (IsDestination(start.first, start.second, dest))
		return;

	bool closedList[g_maxY][g_maxX];
	memset(closedList, false, sizeof(closedList));

	Cell cellDetails[g_maxY][g_maxX];

	// init cells
	for (int i = 0; i < g_maxY; ++i) {
		for (int j = 0; j < g_maxX; ++j) {
			cellDetails[i][j].f = (numeric_limits<float>::max)();
			cellDetails[i][j].g = (numeric_limits<float>::max)();
			cellDetails[i][j].h = (numeric_limits<float>::max)();
			cellDetails[i][j].parentX = -1;
			cellDetails[i][j].parentY = -1;
		}
	}

	// 시작 노드를 초기화 한다.
	int xx = start.first, yy = start.second; // i = y, j = x
	cellDetails[yy][xx].f = 0.f;
	cellDetails[yy][xx].g = 0.f;
	cellDetails[yy][xx].h = 0.f;
	cellDetails[yy][xx].parentX = xx;
	cellDetails[yy][xx].parentY = yy;

	// open list를 만든다.
	set<pPair> openList;

	// 시작 지점의 f를 0으로 둔다.
	openList.insert(make_pair(0.f, make_pair(xx, yy)));

	bool foundDest = false;

	while (!openList.empty()) {
		pPair p = *openList.begin();

		openList.erase(openList.begin()); // openList에 있는 vertex를 삭제.

		xx = p.second.first; // x
		yy = p.second.second;// y
		closedList[yy][xx] = true;

		// 8방향의 successor를 생성한다.
		for (int d = 0; d < 4; ++d) {
			int y = yy + dyDir[d];//yDir[d];
			int x = xx + dxDir[d];// xDir[d];

			if (false == IsValid(x, y))
				continue;

			if (true == IsDestination(x, y, dest)) {
				// 목적지 Cell의 부모를 설정한다.
				cellDetails[y][x].parentX = xx;
				cellDetails[y][x].parentY = yy;
				cout << "dest found" << endl;
				TracePath(map, cellDetails, dest);
				foundDest = true;
				return;
			}
			else if (false == closedList[y][x] && false == map[y][x]) {
				float gNew = cellDetails[yy][xx].g + 1.0f;
				float hNew = GetDistance(x, y, dest.first, dest.second);
				float fNew = gNew + hNew;

				// openList가 아니면 openList에 추가한다.
				// 이미 openList면 
				if (cellDetails[y][x].f == (numeric_limits<float>::max)() ||
					cellDetails[y][x].f > fNew) {
					openList.insert(make_pair(fNew, make_pair(x, y)));

					cellDetails[y][x].f = fNew;
					cellDetails[y][x].g = gNew;
					cellDetails[y][x].h = hNew;
					cellDetails[y][x].parentX = xx;
					cellDetails[y][x].parentY = yy;
				}
			}
		}
	}
	if (false == foundDest)
	{
		cout << "dest not found" << endl;
		return;
	}
		
}

void Astar::DiagonalMove(vector<vector<bool>> map, int x, int y)
{
	for (int i = 0; i < 4; i++)
	{
		if (map[prevPos.y + dyDir[i]][prevPos.x + dxDir[i]] == true)
		{
			switch (i)
			{
			case 0:	//up
			case 1:	//down
				//right
				if (prevPos.x < x)
				{
					movePos.push_back({ ((float)(prevPos.x + 1) * 60.f) + 30.f ,
						((float)prevPos.y * 60.f) + 30.f });
					cout << "->(" << prevPos.x + 1 << "," << prevPos.y << ")" << "\n";

					return;
				}
				//left
				else
				{
					movePos.push_back({ ((float)(prevPos.x - 1) * 60.f) + 30.f ,
						((float)prevPos.y * 60.f) + 30.f });
					cout << "->(" << prevPos.x - 1 << "," << prevPos.y << ")" << "\n";
					return;
				}
				break;
			case 2:	//right
			case 3:	//left
				//up
				if (prevPos.y > y)
				{
					movePos.push_back({ ((float)prevPos.x  * 60.f) + 30.f ,
						((float)(prevPos.y + 1) * 60.f) + 30.f });
					cout << "->(" << prevPos.x << "," << prevPos.y +1 << ")" << "\n";

					return;
				}
				//down
				else
				{
					movePos.push_back({ ((float)prevPos.x * 60.f) + 30.f ,
						((float)(prevPos.y - 1) * 60.f) + 30.f });
					cout << "->(" << prevPos.x << "," << prevPos.y - 1 << ")" << "\n";

					return;
				}
				break;
				
			}
		}
		else
		{
			return;
		}
	}
}

