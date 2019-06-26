#ifndef CVRPAR_H
#define CVRPAR_H

#include <data_structures/topology.h>
#include <data_structures/routes.h>

#include <algorithms/cvrputils.h>

bool gyakuKeiroNoJutsuPar(Route& route,std::vector<Node>& backhauls,std::vector<Route>& routes,std::list<std::array<size_t,2>>& saveBackhaulList);

void cWpar(const Topology& topology,Routes& routes);

#endif // CVRPAR_H
