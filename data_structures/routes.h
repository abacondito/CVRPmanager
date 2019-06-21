#ifndef ROUTES_H
#define ROUTES_H

#include <array>
#include <vector>
#include <data_structures/node.h>


class Routes
{
public:
    Routes();

    std::vector<std::vector<Node> > getRoutes() const;
    void setRoutes(const std::vector<std::vector<Node> > &value);
    void addRoute(std::vector<Node>& route);

protected:
    std::vector<std::vector<Node>> routes;
};

#endif // ROUTES_H
