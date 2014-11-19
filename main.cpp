// Project 5b - Local Search for graph coloring

#include <iostream>
#include <fstream>
#include <set>
#include <random>
#include <time.h>

#include "headers/graph.h"

using namespace std;

void localSearch2opt(graph &g, int maxTime);
void localSearch3opt(graph &g, int maxTime);


void randomColoring(graph &g);
void randomSupervisor(graph &g, void (*searchType)(knapsack&k, int time), int maxTime);

void neighborhood(graph &g, set<int> idx);

void greedyColoring(graph &g);
bool isValidColor(graph g, int n, int c);
void printColorSolution(graph g);
int getNumConflicts(graph g);

int main()
{

	ifstream fin;
    string fileName = "C:\\Users\\Boris\\Documents\\Visual Studio 2010\\Projects\\caimat-5b\\caimat-4a\\instances\\color12-3.input";
	int algorithmOption = 1;

	cout << "Testing File : " << fileName << endl;

    fin.open(fileName.c_str());
    if (!fin)
    {
        cerr << "Cannot open " << fileName << endl;
        return 1;
    }

    try
    {
        cout << "Reading knapsack instance" << endl;
        graph g(fin);

        switch (algorithmOption)
        {
            case 1:
                cout << "Greedy - Local Search 2 opt" << endl;
                greedyColoring(g);
				localSearch2opt(g, 60);
                break;
/*            case 2:
                cout << "Random - Local Search Big" << endl;
                randomSupervisor(k, localSearchBig, 600);
                break;
            case 3:
                cout << "Greedy - Local Search Small" << endl;
                greedyKnapsack(k);
                localSearchSmall(k, 600);
                break;
            case 4:
                cout << "Greedy - Local Search Big" << endl;
                greedyKnapsack(k);
                localSearchBig(k, 600);
                break;	*/
            default:
                cout << "Failure reading option. Please enter a number 1-4\n;
                cout << "1) Random Local Search Small" << endl;
                cout << "2) Random Local Search Big" << endl;
                cout << "3) Greedy Local Search Small" << endl;
                cout << "4) Greedy Local Search Big" << endl;
                break;
        }

        cout << endl << "Best solution" << endl;
        printColorSolution(graph g)
    }


    catch (indexRangeError &ex)
    {
        cout << ex.what() << endl; return 1;
    }
    catch (rangeError &ex)
    {
        cout << ex.what() << endl; return 1;
    }

	system("pause");
	return 0;
}


void localSearch2opt(graph &g, int maTime)
{
	graph champion = knapsack(k);

    time_t endTime = clock() + ( maxTime * 1000 );

    // Loop through the nodes, and find least bad solution for each
    for (int i = 0; i < g.getN && endTime > clock(); i++)
    {
		for (int j = 0; i < g.getN && endTime > clock(); i++)
		{
			if(i == j)
				continue;
			// Go through all possible colors and select the one that produces the least conflicts
			int lowestConflicts = 99999; // pre set to infinity
			int lowestColor1 = 1;
			int lowestColor2 = 1;

			// Greedily color node with color that creates minimum conflicts
			for(int currColor1 = 1; currColor < g.numColors(); currColor++)
			{
				for(int currColor2 = 1; currColor < g.numColors(); currColor++)
				{
					g.setNodeWeight(i, currColor1);
					g.setNodeWeight(j, currColor2);
					if(getNumConflicts(g) < lowestConflicts)
					{
						lowestConflict = getNumConflicts(g);
						lowestColor = currColor;
					}
				}
			}
			g.setNodeWeight(currNode, lowestColor); 

			// Replace champion if this solution is better
			if (getNumConflicts(g) < getNumConflicts(champion)) {
				champion = graph(g);
			} 
    }

    // Save the champion and return him
    g = champion;
}






void greedyColoring(graph &g)
// Greedy algorithm that finds minimum number of conflicts for a
// numColors coloring of graph g by iteratively going trough all
// nodes and choosing the lowest color that does not conflict with 
// neighboring nodes.
{
	// Get the total number of nodes
    int nodeCount = g.numNodes();

	// Go through and color all nodes
	for(int currNode = 0; currNode < nodeCount; currNode++)
	{
		// Go through all possible colors and select the one that produces the least conflicts
		int lowestConflicts = 99999; // pre set to infinity
		int lowestColor = 1;

		// Greedily color node with color that creates minimum conflicts
		for(int currColor = 1; currColor < g.numColors(); currColor++)
		{
			g.setNodeWeight(currNode, currColor);
			if(getNumConflicts(g) < lowestConflicts)
			{
				lowestConflict = getNumConflicts(g);
				lowestColor = currColor;
			}
		}
		g.setNodeWeight(currNode, lowestColor);

		}
	}
}
 
bool isValidColor(graph g, int n, int c)
// Check if node n has any neighbors of color c
// Return value of true means no neighbors of color c currently exist
{
	bool status = true;
	int neighborColor;
	int numNodes = g.numNodes();

	// Iterate through all nodes, checking for neighbors
	for (int i = 0; i < numNodes; i++)
    {		
		// Check if nodes have conflicting color, because of 
		// how the graph is constructed we only need to check in one 
		// direction, from smaller to larger node
		if(g.isEdge(i, n))
		{
			neighborColor = g.getNodeWeight(i);
			if(neighborColor == c)
			{
				status = false;
				return status;
			}
		}
	}

	return status;
}
	
void printColorSolution(graph g)
// print color of each node and total number of colors used
{
	int numNodes = g.numNodes();
	int numColors = 0;
	int currColor;
	int biggestColor = 0;

	cout<<"\nSolution for greedy coloring of this graph:\n\n";

	for(int i = 0; i < numNodes; i++)
	{
		currColor = g.getNodeWeight(i);
		cout<<"Color of node "<<i<<": "<<currColor<<"\n";
		if (currColor > biggestColor)
			biggestColor = currColor;
	}

	cout<<"\nA total of " << biggestColor<< " colors were used.\n\n";
}

int getNumConflicts(graph g)
// Get the total number of conflicts on our graph.
{
    int numConflicts = 0;

	// Iterate through all nodes
    for (int i = 0; i < g.numNodes(); i++)
    {
		// Checking for edges, because of the way the graph is constructed
		// we only need to check in one direction, from smaller to larger node
        for (int j = 0; j < g.numNodes(); j++)
        {
            // If neighboring nodes have same color, add a conflict
            if (g.isEdge(i, j))
            {
                if (g.getNode(i).getWeight() == g.getNode(j).getWeight())
					numConflicts++;
			}
		}
    }
    return numConflicts;
}









