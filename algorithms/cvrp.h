#ifndef CVRP_H
#define CVRP_H

#include <data_structures/topology.h>
#include <data_structures/node.h>
#include <cg3/data_structures/arrays/array2d.h>
#include <vector>

void CWseq(const Topology& topology);

std::array<size_t,2> getMaxIndexes(cg3::Array2D<double> matrix);
void CWseq(const Topology& topology);

#endif // CVRP_H
