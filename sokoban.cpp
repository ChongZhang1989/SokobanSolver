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
	switch(choice) {
		case 1: BFS(ground); break;
		case 2: DFS(ground); break;
	}
	return 0;
}