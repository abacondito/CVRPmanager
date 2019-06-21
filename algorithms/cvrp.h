#ifndef CVRP_H
#define CVRP_H

#include <data_structures/topology.h>
#include <data_structures/node.h>
#include <cg3/data_structures/arrays/array2d.h>
#include <vector>
#include <data_structures/routes.h>
#include <list>
#include <iostream>
#include <fstream>

std::array<size_t,2> getMaxIndexes(cg3::Array2D<double> matrix);

void bootRoute(const Topology& topology);
void growRoute(const Topology& topology);

void computeDistTable(const std::vector<Node>& nodes, cg3::Array2D<double>& table);
void computeSaveTable(const cg3::Array2D<double>& distTable, cg3::Array2D<double>& saveTable);
void computeSaveList(cg3::Array2D<double>& saveTable, std::list<std::array<size_t,2>>& saveList);

void cWseq(const Topology& topology);
void cWpar(const Topology& topology);



#endif // CVRP_H
