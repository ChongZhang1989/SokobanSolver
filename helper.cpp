#include "sokoban.h"

/**
 * initialize the first state of the search
 */
void initState(const vector<string> &ground, State &init)
{
	// initialization
	for (int i = 0; i < ground.size(); ++i) {
		for (int j = 0; j < ground[i].size(); ++j) {
			if (isPerson(ground[i][j])) {
				init.person = Position(i, j);
			}
			if (isBox(ground[i][j])) {
				init.box.push_back(Position(i, j));
			}
		}
	}
}

/**
 * Test whether the box can be moved to this place
 */
int isValidBox(Position &box, State &now, const vector<string> &ground)
{
	// boundary check
	if (box.x >= ground.size() || box.x < 0)
		return 0;
	if (box.y >= ground[box.x].size() || box.y < 0)
		return 0;
	// is wall?
	if (isWall(ground[box.x][box.y])) {
		return 0;
	}
	// overlap with other boxes?
	for (int i = 0; i < now.box.size(); ++i) {
		if ((&box) == (&now.box[i]))
			continue;
		if (box == now.box[i])
			return 0;
	}
	return 1;
}

/**
 * Test whether the "now" state is valid
 */
int validState(int dx, int dy, State &now, const vector<string> &ground)
{
	// boundary check
	if (now.person.x >= ground.size() || now.person.x < 0)
		return 0;
	if (now.person.y >= ground[now.person.x].size() || now.person.y < 0)
		return 0;
	// is wall?
	if (isWall(ground[now.person.x][now.person.y]))
		return 0;
	// check boxes overlap with person
	int personOverlapBox = 1;
	for (int i = 0; i < now.box.size(); ++i) {
		if (now.box[i] == now.person) {
			personOverlapBox = 2;
			now.box[i].x += dx;
			now.box[i].y += dy;
			if (!isValidBox(now.box[i], now, ground)) {
				return 0;
			}
		}
	}
	// terminal state?
	for (int i = 0; i < now.box.size(); ++i) {
		if (!isGoal(ground[now.box[i].x][now.box[i].y])) {
			return personOverlapBox;
		}
	}
	return -1;
}

/**
 * output the solution of the solver
 */
void outputSolution(State &s)
{
	if (s.move.size() != 0) {
		printf("%c", s.move[0]);
	}
	for (int i = 1; i < s.move.size(); ++i) {
		printf(", %c", s.move[i]);
	}
	puts("");
}