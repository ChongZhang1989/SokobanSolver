#include "sokoban.h"

/**
 * recursive solver (useless）
 */
void dfs(Statistics &stat, vector<State> &stateVector, StateSet &rec, const vector<string> &ground, State state, int &flg, State &result)
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
		stat.anodes++;
		if (s == -1) {
			result = now;
			flg = 1;
			return;
		} else if (s && !rec.count(now)) {
			now.currentStateNum = stateVector.size();
			stateVector.push_back(now);
			rec.insert(now);
			dfs(stat, stateVector, rec, ground, now, flg, result);
		} else if (s) {
			stat.bnodes++;
		}
	}
}

/**
 * dfs to the solution
 */
void DFS(const vector<string> &ground)
{
	int time1 = clock();
	Statistics stat;
	State init;
	initState(ground, init);
	StateSet rec;
	vector<State>stateVector;
	stateVector.push_back(init);
	int flg = 0;
	State result;
	stack<State>st;
	st.push(init);
	while (!st.empty()) {
		State tmp = st.top();
		st.pop();
		for (int i = 0; i < 4; ++i) {
			State now = tmp;
			now.person.x += direction[i][0];
			now.person.y += direction[i][1];
			int s = validState(direction[i][0], direction[i][1], now, ground);
			now.move = step[i];
			now.previousStateNum = now.currentStateNum;
			stat.anodes++;
			if (s == -1) {
				result = now;
				goto end;
			} else if (s && !rec.count(now)) {
				now.currentStateNum = stateVector.size();
				st.push(now);
				stateVector.push_back(now);
				rec.insert(now);
			} else if (s) {
				stat.bnodes++;
			}
		}
	}
end:
	stat.cnodes = st.size();
	stat.dnodes = rec.size() + 1;
	stat.runtime = (clock() - time1) * 1.0 / CLOCKS_PER_SEC;
	outputStat(stat);
	outputSolution(stateVector, result);
}