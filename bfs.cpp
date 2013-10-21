#include "sokoban.h"

/**
 * BFS to find the solution
 */
void BFS(const vector<string> &ground)
{
	int time1 = clock();
	Statistics stat;
	State init;
	queue<State>q;
	vector<State>stateVector;
	StateSet rec;
	// initialize the first state
	initState(ground, init);
	q.push(init);
	rec.insert(init);
	stateVector.push_back(init);
	State terminal;
	while (!q.empty()) {
		for (int i = 0; i < 4; ++i) {
			State now = q.front();
			now.person.x += direction[i][0];
			now.person.y += direction[i][1];
			int s = validState(direction[i][0], direction[i][1], now, ground);
			now.move = step[i];
			now.previousStateNum = now.currentStateNum;
			stat.anodes++;
			if (s == -1) {
				terminal = now;
				goto end;
			} else if (s && !rec.count(now)) {
				now.currentStateNum = stateVector.size();
				q.push(now);
				stateVector.push_back(now);
				rec.insert(now);
			} else if (s) {
				stat.bnodes++;
			}
		}
		q.pop();
	}
end:
	stat.dnodes = rec.size() + 1;
	stat.cnodes = q.size();
	stat.runtime = (clock() - time1) * 1.0 / CLOCKS_PER_SEC;
	outputStat(stat);
	outputSolution(stateVector, terminal);
}