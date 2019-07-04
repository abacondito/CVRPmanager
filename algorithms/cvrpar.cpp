#include "cvrpar.h"

//controlla se il nodo in questione è già presente allla fine delle altre route

bool nodeInOtherRoutes(size_t nodeIndex,std::vector<Drawable_route>& routes){
    for (size_t i = 0;i < routes.size();i++) {
        if(nodeIndex == routes[i].getLastNode().getIndex()){
            return true;
        }
    }

    return false;
}


//inversione della rotta se vi è un saving maggiore con il primo linehaul e il primo backhaul,per CW parallelo

bool gyakuKeiroNoJutsuPar(Route& route,std::vector<Node>& backhauls,std::vector<Drawable_route>& routes,std::list<std::array<size_t,2>>& saveBackhaulList){

    size_t maxIndex1 = 0;
    double maxSaving1 = 0.0;
    double saving1 = 0.0;

    size_t maxIndex2 = 0;
    double maxSaving2 = 0.0;
    double saving2 = 0.0;

    Node linehaulFirst = route.getNodeByIndex(1);
    Node linehaulLast = route.getLastNode();

    double distLinehaulBackhaul, distLinehaulFromStart, distBackhaulFromStart;

    std::list<std::array<size_t,2>>::iterator it = saveBackhaulList.begin();

    while(it != saveBackhaulList.end()){

        for (size_t i=0;i<2;i++) {
            size_t index = (*it)[i];

            Node currentBackhaul = backhauls[index];

            if(!nodeInOtherRoutes(currentBackhaul.getIndex(),routes)){
                saving1=0.0;
                distLinehaulBackhaul = linehaulFirst.getCoordinates().dist(currentBackhaul.getCoordinates());
                distLinehaulFromStart = linehaulFirst.getCoordinates().dist(backhauls[0].getCoordinates());
                distBackhaulFromStart = currentBackhaul.getCoordinates().dist(backhauls[0].getCoordinates());

                saving1 = distBackhaulFromStart + distLinehaulFromStart - distLinehaulBackhaul;

                if(saving1 > maxSaving1){
                    maxSaving1 = saving1;
                    maxIndex1 = index;
                }

                distLinehaulBackhaul = linehaulLast.getCoordinates().dist(currentBackhaul.getCoordinates());
                distLinehaulFromStart = linehaulLast.getCoordinates().dist(backhauls[0].getCoordinates());

                saving2 = distBackhaulFromStart + distLinehaulFromStart - distLinehaulBackhaul;

                if(saving2 > maxSaving2){
                    maxSaving2 = saving2;
                    maxIndex2 = index;
                }
            }
        }

        it++;
    }

    if(maxSaving1 > maxSaving2){

        if(maxIndex1 != 0){
            route.reverse();
            if(route.addBackhaul(backhauls[maxIndex1])) return true;
            else {
                route.reverse();
            }
        }

    }
    else {
        if(maxIndex2 != 0){
            if(route.addBackhaul(backhauls[maxIndex2])) return true;
        }
    }

    return false;
}


//cerca il miglior successore backhaul per un linehaul

size_t findBestBackhaulSuccessorFromLinehaulPar(Node& linehaul,const std::vector<Node>& backhauls,std::vector<Drawable_route>& routes,std::list<std::array<size_t,2>>& saveBackhaulList){

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

bool addBestAdjacentNodeByIndexPar(const std::vector<Node>& nodeList,std::list<std::array<size_t,2>>& saveList,std::vector<Drawable_route>& routes,
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

//aggiunge il miglior successore dello stesso tipo a un linehaul o backhaul,non accontentandosi se il primo risultato non è soddisfabile

bool addBestAdjacentNodeByIndexOptimizedPar(const std::vector<Node>& nodeList,std::list<std::array<size_t,2>>& saveList,std::vector<Drawable_route>& routes,
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

                }
                else {
                    //per i Backhaul
                    if(routes[routeIndex].addBackhaul(otherNode)){
                        eraseFromSaveListByItem(saveList,index);
                        //se si restituisco true
                        return true;
                    }

                }

            }

        }

    it++;

    }
    //non ho trovato l'indice,restituisco fallimento
    return false;
}

