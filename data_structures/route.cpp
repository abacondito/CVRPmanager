#include "route.h"

Route::Route()
{

}

bool Route::addLinehaul(Node& linehaul){
    if(this->current_capacity >= linehaul.getDelivery()){
        if(this->nodes.size()>1){
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
        if(this->nodes.size()>1){
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

Node Route::getLastNode(){
    this->nodes.back();
}

Node Route::getNodeByIndex(size_t index){
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
