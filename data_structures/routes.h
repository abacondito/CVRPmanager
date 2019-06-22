#ifndef ROUTES_H
#define ROUTES_H

#include <array>
#include <vector>
#include <data_structures/node.h>
#include <data_structures/route.h>


class Routes
{
public:
    Routes();

    std::vector<Route> getRoutes() const;
    void setRoutes(const std::vector<Route> &value);
    void addRoute(Route& route);

protected:
    std::vector<Route> routes;
};

#endif // ROUTES_H