//CW parallelo

void cWpar(const Topology& topology,std::vector<Drawable_route>& tmpRoutes){
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
    Drawable_route tmpRoute = Drawable_route(topology.getCapacity(),0);
    size_t lastNodeAdded;
    std::array<size_t,2> nodeCouple;

    std::list<std::array<size_t,2>>::iterator it;
    int finishedRoutes = 0;
    int k = 0;


    bool hasFinshedLinehaul[topology.getVehicle_num()];
    bool mustDoIntermediatePass[topology.getVehicle_num()];
    bool finished[topology.getVehicle_num()];
    bool hasAssigned;
    size_t nodeIndex1;
    size_t nodeIndex2;
    Node node1;
    Node node2;

    //inizializzo le route con le coppie di linehaul con il saving maggiore

    for (int i = 0;i<topology.getVehicle_num();i++) {

        hasFinshedLinehaul[i] = false;
        mustDoIntermediatePass[i] = false;
        finished[i] = false;

        std::list<std::array<size_t,2>>::iterator it = saveListLinehaul.begin();

        hasAssigned = false;

        tmpRoute = Drawable_route(topology.getCapacity(),i);
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
                if((node1.getDelivery() + node2.getDelivery()) <=  tmpRoute.getCurrent_capacity_linehaul()){

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

                hasFinshedLinehaul[k] = !(addBestAdjacentNodeByIndexOptimizedPar(topology.getLinehaulNodes(),saveListLinehaul,tmpRoutes,
                                                           k,lastNodeAdded,true));

                if(hasFinshedLinehaul[k]){
                    mustDoIntermediatePass[k]=true;
                }
            }
            else if (!hasFinshedLinehaul[k] && saveListLinehaul.size() == 0) {
                hasFinshedLinehaul[k] = true;
                mustDoIntermediatePass[k]=true;
            }


            //Fase passo intermedio
            if(hasFinshedLinehaul[k] && mustDoIntermediatePass[k]){

                lastNodeAdded = tmpRoutes[k].getLastNode().getIndex() - (topology.getBackhaulNodes().size() - 1);
                //passo intermedio standard
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
                //fine passo intermedio standard

                eraseFromSaveListByItem(saveListLinehaul, lastNodeAdded);

            }

            //Fase di soli Backhaul
            else if(hasFinshedLinehaul[k] && !mustDoIntermediatePass[k] && saveListBackhaul.size() > 0){

                lastNodeAdded = tmpRoutes[k].getLastNode().getIndex();

                finished[k] = !(addBestAdjacentNodeByIndexPar(topology.getBackhaulNodes(),saveListBackhaul,tmpRoutes,
                                                           k,lastNodeAdded,false));
                if(finished[k]){
                    eraseFromSaveListByItem(saveListBackhaul,lastNodeAdded);
                    finished[k] = true;
                    tmpRoutes[k].addLinehaul(topology.getLinehaulNodes()[0]);
                    finishedRoutes ++;
                }
            }
            else if (hasFinshedLinehaul[k] && !mustDoIntermediatePass[k] && saveListBackhaul.size() == 0) {
                finished[k] = true;
                eraseFromSaveListByItem(saveListBackhaul,lastNodeAdded);
                tmpRoutes[k].addLinehaul(topology.getLinehaulNodes()[0]);
                finishedRoutes++;
            }
        }
        //passo alla route successiva
        k = ++k % topology.getVehicle_num();
    }

    adjustLinehauls(tmpRoutes,topology);

}


//CW parallelo con inversione di rotta

