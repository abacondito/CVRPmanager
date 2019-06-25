#include "cvrputils.h"



void computeDistTable(const std::vector<Node>& nodes, cg3::Array2D<double>& table ){

    size_t i,j,nNodes;

    cg3::Point2Dd pointI,pointJ;

    nNodes=nodes.size();

    for (i=0;i<nNodes;i++) {
        pointI=nodes[i].getCoordinates();

        for (j=0;j<nNodes;j++) {
            pointJ=nodes[j].getCoordinates();
            table(i,j)=pointI.dist(pointJ);
        }
    }

}

void computeSaveTable(const cg3::Array2D<double>& distTable, cg3::Array2D<double>& saveTable){

    size_t i,j;
    size_t nNodes = distTable.getSizeX();

    for (i=1;i<nNodes;i++) {
        for (j=1;j<nNodes;j++) {
            if((saveTable(j,i)==0.0) && (i!=j)){
                saveTable(i,j) = distTable(i,0)+distTable(0,j)-distTable(i,j);
            }
        }
    }

}

void computeSaveList(cg3::Array2D<double>& saveTable, std::list<std::array<size_t,2>>& saveList){

    std::array<size_t,2> aux;
    aux = getMaxIndexes(saveTable);

    while(saveTable(aux[0],aux[1])!=0.0){
        saveList.push_back(aux);
        saveTable(aux[0],aux[1])=0.0;
        aux = getMaxIndexes(saveTable);
    }

}

void writeOnFile(Routes& routes,int nNodes){

    std::ofstream myfile ("TestRoutes.txt");

    std::vector<int> allNodes;

    double totCost = 0.0;

    for (size_t i = 0;i < routes.getRoutes().size();i++) {

        if (myfile.is_open())
        {
            //myfile << i << "   ";
            myfile << "Max cost:" << routes.getRoutes()[i].getTotCost() << "   ";
            totCost += routes.getRoutes()[i].getTotCost();
        }

        for (size_t j = 0; j < routes.getRoutes()[i].getRouteSize();j++) {

            if (myfile.is_open())
            {
              Node node = routes.getRoutes()[i].getNodeByIndex(j);
              if(node.getIndex() != 0){
                  allNodes.push_back(node.getIndex());
              }
              myfile << node.getIndex();
              myfile << "   ";
            }
        }
        if (myfile.is_open())
        {
            myfile << "\n";
        }
    }

    if (myfile.is_open())
    {
        myfile << "\n" << "Missing nodes:\n";
    }

    std::sort(allNodes.begin(),allNodes.end());

    bool isPresent[nNodes];

    for (size_t i = 0;i < nNodes;i++) {
        isPresent[i] = false;
    }

    for (size_t i = 0;i < allNodes.size();i++) {
        //myfile << allNodes[i];
        //myfile << "   ";
        isPresent[allNodes[i]] = true;
    }

    for (size_t i = 0;i < nNodes;i++) {
        if(isPresent[i] == false){
            myfile << i;
            myfile << "   ";
        }
    }

    if (myfile.is_open())
    {
        myfile << "\n" << "\n" << "Tot Cost :" << totCost;
        myfile.close();
    }
}



//Ottiene il massimo da un array2D e i rispettivi indici
std::array<size_t,2> getMaxIndexes(cg3::Array2D<double> matrix){

    size_t i,j;
    double tmp=0.0;
    std::array<size_t,2> indexes;
    indexes[0] = 0;
    indexes[1] = 0;

    for (i=0;i<matrix.getSizeX();i++) {
        for (j=0;j<matrix.getSizeY();j++) {
            if(matrix(i,j)>tmp){
               indexes[0]=i;
               indexes[1]=j;
               tmp = matrix(i,j);
            }

        }
    }

    return indexes;

}


void eraseFromSaveListByItem(std::list<std::array<size_t,2>>& saveList,size_t index){

    std::list<std::array<size_t,2>>::iterator it = saveList.begin();

    while(it != saveList.end()){
        if((*it)[0] == index || (*it)[1] == index){
            saveList.erase(it++);
        }
        else {
            it++;
        }
    }
}
