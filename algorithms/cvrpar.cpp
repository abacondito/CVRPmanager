#include "cvrpar.h"

//controlla se il nodo in questione è già presente allla fine delle altre route

bool nodeInOtherRoutes(size_t nodeIndex,std::vector<Route>& routes){
    for (size_t i = 0;i < routes.size();i++) {
        if(nodeIndex == routes[i].getLastNode().getIndex()){
            return true;
        }
    }

    return false;
}


//cerca il miglior successore backhaul per un linehaul

size_t findBestBackhaulSuccessorFromLinehaulPar(Node& linehaul,const std::vector<Node>& backhauls,std::vector<Route>& routes,std::list<std::array<size_t,2>>& saveBackhaulList){

    size_t maxIndex = 0;
    double maxSaving = 0.0;
    double saving = 0.0;
    double distLinehaulBackhaul, distLinehaulFromStart, distBackhaulFromStart;

    std::list<std::array<size_t,2>>::iterator it = saveBackhaulList.begin();

    while(it != saveBackhaulList.end()){

        for (size_t i=0;i<2;i++) {
            size_t index = (*it)[i];

            Node currentBackhaul = backhauls[index];

            if(!nodeInOtherRoutes(currentBackhaul.getIndex(),routes)){
                saving=0.0;
                distLinehaulBackhaul = linehaul.getCoordinates().dist(currentBackhaul.getCoordinates());
                distLinehaulFromStart = linehaul.getCoordinates().dist(backhauls[0].getCoordinates());
                distBackhaulFromStart = currentBackhaul.getCoordinates().dist(backhauls[0].getCoordinates());

                saving = distBackhaulFromStart + distLinehaulFromStart - distLinehaulBackhaul;

                if(saving > maxSaving){
                    maxSaving = saving;
                    maxIndex = index;
                }
            }
        }

        it++;
    }

    return maxIndex;
}

//aggiunge il miglior successore dello stesso tipo a un linehaul o backhaul

bool addBestAdjacentNodeByIndexPar(const std::vector<Node>& nodeList,std::list<std::array<size_t,2>>& saveList,std::vector<Route>& routes,
                                size_t routeIndex, const size_t index,const bool lineHaulOrBackhaul){


    std::list<std::array<size_t,2>>::iterator it = saveList.begin();
    size_t candidateIndex;
    Node otherNode;

    //itero la lista
    while(it != saveList.end()){
        //se l'attuale coppia di indici include l'indice di cui cerco il successore:
        if((*it)[0] == index || (*it)[1] == index){

            //assegno a candidateIndex l'indice del successore
            if((*it)[0] == index) candidateIndex = (*it)[1];
            else candidateIndex = (*it)[0];

            //controllo se l'altro nodo ha valori di delivery o pickup soddisfabili:
            otherNode = nodeList[candidateIndex];
            if(!nodeInOtherRoutes(otherNode.getIndex(),routes)){

                if(lineHaulOrBackhaul){
                    //per i Linehaul

                    if(routes[routeIndex].addLinehaul(otherNode)){
                        eraseFromSaveListByItem(saveList,index);
                        //se si restituisco true
                        return true;
                    }

                    return false;

                }
                else {
                    //per i Backhaul
                    if(routes[routeIndex].addBackhaul(otherNode)){
                        eraseFromSaveListByItem(saveList,index);
                        //se si restituisco true
                        return true;
                    }

                    return false;
                }

            }

        }

    it++;

    }
    //non ho trovato l'indice,restituisco fallimento
    return false;
}


//CW parallelo