void cWparInversion(const Topology& topology,std::vector<Drawable_route>& tmpRoutes){
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
    Drawable_route tmpRoute = Drawable_route(topology.getCapacity(),0);
    size_t lastNodeAdded;
    std::array<size_t,2> nodeCouple;

    std::list<std::array<size_t,2>>::iterator it;
    int finishedRoutes = 0;
    int k = 0;


    bool hasFinshedLinehaul[topology.getVehicle_num()];
    bool mustDoIntermediatePass[topology.getVehicle_num()];
    bool finished[topology.getVehicle_num()];
    bool hasAssigned;
    size_t nodeIndex1;
    size_t nodeIndex2;
    Node node1;
    Node node2;

    //inizializzo le route con le coppie di linehaul con il saving maggiore

    for (int i = 0;i<topology.getVehicle_num();i++) {

        hasFinshedLinehaul[i] = false;
        mustDoIntermediatePass[i] = false;
        finished[i] = false;

        std::list<std::array<size_t,2>>::iterator it = saveListLinehaul.begin();

        hasAssigned = false;

        tmpRoute = Drawable_route(topology.getCapacity(),i);
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
                if((node1.getDelivery() + node2.getDelivery()) <=  tmpRoute.getCurrent_capacity_linehaul()){

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

                hasFinshedLinehaul[k] = !(addBestAdjacentNodeByIndexOptimizedPar(topology.getLinehaulNodes(),saveListLinehaul,tmpRoutes,
                                                           k,lastNodeAdded,true));

                if(hasFinshedLinehaul[k]){
                    mustDoIntermediatePass[k]=true;
                }
            }
            else if (!hasFinshedLinehaul[k] && saveListLinehaul.size() == 0) {
                hasFinshedLinehaul[k] = true;
                mustDoIntermediatePass[k]=true;
            }


            //Fase passo intermedio
            if(hasFinshedLinehaul[k] && mustDoIntermediatePass[k]){

                lastNodeAdded = tmpRoutes[k].getLastNode().getIndex() - (topology.getBackhaulNodes().size() - 1);

                //inizio passo intermedio con inversione
                std::vector<Node> tmpNodes = topology.getBackhaulNodes();

                if(gyakuKeiroNoJutsuPar(tmpRoutes[k],tmpNodes,tmpRoutes,saveListBackhaul)){
                    mustDoIntermediatePass[k]=false;
                }
                else {
                    finished[k] = true;
                    tmpRoutes[k].addLinehaul(topology.getLinehaulNodes()[0]);
                    finishedRoutes++;
                }
                //fine passo intermedio con inversione

                eraseFromSaveListByItem(saveListLinehaul, lastNodeAdded);

            }

            //Fase di soli Backhaul
            else if(hasFinshedLinehaul[k] && !mustDoIntermediatePass[k] && saveListBackhaul.size() > 0){

                lastNodeAdded = tmpRoutes[k].getLastNode().getIndex();

                finished[k] = !(addBestAdjacentNodeByIndexPar(topology.getBackhaulNodes(),saveListBackhaul,tmpRoutes,
                                                           k,lastNodeAdded,false));
                if(finished[k]){
                    eraseFromSaveListByItem(saveListBackhaul,lastNodeAdded);
                    finished[k] = true;
                    tmpRoutes[k].addLinehaul(topology.getLinehaulNodes()[0]);
                    finishedRoutes ++;
                }
            }
            else if (hasFinshedLinehaul[k] && !mustDoIntermediatePass[k] && saveListBackhaul.size() == 0) {
                finished[k] = true;
                eraseFromSaveListByItem(saveListBackhaul,lastNodeAdded);
                tmpRoutes[k].addLinehaul(topology.getLinehaulNodes()[0]);
                finishedRoutes++;
            }
        }
        //passo alla route successiva
        k = ++k % topology.getVehicle_num();
    }

    adjustLinehauls(tmpRoutes,topology);

}

//iterazione del CW parallelo randomizzato

