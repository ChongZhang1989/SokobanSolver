#include "sokoban.h"
#include <fstream>
void getInput(vector<string> &ground)
{
	fstream fs;
	fs.open("in.txt", fstream::in);
	int n;
	fs>>n;
	string t;
	getline(fs, t);
	for (int i = 0; i < n; ++i) {
		getline(fs, t);
		ground.push_back(t);
	}
	fs.close();
}


/**
 * mark all places that are reachable
 */
void pullAndMark(vector<string> &mark, Position b, Position p)
{
	if (outOfBoundary(mark, p) || outOfBoundary(mark, b)) {
		return;
	}
	if (mark[b.x][b.y] == 'V')
		return;
	if (isWall(mark[b.x][b.y]) || isWall(mark[p.x][p.y]))
		return;
	mark[b.x][b.y] = 'V';
	for (int i = 0; i < 4; ++i) {
		Position newbox(b.x + direction[i][0], b.y + direction[i][1]);
		Position newperson(newbox.x + direction[i][0], newbox.y + direction[i][1]);
		pullAndMark(mark, newbox, newperson);
	}
}

/**
 * detecting simple deadlocks
 */
void detectDeadSquare(vector<string> &ground)
{
	vector<Position> goal;
	getGoalPosition(ground, goal);
	vector<string> mark = ground;
	for (int i = 0; i < goal.size(); ++i) {
		for (int j = 0; j < 4; ++j) {
			Position person(goal[i].x  + direction[j][0], goal[i].y + direction[j][1]);
			pullAndMark(mark, goal[i], person);
		}
	}
	for (int i = 0; i < ground.size(); ++i) {
		for (int j = 0; j < ground[i].size(); ++j) {
			if (!isWall(ground[i][j]) && !isPerson(ground[i][j]) && !isBox(ground[i][j]) && mark[i][j] != 'V') {
				ground[i][j] = 'X';
			}
		}
	}
}
void showGround(vector<string> &ground)
{
	for (int i = 0; i < ground.size(); ++i) {
		cout<<ground[i]<<endl;
	}
}
int main()
{
	vector<string>ground;
	getInput(ground);
	detectDeadSquare(ground);
	//showGround(ground);
	puts("1) Breadth first search");
	puts("2) Depth first search");
	puts("3) Uniform cost search");
	puts("4) Greedy best first search");
	puts("5) A* search");
	int choice;
	cin>>choice;
	int t1 = clock();
	switch(choice) {
		case 1: BFS(ground); break;
		case 2: DFS(ground); break;
		case 3: UCS(ground); break;
		case 4: GFS(ground); break;
		case 5: AStar(ground); break;
	}
	int t2 = clock();
	printf("Running Time : %.3lf\n", (t2 - t1) * 1.0 / CLOCKS_PER_SEC);
	return 0;
}