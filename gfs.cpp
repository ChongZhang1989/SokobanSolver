#include "sokoban.h"
#define distance(a, b) (abs(a.x - b.x) + abs(a.y - b.y))

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
	for (int i = 0; i < goal.size(); ++i) {
		for (int j = 0; j < s.state.box.size(); ++j) {
			cost += distance(goal[i], s.state.box[j]);
		}
	}
	s.cost = cost;
}

/**
 * greedy best first search
 */
void GFS(const vector<string> &ground)
{
	StateSet rec;
	priority_queue<GFSState>q;
	vector<Position>goal;
	getGoalPosition(ground, goal);
	GFSState init(0);
	initState(ground, init.state);
	rec.insert(init.state);
	q.push(init);
	GFSState result;
	while (!q.empty()) {
		GFSState tmp = q.top();
		q.pop();
		for (int i = 0; i < 4; ++i) {
			GFSState now = tmp;
			now.state.person.x += direction[i][0];
			now.state.person.y += direction[i][1];
			int s = validState(direction[i][0], direction[i][1], now.state, ground);
			now.state.move.push_back(step[i]);
			if (s == -1) {
				result = now;
				goto end;
			} else if (s && !rec.count(now.state)) {
				evaluate(goal, now);
				q.push(now);
				rec.insert(now.state);
			}
		}
	}
end:
	outputSolution(result.state);
}