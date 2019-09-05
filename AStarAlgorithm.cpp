// AStarAlgorithm2.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "Node.h"

void AStar(std::vector<Node> graph, int startNode, int endNode);

int main()
{

	/************/
	/****FILE****/
	/************/

	FILE* neighborhoodMatrixFilePointer;
	neighborhoodMatrixFilePointer = fopen("NeighborhoodMatrix.txt", "r");

	if (neighborhoodMatrixFilePointer == NULL)
		printf("\n !!! NeighborhoodMatrix.txt Not Found !!! \n");

	std::vector < std::vector < int > >  neighborhoodMatrix;
	neighborhoodMatrix.push_back(std::vector<int>());

	int i = 0;

	while ( ! feof(neighborhoodMatrixFilePointer) )
	{
		int number;
		fscanf(neighborhoodMatrixFilePointer, "%d", &number);
		neighborhoodMatrix[i].push_back(number);

		char character;
		fscanf(neighborhoodMatrixFilePointer, "%c", &character);

		if (character == '\r')
		{
			++i;
			neighborhoodMatrix.push_back(std::vector<int>());
		}

	}

	FILE* heuristicValuesFilePointer;
	heuristicValuesFilePointer = fopen("HeuristicValues.txt", "r");

	if (heuristicValuesFilePointer == NULL)
		printf("\n !!! HeuristicValues.txt Not Found !!! \n");

	std::vector<int> heuristicValues;

	while ( ! feof(heuristicValuesFilePointer) )
	{
		int number;
		fscanf(heuristicValuesFilePointer, "%d", &number);

		heuristicValues.push_back(number);

		char character;
		fscanf(heuristicValuesFilePointer, "%c", &character);

	}


	FILE* StartAndEndNodeNumberFilePointer;
	StartAndEndNodeNumberFilePointer = fopen("StartAndEndNodeNumber.txt", "r");

	if (StartAndEndNodeNumberFilePointer == NULL)
		printf("\n !!! StartAndEndNodeNumber.txt Not Found !!! \n");


	// 0 ==> Start Node && 1 ==> End Node
	std::vector<int> startAndEndNodeValues;

	while (!feof(StartAndEndNodeNumberFilePointer))
	{
		int number;
		fscanf(StartAndEndNodeNumberFilePointer, "%d", &number);

		startAndEndNodeValues.push_back(number);

		char character;
		fscanf(StartAndEndNodeNumberFilePointer, "%c", &character);

	}

	fclose(neighborhoodMatrixFilePointer);
	fclose(heuristicValuesFilePointer);
	fclose(StartAndEndNodeNumberFilePointer);

	/************/
	/****FILE****/
	/************/

	/*************/
	/****GRAPH****/
	/*************/

	std::vector<Node> graph;

	for (int i = 0; i < neighborhoodMatrix.size(); i++)
	{
		Node tempNode;
		tempNode.heuristic = heuristicValues[i];
		tempNode.NodeNumber = i;

		for (int j = 0; j < neighborhoodMatrix[i].size(); j++)
		{
			if ( neighborhoodMatrix[i][j] != 0 )
			{
				Edge tempEdge;
				tempEdge.weight = neighborhoodMatrix[i][j];
				tempEdge.destination = j;

				tempNode.edgeVector.push_back(tempEdge);

			}
		}

		graph.push_back(tempNode);

	}

	for (size_t i = 0; i < graph.size(); i++)
	{
		printf("%lu => ",i);
		for (size_t j = 0; j < graph[i].edgeVector.size(); j++)
		{
			printf("%d%s", graph[i].edgeVector[j].destination, (graph[i].edgeVector.size() -1 ) == j ? "" : ", ");
		}
		printf(" :: Heuristic => %d \n",graph[i].heuristic);
	}

	printf("\nStart Node => %d :: End Node => %d \n",startAndEndNodeValues[0], startAndEndNodeValues[1]);

	/*************/
	/****GRAPH****/
	/*************/

	AStar(graph, startAndEndNodeValues[0], startAndEndNodeValues[1]);

    return 0;
}

void AStar(std::vector<Node> graph, int startNode, int endNode)
{

	std::vector< std::vector<Node> > queue;
	int currentNode = startNode;

	queue.push_back(std::vector<Node>());
	queue[0].push_back(graph[currentNode]);

	int pathNumber = 0;
	int pathLength = INT_MAX;
	int addHeuristicNumber = 0;

	while ( currentNode != endNode )
	{
		pathLength = INT_MAX;

		// Get Min Value Queue
		for (size_t i = 0; i < queue.size(); i++)
		{
			int tempLength = 0;
			std::vector<int> containsNodeNumber;
			containsNodeNumber.push_back(queue[i][0].NodeNumber);


			for (size_t j = 0; j < queue[i].size(); j++)
			{

				if ( j == ( queue[i].size() -1 ) )
				{
					int tempHeuristicLength = INT_MAX;

					for (size_t k = 0; k < queue[i][j].edgeVector.size(); k++)
					{
						// Not Contains
						if ( ! ( std::find(containsNodeNumber.begin(), containsNodeNumber.end(), queue[i][j].edgeVector[k].destination) != containsNodeNumber.end() ) )
						{

							int currentHeuristicLength =
								queue[i][j].edgeVector[k].weight +
								graph[queue[i][j].edgeVector[k].destination].heuristic;

							if (tempHeuristicLength > currentHeuristicLength)
							{
								tempHeuristicLength = currentHeuristicLength;
								addHeuristicNumber = k;
							}
						}

					}


					tempLength += tempHeuristicLength;

				}
				else
				{
					// Real Length
					for (size_t k = 0; k < queue[i][j].edgeVector.size(); k++)
					{
						if ( queue[i][j].edgeVector[k].destination == queue[i][j+1].NodeNumber)
						{
							tempLength += queue[i][j].edgeVector[k].weight;
							containsNodeNumber.push_back(queue[i][j].edgeVector[k].destination);
							break;
						}
					}
				}
			}

			if ( pathLength > tempLength )
			{
				pathLength = tempLength;
				pathNumber = i;
			}

			containsNodeNumber.clear();

		}


		std::vector<int> containsNumber;
		for (size_t i = 0; i < queue[pathNumber].size(); i++)
		{
			containsNumber.push_back(queue[pathNumber][i].NodeNumber);
		}

		// Add Queue
		for (size_t i = 0; i < queue[pathNumber][ queue[pathNumber].size() -1  ].edgeVector.size(); i++)
		{
			if (i != addHeuristicNumber &&  
				( ! (std::find(containsNumber.begin(), containsNumber.end(), queue[pathNumber][queue[pathNumber].size() - 1].edgeVector[i].destination) != containsNumber.end()))
				)
			{
				queue.push_back(queue[pathNumber]);
				queue[queue.size() - 1].push_back(
					graph[queue[pathNumber][queue[pathNumber].size() - 1].edgeVector[i].destination]
				);


			}

		}

		queue[pathNumber].push_back(
			graph[queue[pathNumber][queue[pathNumber].size() - 1].edgeVector[addHeuristicNumber].destination]
		);
		currentNode = queue[pathNumber][queue[pathNumber].size() - 2].edgeVector[addHeuristicNumber].destination;

		containsNumber.clear();


	} // While


	printf("Path Length : %d\n",pathLength);

	for (size_t i = 0; i < queue[pathNumber].size(); i++)
	{
		printf("%d %s", queue[pathNumber][i].NodeNumber, i == (queue[pathNumber].size() -1) ? "\n" : "=> " );
	}

}



