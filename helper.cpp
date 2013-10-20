#include "sokoban.h"
int horizontalFreeze(int i, vector<int> &hfreeze, vector<int> &vfreeze, State &now, const vector<string> &ground);
int verticalFreeze(int i, vector<int> &hfreeze, vector<int> &vfreeze, State &now, const vector<string> &ground);

/**
 * judge if p is out of boundary
 */
int outOfBoundary(const vector<string> &ground, Position p)
{
	if (p.x >= ground.size() || p.x < 0)
		return 1;
	if (p.y >= ground[p.x].size() || p.y < 0)
		return 1;
	return 0;
}

/**
 * initialize the first state of the search
 */
void initState(const vector<string> &ground, State &init)
{
	// initialization
	init.currentStateNum = 0;
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

int isBox2(State &now, int x, int y)
{
	for (int i = 0; i < now.box.size(); ++i)
		if (now.box[i].x == x && now.box[i].y == y)
			return i;
	return -1;
}

int horizontalFreeze(int i, vector<int> &hfreeze, vector<int> &vfreeze, State &now, const vector<string> &ground)
{
	if (hfreeze[i] >= 0)
		return hfreeze[i];
	hfreeze[i] = -1;
	int x = now.box[i].x;
	int y = now.box[i].y;
	// if left side or right side is wall
	if ( (outOfBoundary(ground, Position(x, y - 1)) || isWall(ground[x][y - 1]) ) ||
		 (outOfBoundary(ground, Position(x, y + 1)) || isWall(ground[x][y + 1]) ) ) {
		return hfreeze[i] = 0;
	}
	// if left side and right side are dead squares
	if (isDeadSquare(ground[x][y - 1]) && isDeadSquare(ground[x][y + 1])) {
		return hfreeze[i] = 0;
	}
	// if left side or right side is box?
	int b = isBox2(now, x, y - 1);
	if (b != -1) {
		if (vfreeze[b] == 0 || vfreeze[b] == -1)
			return hfreeze[i] = 0;
		if (vfreeze[b] == -2) {
			if (verticalFreeze(b, hfreeze, vfreeze, now, ground) == 0) {
				return hfreeze[i] = 0;
			}
		}
	}
	b = isBox2(now, x, y + 1);
	if (b != -1) {
		if (x == 6 && y == 11)
		if (vfreeze[b] == 0 || vfreeze[b] == -1)
			return hfreeze[i] = 0;
		if (vfreeze[b] == -2) {
			if (verticalFreeze(b, hfreeze, vfreeze, now, ground) == 0) {
				return hfreeze[i] = 0;
			}
		}
	}
	return hfreeze[i] = 1;
}

int verticalFreeze(int i, vector<int> &hfreeze, vector<int> &vfreeze, State &now, const vector<string> &ground)
{
	if (vfreeze[i] >= 0)
		return vfreeze[i];
	vfreeze[i] = -1;
	int x = now.box[i].x;
	int y = now.box[i].y;
	// if left side or right side is wall
	if ( (outOfBoundary(ground, Position(x - 1, y)) || isWall(ground[x - 1][y]) ) ||
		 (outOfBoundary(ground, Position(x + 1, y)) || isWall(ground[x + 1][y]) ) ) {
		return vfreeze[i] = 0;
	}
	// if left side and right side are dead squares
	if (isDeadSquare(ground[x - 1][y]) && isDeadSquare(ground[x + 1][y])) {
		return vfreeze[i] = 0;
	}
	// if left side or right side is box?
	int b = isBox2(now, x - 1, y);	
	if (b != -1) {
		if (hfreeze[b] == 0 || hfreeze[b] == -1) {
			return vfreeze[i] = 0;
		}
		if (hfreeze[b] == -2) {
			if (horizontalFreeze(b, hfreeze, vfreeze, now, ground) == 0) {
				return vfreeze[i] = 0;
			}
		}
	}
	b = isBox2(now, x + 1, y);
	if (b != -1) {
		if (hfreeze[b] == 0 || hfreeze[b] == -1) {
			return vfreeze[i] = 0;
		}
		if (hfreeze[b] == -2) {
			if (horizontalFreeze(b, hfreeze, vfreeze, now, ground) == 0) {
				return vfreeze[i] = 0;
			}
		}
	}
	return vfreeze[i] = 1;
}


/**
 * judge if the current state is freeze deadlock
 */
int isFreezeDeadlock(State &now, const vector<string> &ground)
{
	vector<int>hfreeze(now.box.size(), -2);
	vector<int>vfreeze(now.box.size(), -2);
	for (int i = 0; i < now.box.size(); ++i) {
		horizontalFreeze(i, hfreeze, vfreeze, now, ground);
		verticalFreeze(i, hfreeze, vfreeze, now, ground);
	}
	for (int i = 0; i < hfreeze.size(); ++i) {
		if (hfreeze[i] == 0 && vfreeze[i] == 0 && !isGoal(ground[now.box[i].x][now.box[i].y])) {
			return 1;
		}
	}
	return 0;
}

/**
 * judge if the current state is a deadlock
 */
int isdeadlock(Position &box, State &now, const vector<string> &ground)
{
	if (isDeadSquare(ground[box.x][box.y]))
		return 1;
	if (isFreezeDeadlock(now, ground))
		return 1;
	return 0;
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
	if (isdeadlock(box, now, ground))
		return 0;
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
			if (!isValidBox(now.box[i], now, ground))
				return 0;
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
void outputSolution(vector<State> &stateVector, State &s)
{
	if (s.currentStateNum == -1 || s.previousStateNum == -1)
		return;
	stack<char>move;
	move.push(s.move);
	int statenum = s.previousStateNum;
	while (statenum) {
		move.push(stateVector[statenum].move);
		statenum = stateVector[statenum].previousStateNum;
	}
	printf("%c", move.top());
	move.pop();
	while (!move.empty()) {
		printf(", %c", move.top());
		move.pop();
	}
	puts("");
}

/**
 * get the goal positions
 */
void getGoalPosition(const vector<string> &ground, vector<Position> &goal)
{
	for (int i = 0; i < ground.size(); ++i) {
		for (int j = 0; j < ground[i].size(); ++j) {
			if (isGoal(ground[i][j])) {
				goal.push_back(Position(i, j));
			}
		}
	}
}