#ifndef __Sokoban
#include <iostream>
#include <stdio.h>
#include <vector>
#include <queue>
#include <map>
#include <tr1/unordered_map>
#include <tr1/unordered_set>
using namespace std;

const int direction[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
const char step[4] = {'d', 'u', 'r', 'l'};

struct Position {
	int x, y;
	Position() {}
	Position(int _x, int _y) : x(_x), y(_y) {}
	bool operator == (const Position &b) const
	{
		return x == b.x && y == b.y;
	}
	bool operator != (const Position &b) const
	{
		return !(x == b.x && y == b.y);
	}
};
struct State {
	Position person;
	vector<Position>box;
	vector<char>move;
};

#define isWall(c) (c == '#' ? true : false)
#define isGoal(c) (c == '.' ? 1 : ( c == '+' ? 2 : ( c == '*' ? 3 : 0) ) )
#define isPerson(c) (c == '@' ? 1 : ( c == '+' ? 2 : 0) )
#define isBox(c) (c == '$' ? 1 : ( c == '*' ? 2 : 0 ) )

// unordered_set related
class PositionHash {
public:
	size_t operator () (const Position &a) const
	{
		return (a.x ^ (a.y << 1));
	}
};

class PositionEqual {
public:
	bool operator () (const Position &a, const Position &b) const
	{
		return a == b;
	}
};

class StateHash {
public:
	size_t operator () (const State &a) const
	{
		int h = a.person.x ^ (a.person.y << 1);
		for (int i = 0; i < a.box.size(); ++i) {
			h = h << 1;
			h ^= a.box[i].x ^ (a.box[i].y << 1);
		}
		return (h);
	}
};

class StateEqual {
public:
	bool operator () (const State &a, const State &b) const
	{
		if (a.person != b.person)
			return false;
		std::tr1::unordered_set<Position, PositionHash, PositionEqual>rec;
		for (int i = 0; i < a.box.size(); ++i) {
			rec.insert(a.box[i]);
		}
		for (int i = 0; i < b.box.size(); ++i)
			if (!rec.count(b.box[i]))
				return false;
		return true;
	}
};


typedef std::tr1::unordered_set<State, StateHash, StateEqual> StateSet;

// functions
void BFS(vector<string> ground);
void DFS(vector<string> ground);
int validState(int dx, int dy, State &now, vector<string> &ground);
void initState(vector<string> &ground, State &init);

#endif