#ifndef CVRPAR_H
#define CVRPAR_H

#include <data_structures/topology.h>
#include <data_structures/routes.h>
#include <drawable_objects/drawable_route.h>

#include <algorithms/cvrputils.h>

bool gyakuKeiroNoJutsuPar(Route& route,std::vector<Node>& backhauls,std::vector<Drawable_route>& routes,std::list<std::array<size_t,2>>& saveBackhaulList);

void cWpar(const Topology& topology,std::vector<Drawable_route>& routes);
void cWpar2(const Topology& topology,std::vector<Drawable_route>& routes);

#endif // CVRPAR_H
