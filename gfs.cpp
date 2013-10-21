#include "sokoban.h"
#define distance(a, b) (fabs(a.x - b.x) + fabs(a.y - b.y))

struct GFSState {
	int cost;
	State state;
	GFSState() {}
	GFSState(int c) : cost(c) {}
	bool operator < (const GFSState &b) const
	{
		return cost > b.cost;
	}
};

/**
 * evaluate the cost of the current state
 */
void evaluate(vector<Position> &goal, GFSState &s)
{
	int cost = 0;
	vector<bool>v(s.state.box.size(), false);
	for (int i = 0; i < goal.size(); ++i) {
		int mindis = 0x7fffffff;
		int p = 0;
		for (int j = 0; j < s.state.box.size(); ++j) {
			if (!v[j] && mindis > distance(goal[i], s.state.box[j])) {
				mindis = distance(goal[i], s.state.box[j]);
				p = j;
			}
		}
		v[p] = true;
		cost += mindis;
	}
	s.cost = cost;
}

/**
 * greedy best first search
 */
void GFS(const vector<string> &ground)
{
	int time1 = clock();
	Statistics stat;
	StateSet rec;
	priority_queue<GFSState>q;
	vector<Position>goal;
	vector<State>stateVector;
	getGoalPosition(ground, goal);
	GFSState init(0);
	initState(ground, init.state);
	rec.insert(init.state);
	q.push(init);
	stateVector.push_back(init.state);
	GFSState result;
	while (!q.empty()) {
		GFSState tmp = q.top();
		q.pop();
		for (int i = 0; i < 4; ++i) {
			GFSState now = tmp;
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
				now.state.currentStateNum = stateVector.size();
				evaluate(goal, now);
				q.push(now);
				stateVector.push_back(now.state);
				rec.insert(now.state);
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