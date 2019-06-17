#include "cvrp.h"

//Ottiene il massimo da un array2D e i rispettivi indici
std::array<size_t,2> getMaxIndexes(cg3::Array2D<double> matrix){

    size_t i,j;
    double tmp=0;
    std::array<size_t,2> indexes;

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

void CWseq(const Topology& topology){



    size_t i,j,nNodes;



    cg3::Point2Dd pointI,pointJ;

    std::vector<Node> nodeList=topology.getNodes();

    nNodes=topology.getNodes().size();

    cg3::Array2D<double> distTable = cg3::Array2D<double>(nNodes,nNodes,0.0);
    cg3::Array2D<double> saveTable = cg3::Array2D<double>(nNodes,nNodes,0.0);

    //Calcolo della Tabella delle Distanze
    for (i=0;i<nNodes;i++) {
        pointI=topology.getNodes()[i].getCoordinates();

        for (j=0;j<nNodes;j++) {
            pointJ=topology.getNodes()[j].getCoordinates();
           distTable(i,j)=pointI.dist(pointJ);
        }
    }



    //Calcolo della Tabella dei Saving
    for (i=0;i<nNodes;i++) {
        for (j=0;j<nNodes;j++) {
            if(saveTable(i,j)>0.0){
                saveTable(i,j) = distTable(i,0)+distTable(0,j)-distTable(i,j);
                saveTable(j,i) = 0.0;
            }
        }
    }

    //Calcolo della Lista dei Saving
    std::vector<size_t,size_t> saveList;




    std::array<size_t,2> aux;

    do{

        aux= getMaxIndexes(saveTable);
        saveList.push_back(aux[0],aux[1]);
        saveTable(aux[0],aux[1])=0.0;

    }while(saveTable(aux[0],aux[1])!=0.0);



}




