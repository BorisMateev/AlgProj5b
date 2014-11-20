// Project 5b - Local Search for graph coloring

#include <iostream>
#include <fstream>
#include <set>
#include <random>
#include <time.h>

#include "headers/graph.h"

using namespace std;

void localSearch2opt(graph &g, int numColors, int maxTime);
void localSearch3opt(graph &g, int numColors, int maxTime);


void randomColoring(graph &g);
void randomSupervisor(graph &g, void (*searchType)(graph&g, int time), int maxTime);

void neighborhood(graph &g, set<int> idx);

void greedyColoring(graph &g, int numColors);
bool isValidColor(graph g, int n, int c);
void printColorSolution(graph g);
int getNumConflicts(graph g);

int main()
{

	ifstream fin;
	int numColors;
    string fileName = "C:\\Users\\Boris\\Documents\\Visual Studio 2010\\Projects\\caimat-5b\\caimat-5b\\instances\\color12-3.input";
	int algorithmOption = 2;

	cout << "Testing File : " << fileName << endl;

    fin.open(fileName.c_str());
    if (!fin)
    {
        cerr << "Cannot open " << fileName << endl;
        return 1;
    }

    try
    {
		 cout << "Reading number of colors" << endl;
        fin >> numColors;

        cout << "Reading knapsack instance" << endl;
        graph g(fin);

		cout << "Num colors: " << numColors << endl;
        cout << g;

        switch (algorithmOption)
        {
            case 1:
                cout << "Greedy - Local Search 2 opt" << endl;
                greedyColoring(g, numColors);
				localSearch2opt(g, numColors, 60);
                break;
            case 2:
                cout << "Greedy - Local Search 3opt" << endl;
                greedyColoring(g, numColors);
				localSearch3opt(g, numColors, 60);
                break;
/*            case 3:
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
                cout << "Failure reading option. Please enter a number 1-4\n";
                cout << "1) Random Local Search Small" << endl;
                cout << "2) Random Local Search Big" << endl;
                cout << "3) Greedy Local Search Small" << endl;
                cout << "4) Greedy Local Search Big" << endl;
                break;
        }

        cout << endl << "Best solution" << endl;
        printColorSolution(g);
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


void localSearch2opt(graph &g, int numColors, int maxTime)
{
	graph champion = graph(g);

    time_t endTime = clock() + ( maxTime * 1000 );

    // Loop through the nodes, and find least bad solution for each
    for (int i = 0; (i < g.numNodes()) /*&& endTime > clock()*/; i++)
    {
		for (int j = 0; (j < g.numNodes()) && endTime > clock(); j++)
		{
			if(i == j)
				continue;
			
			// Go through all possible colors and select the one that produces the least conflicts
			int lowestConflicts = 99999; // pre set to infinity
			int lowestColorI = 0;
			int lowestColorJ = 0;
			
			// Greedily color node with color that creates minimum conflicts
			for(int currColorI = 0; currColorI < numColors; currColorI++)
			{
				for(int currColorJ = 0; currColorJ < numColors; currColorJ++)
				{
					g.setNodeWeight(i, currColorI);
					g.setNodeWeight(j, currColorJ);
					if(getNumConflicts(g) < lowestConflicts)
					{
						lowestConflicts = getNumConflicts(g);
						lowestColorI = currColorI;
						lowestColorJ = currColorJ;
					}
				}
			}
			g.setNodeWeight(i, lowestColorI); 
			g.setNodeWeight(j, lowestColorJ);

			// Replace champion if this solution is better
			if (getNumConflicts(g) < getNumConflicts(champion)) {
				champion = graph(g);
			} 
		}
	}
    // Save the champion and return him
    g = champion;	
}

void localSearch3opt(graph &g, int numColors, int maxTime)
{
	graph champion = graph(g);

    time_t endTime = clock() + ( maxTime * 1000 );

    // Loop through the nodes, and find least bad solution for each
    for (int i = 0; (i < g.numNodes()) && endTime > clock(); i++)
    {
		for (int j = 0; (j < g.numNodes()) && endTime > clock(); j++)
		{
			for (int k = 0; (k < g.numNodes()) && endTime > clock(); k++)
			{
				if(i == j || i == k || j == k)
					continue;
			
				// Go through all possible colors and select the one that produces the least conflicts
				int lowestConflicts = 99999; // pre set to infinity
				int lowestColorI = 0;
				int lowestColorJ = 0;
				int lowestColorK = 0;
			
				// Greedily color node with color that creates minimum conflicts
				for(int currColorI = 0; currColorI < numColors; currColorI++)
				{
					for(int currColorJ = 0; currColorJ < numColors; currColorJ++)
					{
						for(int currColorK = 0; currColorK < numColors; currColorK++)
						{
							g.setNodeWeight(i, currColorI);
							g.setNodeWeight(j, currColorJ);
							g.setNodeWeight(k, currColorK);

							if(getNumConflicts(g) < lowestConflicts)
							{
								lowestConflicts = getNumConflicts(g);
								lowestColorI = currColorI;
								lowestColorJ = currColorJ;
								lowestColorK = currColorK;
							}
						}
					}
					g.setNodeWeight(i, lowestColorI); 
					g.setNodeWeight(j, lowestColorJ);
					g.setNodeWeight(k, lowestColorK);

					// Replace champion if this solution is better
					if (getNumConflicts(g) < getNumConflicts(champion))
					{
						champion = graph(g);
					} 
				}
			}
		
		}
	}
	// Save the champion to return him
		g = champion;	
}

void greedyColoring(graph &g, int numColors)
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
		int lowestColor = 0;

		// Greedily color node with color that creates minimum conflicts
		for(int currColor = 0; currColor < numColors; currColor++)
		{
			g.setNodeWeight(currNode, currColor);
			if(getNumConflicts(g) < lowestConflicts)
			{
				lowestConflicts = getNumConflicts(g);
				lowestColor = currColor;
			}
		}
		g.setNodeWeight(currNode, lowestColor);

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

	cout<<"\nSolution for local search of this graph:\n\n";
	cout<<g;
	cout<<"Number of conflicts detected: "<< getNumConflicts(g)<<endl;
	
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









