#include "routes.h"

Routes::Routes()
{

}

std::vector<std::vector<Node> > Routes::getRoutes() const
{
    return routes;
}

void Routes::setRoutes(const std::vector<std::vector<Node> > &value)
{
    routes = value;
}

void Routes::addRoute(std::vector<Node>& route){
    this->routes.push_back(route);
}


