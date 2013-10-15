#include "sokoban.h"
#define distance(a, b) (abs(a.x - b.x) + abs(a.y - b.y))

struct AState {
	int gcost;
	int hcost;
	State state;
	AState() {gcost = 0;}
	bool operator < (const AState &b) const
	{
		return (gcost + hcost) > (b.gcost + b.hcost);
	}
};

/**
 * evaluate the cost of the current state
 */
void evaluate(vector<Position> &goal, AState &s)
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
	s.hcost = cost;
}

/**
 * A Star search
 */
void AStar(const vector<string> &ground)
{
	StateSet rec;
	priority_queue<AState>q;
	vector<Position>goal;
	getGoalPosition(ground, goal);
	AState init;
	initState(ground, init.state);
	rec.insert(init.state);
	q.push(init);
	AState result;
	while (!q.empty()) {
		AState tmp = q.top();
		q.pop();
		for (int i = 0; i < 4; ++i) {
			AState now = tmp;
			now.state.person.x += direction[i][0];
			now.state.person.y += direction[i][1];
			int s = validState(direction[i][0], direction[i][1], now.state, ground);
			now.state.move.push_back(step[i]);
			if (s == -1) {
				result = now;
				goto end;
			} else if (s && !rec.count(now.state)) {
				evaluate(goal, now);
				now.gcost += s;
				q.push(now);
				rec.insert(now.state);
			}
		}
	}
end:
	outputSolution(result.state);
}