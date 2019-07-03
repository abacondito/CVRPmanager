#ifndef CVRP_H
#define CVRP_H

#include <data_structures/topology.h>
#include <data_structures/node.h>
#include <cg3/data_structures/arrays/array2d.h>
#include <drawable_objects/drawable_route.h>
#include <vector>
#include <data_structures/routes.h>
#include <list>
#include <iostream>
#include <fstream>

#include <algorithms/cvrputils.h>


bool gyakuKeiroNoJutsu(Route& route,std::vector<Node>& backhauls,std::list<std::array<size_t,2>>& saveBackhaulList);

void ultimateGyakuKeiroNoJutsu(Route& routeLinehaul,Route& routeBackhaul,Route& finalRoute);

void cWseqRefined(const Topology& topology,std::vector<Drawable_route>& routes);

void cWseqRaw(const Topology& topology,std::vector<Drawable_route>& routes);

void cWseqUltimate(const Topology& topology,std::vector<Drawable_route>& routes);

#endif // CVRP_H
