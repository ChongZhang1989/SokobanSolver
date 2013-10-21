#include "sokoban.h"

struct UCSState {
	int cost;
	State state;
	UCSState() {}
	UCSState(int c) : cost(c) {}
	bool operator < (const UCSState &b) const
	{
		return cost > b.cost;
	}
};

/**
 * Uniform Cost Search
 */
void UCS(const vector<string> &ground)
{
	int time1 = clock();
	Statistics stat;
	priority_queue<UCSState>q;
	StateSet rec;
	UCSState init(0);
	initState(ground, init.state);
	vector<State>stateVector;
	q.push(init);
	stateVector.push_back(init.state);
	rec.insert(init.state);
	UCSState result;
	while (!q.empty()) {
		UCSState tmp = q.top();
		q.pop();
		for (int i = 0; i < 4; ++i) {
			UCSState now = tmp;
			now.state.person.x += direction[i][0];
			now.state.person.y += direction[i][1];
			int s = validState(direction[i][0], direction[i][1], now.state, ground);
			now.state.move = step[i];
			now.state.previousStateNum = now.state.currentStateNum;
			stat.anodes++;
			if (s == -1) {
				result = now;
				goto end;
			} else if (s && !rec.count(now.state)) {
				now.cost += s;
				now.state.currentStateNum = stateVector.size();
				stateVector.push_back(now.state);
				rec.insert(now.state);
				q.push(now);
			} else if (s) {
				stat.bnodes++;
			}
		}
	}
end:
	stat.cnodes = q.size();
	stat.dnodes = rec.size() + 1;
	stat.runtime = (clock() - time1) * 1.0 / CLOCKS_PER_SEC;
	outputStat(stat);
	outputSolution(stateVector, result.state);
}