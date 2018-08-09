//#pragma once
#ifndef Node_H
#define Node_H
#include "Edge.h"
#include <vector>

class Node
{
public:
	Node();
	~Node();

	std::vector<Edge> edgeVector;
	int heuristic;
	int NodeNumber;
};

#endif // !Node_H