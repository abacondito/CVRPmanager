#include "cvrp.h"

//Ottiene il massimo da un array2D e i rispettivi indici
std::array<size_t,2> getMaxIndexes(cg3::Array2D<double> matrix){

    size_t i,j;
    double tmp=0.0;
    std::array<size_t,2> indexes;
    indexes[0] = 0;
    indexes[1] = 0;
    size_t size_x=matrix.getSizeX();
    size_t size_y=matrix.getSizeY();

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



void computeDistTable(const std::vector<Node>& nodes, cg3::Array2D<double>& table ){

    size_t i,j,nNodes;

    cg3::Point2Dd pointI,pointJ;

    nNodes=nodes.size();

    //cg3::Array2D<double> distTable = cg3::Array2D<double>(nNodes,nNodes,0.0);


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
        for (j=2;j<nNodes;j++) {
            if((saveTable(j,i)==0.0) && (i!=j)){
                saveTable(i,j) = distTable(i,0)+distTable(0,j)-distTable(i,j);
            }
        }
    }

}

void cWseq(const Topology& topology){

    size_t i,j,nNodes;
    cg3::Point2Dd pointI,pointJ;
    std::vector<Node> nodeList=topology.getNodes();
    nNodes=topology.getNodes().size();

    cg3::Array2D<double> distTable = cg3::Array2D<double>(nNodes,nNodes,0.0);
    computeDistTable(topology.getNodes(),distTable);
    /*cg3::Array2D<double>(nNodes,nNodes,0.0);*/
    cg3::Array2D<double> saveTable = cg3::Array2D<double>(nNodes,nNodes,0.0);
    computeSaveTable(distTable, saveTable);

    /*
    //Calcolo della Tabella delle Distanze
    for (i=0;i<nNodes;i++) {
        pointI=topology.getNodes()[i].getCoordinates();

        for (j=0;j<nNodes;j++) {
            pointJ=topology.getNodes()[j].getCoordinates();
           distTable(i,j)=pointI.dist(pointJ);
        }
    }



    //Calcolo della Tabella dei Saving
    for (i=1;i<nNodes;i++) {
        for (j=2;j<nNodes;j++) {
            if((saveTable(j,i)==0.0) && (i!=j)){
                saveTable(i,j) = distTable(i,0)+distTable(0,j)-distTable(i,j);
            }
        }
    }
*/
    //Calcolo della Lista dei Saving
    std::vector<std::array<size_t,2>> saveList;




    std::array<size_t,2> aux;   
    aux= getMaxIndexes(saveTable);

    while(saveTable(aux[0],aux[1])!=0.0){
        saveList.push_back(aux);
        saveTable(aux[0],aux[1])=0.0;
        aux= getMaxIndexes(saveTable);
    }

    /*do{

        aux= getMaxIndexes(saveTable);
        saveList.push_back(aux);
        saveTable(aux[0],aux[1])=-1.0;

    }while(saveTable(aux[0],aux[1])!=0.0);*/
    
    

    Routes routes;
    std::vector<Node> tmpRoute;


    for (i=0;i<topology.getVehicle_num();i++) {

        tmpRoute.push_back(nodeList[0]);
        size_t index = 0;

        //tmpRoute.push_back(saveList. nodeList[index])
        
    }

    int z;
    z =1;

}




