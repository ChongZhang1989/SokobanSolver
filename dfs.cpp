#include "sokoban.h"

void dfs(StateSet &rec, vector<string> &ground, State state, int &flg, State &result)
{
	// got a result, return
	if (flg)
		return;
	for (int i = 0; i < 4 && !flg; ++i) {
		State now = state;
		now.person.x += direction[i][0]; 
		now.person.y += direction[i][1];
		int s = validState(direction[i][0], direction[i][1], now, ground);
		now.move.push_back(step[i]);
		if (s == -1) {
			result = now;
			flg = 1;
			return;
		} else if (s && !rec.count(now)) {
			rec.insert(now);
			dfs(rec, ground, now, flg, result);
		}
	}
}

/**
 * dfs to the solution
 */
void DFS(vector<string> ground)
{
	State init;
	initState(ground, init);
	StateSet rec;
	int flg = 0;
	State result;
	dfs(rec, ground, init, flg, result);
	if (flg) {
		printf("%c", result.move[0]);
		for (int i = 1; i < result.move.size(); ++i) {
			printf(", %c", result.move[i]);
		}
		puts("");
	}
}