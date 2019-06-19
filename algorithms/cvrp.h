#ifndef CVRP_H
#define CVRP_H

#include <data_structures/topology.h>
#include <data_structures/node.h>
#include <cg3/data_structures/arrays/array2d.h>
#include <vector>
#include <data_structures/routes.h>

std::array<size_t,2> getMaxIndexes(cg3::Array2D<double> matrix);

void bootRoute(const Topology& topology);
void growRoute(const Topology& topology);

cg3::Array2D<double>& computeDistTable(std::vector<Node>& nodes);
cg3::Array2D<double>& computeSaveTable(cg3::Array2D<double>& distTable);

void cWseq(const Topology& topology);
void cWpar(const Topology& topology);



#endif // CVRP_H
