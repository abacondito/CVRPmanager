#include "routes.h"

Routes::Routes()
{

}

std::vector<Route> Routes::getRoutes() const
{
    return routes;
}

void Routes::setRoutes(const std::vector<Route> &value)
{
    routes = value;
}

void Routes::addRoute(Route& route){
    this->routes.push_back(route);
}


