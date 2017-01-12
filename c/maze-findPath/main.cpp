/* 

Gonul AYCI
Bogazici University, January 2017

*/

#include <vector>
#include <iostream>
#include <fstream>

#define GRID_WIDTH 22
#define GRID_HEIGHT 10
#define WALK '0' 
#define WALL '1' 
using namespace std; //for vector, and cout

struct Node
{
	int rowNumber, colnNumber, f, g, h; //f, g, h are the elements of A-star search algorithm
	bool isWalkable;
	vector<Node*> connectedList;
	Node* parent;
	char asciiChr;

	void doControl(int row, int coln, bool walkable){

		isWalkable = walkable;
		rowNumber = row;
		colnNumber = coln;
		parent = NULL;
	} 	
	
};

Node mazeNode[GRID_HEIGHT][GRID_WIDTH], *startNode = NULL, *endNode = NULL;

// openList for unvisited nodes, closedList for visited nodes.
//pop from openList then push it to closedList
vector<Node*> openList, closedList;

//create a maze
char sampleMaze[GRID_HEIGHT][GRID_WIDTH];

//read from a file
void createMazeFromFile(char* FileName){
	fstream file;
	file.open(FileName);
	int counter = 0;

	if (file.is_open())
	{
		while(counter < max(GRID_WIDTH, GRID_HEIGHT)){
			file.getline(sampleMaze[counter], GRID_WIDTH +1);
			counter++;
		}
	}

}

//directions of path
void directionOfPathNodes(Node* node){
	if (!node -> isWalkable)
		return;

	if(node -> rowNumber >= 1 && mazeNode[node -> rowNumber][node -> colnNumber -1].isWalkable) //left
		node -> connectedList.push_back(&mazeNode[node -> rowNumber][node -> colnNumber -1]);

	if(node -> rowNumber >= 1 && mazeNode[node -> rowNumber -1][node -> colnNumber].isWalkable) //up
		node -> connectedList.push_back(&mazeNode[node -> rowNumber -1][node -> colnNumber]);

	if(node -> colnNumber < GRID_WIDTH -1 && mazeNode[node -> rowNumber][node -> colnNumber +1].isWalkable) //right
		node -> connectedList.push_back(&mazeNode[node -> rowNumber][node -> colnNumber +1]);

	if(node -> rowNumber < GRID_HEIGHT -1 && mazeNode[node -> rowNumber +1][node -> colnNumber].isWalkable) //down
		node -> connectedList.push_back(&mazeNode[node -> rowNumber +1][node -> colnNumber]);

}

void initializeNodes(){

	for (int i = 0; i < GRID_HEIGHT; ++i)
	{
       for (int j = 0; j < GRID_WIDTH; ++j)
       {
		
          mazeNode[i][j].asciiChr = sampleMaze[i][j];
		  int asciiChr = (int)mazeNode[i][j].asciiChr;
		  
		  mazeNode[i][j].doControl(i, j, asciiChr != WALL);
		  if (asciiChr == 'A')
			startNode = &mazeNode[i][j];
          else if (asciiChr == 'F')
      		endNode = &mazeNode[i][j];
               
       }
	}

	if(startNode == NULL || endNode == NULL)
	{
		printf("No Path Found");
		exit(0);
	}

	for (int i = 0; i < GRID_HEIGHT; ++i)
	{
		for (int j = 0; j < GRID_WIDTH; ++j)
		{
			directionOfPathNodes(&mazeNode[i][j]);
		}
	}

}

//A-star search algorithm: selects the path that minimizes f(n) = g(n) + h(n)
//g(n) is the cost of the path from the start node to n
//h(n) is a heuristic that estimates the cost of the cheapest path n to the goal
void UpdateScore(Node* start, Node* end)
    {
            start -> g = start -> parent -> g + 1;
			start -> h = abs(start -> rowNumber - end -> rowNumber) + abs(start -> colnNumber - end -> colnNumber);
          	start -> f = start -> g + start -> h;
    
	}

bool findPath(Node* start, Node* end){

	if (!start -> isWalkable)
	{
		return false;
	}

	bool found = false;

	openList.push_back(start);
	while(true){
		if (openList.empty())
			break;

		Node* bestNode = openList.back();
		openList.pop_back();
		closedList.push_back(bestNode);

		if (bestNode == end)
		{
			found = true;
			break;
		}

		vector<Node*>::iterator iter;
		for (int i = 0; i < bestNode -> connectedList.size(); ++i)
		{
			if (find(closedList.begin(), closedList.end(), bestNode -> connectedList[i]) != closedList.end())
				continue;
			
			iter = find(openList.begin(), openList.end(), bestNode -> connectedList[i]);
			if(iter != openList.end())
			{
				
				openList[distance(openList.begin(), iter)] -> parent = bestNode;
				
				UpdateScore(openList[distance(openList.begin(), iter)], end);
								
			}
			else
			{
				bestNode -> connectedList[i] -> parent = bestNode;
				UpdateScore(bestNode -> connectedList[i], end);
				openList.push_back(bestNode -> connectedList[i]);
				
			}
		}
		
	}
	return found;
}

//count the number of steps of connected nodes
void tracePath()
{
	Node* trace = endNode;
	trace = trace -> parent;
	int number_of_steps = 1;
	while(trace != startNode)
	{
		number_of_steps++;
		sampleMaze[trace -> rowNumber][trace->colnNumber] = 'P';
		trace = trace -> parent;
	}

	for(int i = 0; i < GRID_HEIGHT; ++i)
	{
		for(int j = 0; j < GRID_WIDTH; ++j)

			cout<<sampleMaze[i][j]<< " ";
		cout<< "\n";
	}

printf("No of Steps = %d\n", number_of_steps, "\n");
}

int main(int argc, char const *argv[])
{
	createMazeFromFile("map.txt");

	initializeNodes();

	if(findPath(startNode,endNode))
		tracePath();

	return 0;

}



