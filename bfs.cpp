#include "sokoban.h"

/**
 * BFS to find the solution
 */
void BFS(vector<string> ground)
{
	State init;
	queue<State>q;
	StateSet rec;
	// initialize the first state
	initState(ground, init);
	q.push(init);
	rec.insert(init);
	State terminal;
	while (!q.empty()) {
		for (int i = 0; i < 4; ++i) {
			State now = q.front();
			now.person.x += direction[i][0]; 
			now.person.y += direction[i][1];
			int s = validState(direction[i][0], direction[i][1], now, ground);
			if (s) {
				now.move.push_back(step[i]);
				if (s == -1) {
					terminal = now;
					goto end;
				}
				if (!rec.count(now)) {
					q.push(now);
					rec.insert(now);
				}
			}
		}
		q.pop();
	}
end:
	if (terminal.move.size() > 0)
		printf("%c", terminal.move[0]);
	for (int i = 1; i < terminal.move.size(); ++i) {
		printf(", %c ", terminal.move[i]);
	}
	puts("");
}