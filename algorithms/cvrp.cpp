#include "cvrp.h"

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

size_t findBestBackhaulSuccessorFromLinehaul(Node& linehaul,std::vector<Node>& backhauls){

    size_t maxIndex = 0;
    double maxSaving = 0.0;

    for (size_t i = 1;i < backhauls.size();i++) {
        Node& currentBackhaul = backhauls[i];
        double saving = 0.0;
        double distLinehaulBackhaul = linehaul.getCoordinates().dist(currentBackhaul.getCoordinates());
        double distLinehaulFromStart = linehaul.getCoordinates().dist(backhauls[0].getCoordinates());
        double distBackhaulFromStart = currentBackhaul.getCoordinates().dist(backhauls[0].getCoordinates());

        saving = distBackhaulFromStart + distLinehaulFromStart - distLinehaulBackhaul;

        if(saving > maxSaving){
            maxSaving = saving;
            maxIndex = i;
        }
    }

    return maxIndex;
}

void eraseFromSaveTableByIndex(std::list<std::array<size_t,2>>& saveList,size_t index){

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

//data una savingList e un indice aggiunge alla route il successore con il saving più alto(se esistente),elimina le coppie che includono il predecessore e
//restituisce true se va a buon fine o false viceversa

bool addBestAdjacentNodeByIndex(const std::vector<Node> nodeList,std::list<std::array<size_t,2>>& saveList,std::vector<Node>& route,const size_t index,
                                const double& max_capacity,double& current_capacity,const bool lineHaulOrBackhaul){


    std::list<std::array<size_t,2>>::iterator it = saveList.begin();

    //itero la lista
    while(it != saveList.end()){
        //se l'attuale coppia di indici include l'indice di cui cerco il successore:
        if((*it)[0] == index || (*it)[1] == index){
            size_t candidateIndex;
            //assegno a candidateIndex l'indice del successore
            if((*it)[0] == index) candidateIndex = (*it)[1];
            else candidateIndex = (*it)[0];

            //controllo se l'altro nodo ha valori di delivery o pickup soddisfabili:
            Node otherNode = nodeList[candidateIndex];
            if(lineHaulOrBackhaul){
                //per i Linehaul
                if(otherNode.getDelivery() > current_capacity){
                    //se no restituisco fallimento
                    return false;
                }
                else {
                    //se si:
                    //elimino tutte le coppie che includono il predecessore
                    eraseFromSaveTableByIndex(saveList,index);
                    //aggiungo il successore alla route
                    current_capacity -= otherNode.getDelivery();
                    route.push_back(otherNode);
                    return true;
                }
            }
            else {
                //per i Backhaul
                if(otherNode.getPickup() > max_capacity - current_capacity){
                    //se no restituisco fallimento
                    return false;
                }
                else {
                    //se si:
                    //elimino tutte le coppie che includono il predecessore
                    eraseFromSaveTableByIndex(saveList,index);
                    //aggiungo il successore alla route
                    current_capacity -= otherNode.getPickup();
                    route.push_back(otherNode);
                    return true;
                }
            }
        }

        else {
            it++;
        }
    }
    //non ho trovato l'indice,restituisco fallimento
    return false;
}




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
        for (j=2;j<nNodes;j++) {
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
        saveList.push_front(aux);
        saveTable(aux[0],aux[1])=0.0;
        aux = getMaxIndexes(saveTable);
    }

}

void cWseq(const Topology& topology){

    size_t nNodesLinehaul=topology.getLinehaulNodes().size();
    //inizializzo tabella distanze Linehaul
    cg3::Array2D<double> distTableLinehaul = cg3::Array2D<double>(nNodesLinehaul,nNodesLinehaul,0.0);
    computeDistTable(topology.getLinehaulNodes(),distTableLinehaul);
    //inizializzo tabella savings Linehaul
    cg3::Array2D<double> saveTableLinehaul = cg3::Array2D<double>(nNodesLinehaul,nNodesLinehaul,0.0);
    computeSaveTable(distTableLinehaul, saveTableLinehaul);
    //inizializzo lista ordinata savings Linehaul
    std::list<std::array<size_t,2>> saveListLinehaul;
    computeSaveList(saveTableLinehaul,saveListLinehaul);


    size_t nNodesBackhaul=topology.getBackhaulNodes().size();
    //inizializzo tabella distanze Backhaul
    cg3::Array2D<double> distTableBackhaul = cg3::Array2D<double>(nNodesBackhaul,nNodesBackhaul,0.0);
    computeDistTable(topology.getBackhaulNodes(),distTableBackhaul);
    //inizializzo tabella distanze Backhaul
    cg3::Array2D<double> saveTableBackhaul = cg3::Array2D<double>(nNodesBackhaul,nNodesBackhaul,0.0);
    computeSaveTable(distTableBackhaul, saveTableBackhaul);
    //inizializzo tabella distanze Backhaul
    std::list<std::array<size_t,2>> saveListBackhaul;
    computeSaveList(saveTableBackhaul,saveListBackhaul);

    Routes routes;
    std::vector<Node> tmpRoute;

    for (int i=0;i<topology.getVehicle_num();i++) {

        size_t lastNodeAdded;

        double current_capacity = topology.getCapacity();

        if(saveListLinehaul.size() > 0){
            //aggiungo alla route i primi due Linehaul con il saving più alto
            std::array<size_t,2> nodeCouple = saveListLinehaul.front();
            saveListLinehaul.erase(saveListLinehaul.begin());
            tmpRoute.push_back(topology.getLinehaulNodes()[nodeCouple[0]]);
            tmpRoute.push_back(topology.getLinehaulNodes()[nodeCouple[1]]);
        }
        //segnala se il veicolo ha capacità sufficiente a soddisfare il delivery del successore ottimale(saving più alto) nella route
        bool hasNotFailed = true;

        while(hasNotFailed){
            lastNodeAdded = tmpRoute.back().getIndex();
            //prova ad aggiungere il miglior successore e aggiorna hasNotFailed con true se è riuscito o false se ha fallito
            hasNotFailed = addBestAdjacentNodeByIndex(topology.getLinehaulNodes(),saveListLinehaul,tmpRoute,lastNodeAdded,topology.getCapacity(),current_capacity,true);
        }



        //tmpRoute.push_back(saveList. nodeList[index])
        
    }
}




