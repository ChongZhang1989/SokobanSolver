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
int main()
{
	vector<string>ground;
	getInput(ground);
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