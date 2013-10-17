#ifndef __Sokoban
#include <iostream>
#include <stdio.h>
#include <vector>
#include <queue>
#include <map>
#include <time.h>
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

#define isWall(c) (c == '#' ? 1 : 0)
#define isGoal(c) (c == '.' ? 1 : ( c == '+' ? 2 : ( c == '*' ? 3 : 0) ) )
#define isPerson(c) (c == '@' ? 1 : ( c == '+' ? 2 : 0) )
#define isBox(c) (c == '$' ? 1 : ( c == '*' ? 2 : 0 ) )
#define isDeadSquare(c) (c == 'X' ? 1 : 0)

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

typedef std::tr1::unordered_set<Position, PositionHash, PositionEqual> PositionSet;

class StateHash {
public:
	size_t operator () (const State &a) const
	{
		PositionSet rec;
		for (int i = 0; i < a.box.size(); ++i) {
			rec.insert(a.box[i]);
		}
		int h = a.person.x ^ (a.person.y << 1);
		for (PositionSet::iterator it = rec.begin(); it != rec.end(); ++it) {
			h <<= 1;
			h ^= it->x ^ ((it->y) << 1);
		}
		return h;
	}
};

class StateEqual {
public:
	bool operator () (const State &a, const State &b) const
	{
		if (a.person != b.person)
			return false;
		PositionSet rec;
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
void BFS(const vector<string> &ground);
void DFS(const vector<string> &ground);
void UCS(const vector<string> &ground);
void GFS(const vector<string> &ground);
void AStar(const vector<string> &ground);
int validState(int dx, int dy, State &now, const vector<string> &ground);
void initState(const vector<string> &ground, State &init);
void outputSolution(State &s);
void getGoalPosition(const vector<string> &ground, vector<Position> &goal);
int outOfBoundary(const vector<string> &ground, Position p);

#endif