void cWparRandomStep(const Topology& topology,std::vector<Drawable_route>& tmpRoutes){
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
    Drawable_route tmpRoute = Drawable_route(topology.getCapacity(),0);
    size_t lastNodeAdded;
    std::array<size_t,2> nodeCouple;

    std::list<std::array<size_t,2>>::iterator it;
    int finishedRoutes = 0;
    int k = 0;


    bool hasFinshedLinehaul[topology.getVehicle_num()];
    bool mustDoIntermediatePass[topology.getVehicle_num()];
    bool finished[topology.getVehicle_num()];
    bool hasAssigned;
    size_t nodeIndex1;
    size_t nodeIndex2;
    Node node1;
    Node node2;

    //inizializzo le route con le coppie di linehaul con il saving maggiore

    for (int i = 0;i<topology.getVehicle_num();i++) {

        hasFinshedLinehaul[i] = false;
        mustDoIntermediatePass[i] = false;
        finished[i] = false;

        std::list<std::array<size_t,2>>::iterator it = saveListLinehaul.begin();

        hasAssigned = false;

        tmpRoute = Drawable_route(topology.getCapacity(),i);
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
                if((node1.getDelivery() + node2.getDelivery()) <=  tmpRoute.getCurrent_capacity_linehaul()){

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

                hasFinshedLinehaul[k] = !(addBestAdjacentNodeByIndexOptimizedPar(topology.getLinehaulNodes(),saveListLinehaul,tmpRoutes,
                                                           k,lastNodeAdded,true));

                if(hasFinshedLinehaul[k]){
                    mustDoIntermediatePass[k]=true;
                }
            }
            else if (!hasFinshedLinehaul[k] && saveListLinehaul.size() == 0) {

                hasFinshedLinehaul[k] = true;
                mustDoIntermediatePass[k]=true;
            }


            //Fase passo intermedio
            if(hasFinshedLinehaul[k] && mustDoIntermediatePass[k]){

                lastNodeAdded = tmpRoutes[k].getLastNode().getIndex() - (topology.getBackhaulNodes().size() - 1);

                std::vector<Node> tmpNodes = topology.getBackhaulNodes();

                if(gyakuKeiroNoJutsuPar(tmpRoutes[k],tmpNodes,tmpRoutes,saveListBackhaul)){
                    mustDoIntermediatePass[k]=false;
                }
                else {
                    finished[k] = true;
                    tmpRoutes[k].addLinehaul(topology.getLinehaulNodes()[0]);
                    finishedRoutes++;
                }
                //fine passo intermedio con inversione

                eraseFromSaveListByItem(saveListLinehaul, lastNodeAdded);

            }

            //Fase di soli Backhaul
            else if(hasFinshedLinehaul[k] && !mustDoIntermediatePass[k] && saveListBackhaul.size() > 0){

                lastNodeAdded = tmpRoutes[k].getLastNode().getIndex();

                finished[k] = !(addBestAdjacentNodeByIndexPar(topology.getBackhaulNodes(),saveListBackhaul,tmpRoutes,
                                                           k,lastNodeAdded,false));
                if(finished[k]){
                    eraseFromSaveListByItem(saveListBackhaul,lastNodeAdded);
                    finished[k] = true;
                    tmpRoutes[k].addLinehaul(topology.getLinehaulNodes()[0]);
                    finishedRoutes ++;
                }
            }
            else if (hasFinshedLinehaul[k] && !mustDoIntermediatePass[k] && saveListBackhaul.size() == 0) {
                finished[k] = true;
                eraseFromSaveListByItem(saveListBackhaul,lastNodeAdded);
                tmpRoutes[k].addLinehaul(topology.getLinehaulNodes()[0]);
                finishedRoutes++;
            }
        }


        //passo a un altra route random
        k = std::rand() % topology.getVehicle_num();

    }

    adjustLinehauls(tmpRoutes,topology);

}

//CW parallelo random con 100 iterazioni,restituisce la migliore

void cWparRandomized(const Topology& topology,std::vector<Drawable_route>& routes){


    double minCost = 10000000.0;

    double currentCost;

    size_t bestIteration = 0;

    std::vector<Drawable_route> iterationRoutes;
    
    std::array<std::vector<Drawable_route>,100> iterations;
    
    for (size_t i = 0;i < 100;i++) {

        currentCost = 0.0;


        cWparRandomStep(topology,iterationRoutes);

        for (size_t j = 0;j<iterationRoutes.size();j++) {
            currentCost += iterationRoutes[j].getTotCost();
        }
        
        iterations[i] = iterationRoutes;

        iterationRoutes.clear();

        if(currentCost < minCost){

            minCost = currentCost;
            bestIteration = i;
        }
        
    }


    routes = iterations[bestIteration];

}
