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

#include <algorithms/cvrputils.h>


void cWseqTresh(const Topology& topology,Routes& routes);

void cWseq(const Topology& topology,Routes& routes);


#endif // CVRP_H
