// Project 5b - Local Search for graph coloring

#include <iostream>
#include <fstream>
#include <set>
#include <random>
#include <time.h>
#include <stdint.h>

#include "headers/graph.h"

using namespace std;

void localSearch2opt(graph &g, int numColors, int maxTime);
void localSearch3opt(graph &g, int numColors, int maxTime);

void randomSupervisor(graph &g, void (*searchType)(graph&g, int numColors, int maxTime), int numColors, int maxTime);

void greedyColoring(graph &g, int numColors);
void randomColoring(graph &g, int numColors);


void clearColoring(graph &g);
void printColorSolution(graph g);
int getNumConflicts(graph g);


int main()
{

    ifstream fin;
    int numColors;
    string fileName = "/Users/Ben/Development/Algorithms/Project5/AlgProj5b/instances/color48-5.input";
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
		cout << "Reading number of colors" << endl;
        fin >> numColors;

        cout << "Reading knapsack instance" << endl;
        graph g(fin);

		cout << "Num colors: " << numColors << endl;
        cout << g;

        switch (algorithmOption)
        {
            case 1:
                cout << "Random - Local Search Small" << endl;
                randomColoring(g, numColors);
                randomSupervisor(g, localSearch2opt, numColors, 600);
                break;
            case 2:
                cout << "Random - Local Search Big" << endl;
                randomColoring(g, numColors);
                randomSupervisor(g, localSearch3opt, numColors, 600);
                break;
            case 3:
                cout << "Greedy - Local Search Small" << endl;
                greedyColoring(g, numColors);
                localSearch2opt(g, numColors, 600);
                break;
            case 4:
                cout << "Greedy - Local Search Big" << endl;
                greedyColoring(g, numColors);
                localSearch3opt(g, numColors, 600);
            default:
                cout << "Failure reading option. Please enter a number 1-4" << endl;
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
// Steepest decent local search algorithm using a 2 opt neighborhood. 
// Using 2opt, pairs of nodes have their colors switched to the combination
// that gives the lowest number of conflicts. The algorithm continues to 
// take the solution of the best modifications, moving in the steepest descent
// towards the optimal solution. 
{
	graph champion = graph(g);

    time_t endTime = clock() + ( maxTime * 1000000 );

    // Loop through all pairs of nodes i and j to generate naighborhood
    for (int i = 0; (i < g.numNodes()) /*&& endTime > clock()*/; i++)
    {
		for (int j = 0; (j < g.numNodes()) && endTime > clock(); j++)
		{
			if(i == j)
				continue;
			
			// Go through all possible colors and select the one that produces the least conflicts
			int lowestConflicts = 99999; // pre set to infinity
			int lowestColorI = 0; // Temp to determine color for the best next step
			int lowestColorJ = 0;
			
			// Greedily color node pair with color that creates minimum conflicts
			for(int currColorI = 0; currColorI < numColors; currColorI++)
			{
				for(int currColorJ = 0; currColorJ < numColors; currColorJ++)
				{
					g.setNodeWeight(i, currColorI);
					g.setNodeWeight(j, currColorJ);

					// Check if this colors combination is the best
					if(getNumConflicts(g) < lowestConflicts)
					{
						lowestConflicts = getNumConflicts(g);
						lowestColorI = currColorI;
						lowestColorJ = currColorJ;
					}
				}
			}
			// Move in the direction of a better solution
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
// Steepest decent local search algorithm using a 3 opt neighborhood. 
// Using 3opt, triplets of nodes have their colors switched to the combination
// that gives the lowest number of conflicts. The algorithm continues to 
// take the solution of the best modifications, moving in the steepest descent
// towards the optimal solution.
{
	graph champion = graph(g);

    time_t endTime = clock() + ( maxTime * 1000000);

    // Loop through all triplets of nodes i, j and k
    for (int i = 0; (i < g.numNodes()) && endTime > clock(); i++)
    {
		for (int j = 0; (j < g.numNodes()) && endTime > clock(); j++)
		{
			for (int k = 0; (k < g.numNodes()) && endTime > clock(); k++)
			{
				if(i == j || i == k || j == k)
					continue;
			
				// Temporary variables to store best coloring of the 
				// 3 nodes and move in the steepest descent
				int lowestConflicts = 99999; // pre set to infinity
				int lowestColorI = 0;
				int lowestColorJ = 0;
				int lowestColorK = 0;
			
				// Iterate through all color combinations and store the best
				for(int currColorI = 0; currColorI < numColors; currColorI++)
				{
					for(int currColorJ = 0; currColorJ < numColors; currColorJ++)
					{
						for(int currColorK = 0; currColorK < numColors; currColorK++)
						{
							g.setNodeWeight(i, currColorI);
							g.setNodeWeight(j, currColorJ);
							g.setNodeWeight(k, currColorK);

							// Checks for best coloring
							if(getNumConflicts(g) < lowestConflicts)
							{
								lowestConflicts = getNumConflicts(g);
								lowestColorI = currColorI;
								lowestColorJ = currColorJ;
								lowestColorK = currColorK;
							}
						}
					}
					// Move in the direction of the best neighboring solution
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

void randomColoring(graph &g, int numColors)
// Assign a random color in range numColors to each node in graph g
{
    srand(time(NULL));

    int color;

    for (int i = 0; i < g.numNodes(); i++)
    {
        color = rand() % numColors;

        g.setNodeWeight(i, color);
    }
}

void randomSupervisor(graph &g, void (*searchType)(graph&g, int numColors, int maxTime), int numColors, int maxTime)
// Run local search with 2opt or 3opt neighborhood (passed into this function),
// using a random seed. 
{
    graph bestGraph = graph(g);
    int bestNumConflicts = INT32_MAX;
    int tempNumConflicts;
    
    time_t endTime = clock() + (maxTime * 1000);

	// Keep running until time limit is reached
    while (endTime > clock())
    {
        clearColoring(g);
        // Generate a new random coloring
        randomColoring(g, numColors);

        // Use local search
        (*searchType)(g, numColors, maxTime);

		// If there is time remaining, store and search a new neighborhood
        tempNumConflicts = getNumConflicts(g);
        if (tempNumConflicts < bestNumConflicts)
        {
            bestGraph = graph(g);
            bestNumConflicts = tempNumConflicts;

            if (bestNumConflicts == 0)
                break;
        }
    }

    g = graph(bestGraph);
}

void clearColoring(graph &g)
// Set all node weights to 0
{
    for (int i = 0; i < g.numNodes(); i++) {
            g.setNodeWeight(i, 0);
    }
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









