#include "route.h"

Route::Route(double max_capacity)
{
    this->max_capacity = max_capacity;
    this->current_capacity = max_capacity;
}

bool Route::addLinehaul(Node& linehaul){
    if(this->current_capacity >= linehaul.getDelivery()){
        if(this->nodes.size()>0){
            cg3::Point2Dd last = this->nodes.back().getCoordinates();
            cg3::Point2Dd current = linehaul.getCoordinates();
            double dist = last.dist(current);
            totCost += this->nodes.back().getCoordinates().dist(linehaul.getCoordinates());
        }
        this->current_capacity -= linehaul.getDelivery();
        this->nodes.push_back(linehaul);
        return true;
    }

    return false;
}

bool Route::addBackhaul(Node& backhaul){
    if((this->max_capacity - this->current_capacity) >= backhaul.getPickup()){
        if(this->nodes.size()>0){
            totCost += this->nodes.back().getCoordinates().dist(backhaul.getCoordinates());
        }
        this->current_capacity += backhaul.getPickup();
        this->nodes.push_back(backhaul);
        return true;
    }

    return false;
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

double Route::getCurrent_capacity() const
{
    return current_capacity;
}

void Route::setCurrent_capacity(double value)
{
    current_capacity = value;
}

double Route::getTotCost() const
{
    return totCost;
}