void cWpar(const Topology& topology,Routes& routes){
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

    //Processo del passo base
    Route tmpRoute = Route(topology.getCapacity());
    size_t lastNodeAdded;
    std::array<size_t,2> nodeCouple;

    std::list<std::array<size_t,2>>::iterator it;
    int finishedRoutes = 0;
    int k = 0;

    std::vector<Route> tmpRoutes;

    bool hasFinshedLinehaul[topology.getVehicle_num()];
    bool mustDoIntermediatePass[topology.getVehicle_num()];
    bool finished[topology.getVehicle_num()];
    bool hasAssigned;
    size_t nodeIndex1;
    size_t nodeIndex2;
    Node node1;
    Node node2;
    size_t nodeLeftBehindL = 0;
    size_t nodeLeftBehindB = 0;

    //inizializzo le route con le coppie di linehaul con il saving maggiore

    for (int i = 0;i<topology.getVehicle_num();i++) {

        hasFinshedLinehaul[i] = false;
        mustDoIntermediatePass[i] = false;
        finished[i] = false;

        std::list<std::array<size_t,2>>::iterator it = saveListLinehaul.begin();

        hasAssigned = false;

        tmpRoute = Route(topology.getCapacity());
        tmpRoute.addLinehaul(topology.getLinehaulNodes()[0]);

        while(it != saveListLinehaul.end() && !hasAssigned){

            nodeCouple = *(it);
            nodeIndex1 = nodeCouple[0] + (topology.getBackhaulNodes().size() - 1);
            nodeIndex2 = nodeCouple[1] + (topology.getBackhaulNodes().size() - 1);
            //se la coppia non è presente in nessuna route
            if(!nodeInOtherRoutes(nodeIndex1,tmpRoutes) && !nodeInOtherRoutes(nodeIndex2,tmpRoutes)){


                node1 = topology.getLinehaulNodes()[nodeCouple[0]];
                node2 = topology.getLinehaulNodes()[nodeCouple[1]];


                //controllo se posso caricare i nodi
                if((node1.getDelivery() + node2.getDelivery()) <=  tmpRoute.getCurrent_capacity()){

                    //aggiungo la coppia
                    saveListLinehaul.erase(it);
                    tmpRoute.addLinehaul(topology.getLinehaulNodes()[nodeCouple[0]]);
                    tmpRoute.addLinehaul(topology.getLinehaulNodes()[nodeCouple[1]]);
                    eraseFromSaveListByItem(saveListLinehaul,nodeCouple[0]);
                    tmpRoutes.push_back(tmpRoute);
                    hasAssigned = true;
                }
            }

            it++;
        }
    }


    while(finishedRoutes < topology.getVehicle_num()){

        //Se la rotta non è finita
        if(!finished[k]){
            //Fase di soli Linehaul
            if(!hasFinshedLinehaul[k] && saveListLinehaul.size() > 0){
                lastNodeAdded = tmpRoutes[k].getLastNode().getIndex() - (topology.getBackhaulNodes().size() - 1);

                hasFinshedLinehaul[k] = !(addBestAdjacentNodeByIndexPar(topology.getLinehaulNodes(),saveListLinehaul,tmpRoutes,
                                                           k,lastNodeAdded,true));

                if(hasFinshedLinehaul[k]) mustDoIntermediatePass[k]=true;
            }
            else if (!hasFinshedLinehaul[k] && saveListLinehaul.size() == 0) {
                finished[k] = true;
                tmpRoutes[k].addLinehaul(topology.getLinehaulNodes()[0]);
                finishedRoutes++;
            }


            //Fase passo intermedio
            if(hasFinshedLinehaul[k] && mustDoIntermediatePass[k]){

                lastNodeAdded = tmpRoutes[k].getLastNode().getIndex() - (topology.getBackhaulNodes().size() - 1);

                size_t best = findBestBackhaulSuccessorFromLinehaulPar
                        (topology.getLinehaulNodes()[lastNodeAdded],topology.getBackhaulNodes(),tmpRoutes,saveListBackhaul);
                if(best != 0){
                   tmpRoutes[k].addBackhaul(topology.getBackhaulNodes()[best]);
                   mustDoIntermediatePass[k]=false;
                }
                else {
                    finished[k] = true;
                    tmpRoutes[k].addLinehaul(topology.getLinehaulNodes()[0]);
                    finishedRoutes++;
                }

                eraseFromSaveListByItem(saveListLinehaul, lastNodeAdded);

            }

            //Fase di soli Backhaul
            else if(hasFinshedLinehaul[k] && !mustDoIntermediatePass[k] && saveListBackhaul.size() > 0){

                lastNodeAdded = tmpRoutes[k].getLastNode().getIndex();

                finished[k] = !(addBestAdjacentNodeByIndexPar(topology.getBackhaulNodes(),saveListBackhaul,tmpRoutes,
                                                           k,lastNodeAdded,false));
                if(finished[k]){
                    eraseFromSaveListByItem(saveListBackhaul,lastNodeAdded);
                    tmpRoutes[k].addLinehaul(topology.getLinehaulNodes()[0]);
                    finishedRoutes ++;
                }
            }
            else if (hasFinshedLinehaul[k] && !mustDoIntermediatePass[k] && saveListBackhaul.size() == 0) {
                finished[k] = true;
                tmpRoutes[k].addLinehaul(topology.getLinehaulNodes()[0]);
                finishedRoutes++;
            }
        }

        k = ++k % topology.getVehicle_num();
    }

    routes.setRoutes(tmpRoutes);

    writeOnFile(routes);

}
