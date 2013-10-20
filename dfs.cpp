#include "sokoban.h"

/**
 * recursive solver
 */
void dfs(vector<State> &stateVector, StateSet &rec, const vector<string> &ground, State state, int &flg, State &result)
{
	// got a result, return
	if (flg)
		return;
	for (int i = 0; i < 4 && !flg; ++i) {
		State now = state;
		now.person.x += direction[i][0]; 
		now.person.y += direction[i][1];
		int s = validState(direction[i][0], direction[i][1], now, ground);
		now.move = step[i];
		now.previousStateNum = now.currentStateNum;
		if (s == -1) {
			result = now;
			flg = 1;
			return;
		} else if (s && !rec.count(now)) {
			now.currentStateNum = stateVector.size();
			stateVector.push_back(now);
			rec.insert(now);
			dfs(stateVector, rec, ground, now, flg, result);
		}
	}
}

/**
 * dfs to the solution
 */
void DFS(const vector<string> &ground)
{
	State init;
	initState(ground, init);
	StateSet rec;
	vector<State>stateVector;
	stateVector.push_back(init);
	int flg = 0;
	State result;
	dfs(stateVector, rec, ground, init, flg, result);
	if (flg) {
		outputSolution(stateVector, result);
	}
}