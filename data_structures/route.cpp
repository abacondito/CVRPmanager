#include "route.h"

Route::Route(double max_capacity,int routeIndex)
{
    this->max_capacity = max_capacity;
    this->current_capacity_linehaul = max_capacity;
    this->current_capacity_backhaul = max_capacity;
    this->routeIndex = routeIndex;
}

bool Route::addLinehaul(Node& linehaul){
    if(this->current_capacity_linehaul >= linehaul.getDelivery()){
        if(this->nodes.size()>0){
            /*cg3::Point2Dd last = this->nodes.back().getCoordinates();
            cg3::Point2Dd current = linehaul.getCoordinates();
            double dist = last.dist(current);*/
            totCost += this->nodes.back().getCoordinates().dist(linehaul.getCoordinates());
        }
        this->current_capacity_linehaul -= linehaul.getDelivery();
        this->nodes.push_back(linehaul);
        return true;
    }

    return false;
}

bool Route::addBackhaul(Node& backhaul){
    if((this->current_capacity_backhaul) >= backhaul.getPickup()){
        if(this->nodes.size()>0){
            totCost += this->nodes.back().getCoordinates().dist(backhaul.getCoordinates());
        }
        this->current_capacity_backhaul -= backhaul.getPickup();
        this->nodes.push_back(backhaul);
        return true;
    }

    return false;
}

void Route::reverse(){
    std::vector<Node> tmpNodes;
    tmpNodes.push_back(this->nodes[0]);

    for (size_t i = this->nodes.size() -1;i >0;i--) {
        tmpNodes.push_back(this->nodes[i]);
    }

    this->nodes = tmpNodes;
}

void Route::addStartingPoint(Node& startingPoint){
    this->nodes.push_back(startingPoint);
}

Node& Route::getLastNode(){
    this->nodes[nodes.size()-1];
}

Node& Route::getNodeByIndex(size_t index){
    return  this->nodes[index];
}
size_t Route::getRouteSize(){
    return this->nodes.size();
}


double Route::getMax_capacity() const
{
    return max_capacity;
}

void Route::setMax_capacity(double value)
{
    max_capacity = value;
}

double Route::getTotCost() const
{
    return totCost;
}

double Route::getCurrent_capacity_linehaul() const
{
    return current_capacity_linehaul;
}

void Route::setCurrent_capacity_linehaul(double value)
{
    current_capacity_linehaul = value;
}

double Route::getCurrent_capacity_backhaul() const
{
    return current_capacity_backhaul;
}

void Route::setCurrent_capacity_backhaul(double value)
{
    current_capacity_backhaul = value;
}

int Route::getRouteIndex() const
{
    return routeIndex;
}

void Route::setRouteIndex(int value)
{
    routeIndex = value;
}
