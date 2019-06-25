#include "cvrp.h"


void writeOnFile(Routes& routes){

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
        myfile << "\n" << "All nodes:\n";
    }

    std::sort(allNodes.begin(),allNodes.end());

    for (size_t i = 0;i < allNodes.size();i++) {
        myfile << allNodes[i];
        myfile << "   ";
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

size_t findBestBackhaulSuccessorFromLinehaul(Node& linehaul,const std::vector<Node>& backhauls,std::list<std::array<size_t,2>>& saveBackhaulList){

    size_t maxIndex = 0;
    double maxSaving = 0.0;
    double saving = 0.0;
    double distLinehaulBackhaul, distLinehaulFromStart, distBackhaulFromStart;

    std::list<std::array<size_t,2>>::iterator it = saveBackhaulList.begin();

    while(it != saveBackhaulList.end()){

        for (size_t i=0;i<2;i++) {
            size_t index = (*it)[i];

            Node currentBackhaul = backhauls[index];
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

        it++;
    }

    return maxIndex;
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

//data una savingList e un indice aggiunge alla route il successore con il saving più alto(se esistente),elimina le coppie che includono il predecessore e
//restituisce true se va a buon fine o false viceversa

bool addBestAdjacentNodeByIndex(const std::vector<Node>& nodeList,std::list<std::array<size_t,2>>& saveList,
                                Route& route, const size_t index,const bool lineHaulOrBackhaul){


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
            if(lineHaulOrBackhaul){
                //per i Linehaul
                if(route.addLinehaul(otherNode)){
                    eraseFromSaveListByItem(saveList,index);
                    //se si restituisco true
                    return true;
                }

                return false;

            }
            else {
                //per i Backhaul
                if(route.addBackhaul(otherNode)){
                    eraseFromSaveListByItem(saveList,index);
                    //se si restituisco true
                    return true;
                }

                return false;
            }
        }

        else {
            it++;
        }
    }
    //non ho trovato l'indice,restituisco fallimento
    return false;
}

bool addBestAdjacentNodeByIndexOptimized(const std::vector<Node>& nodeList,std::list<std::array<size_t,2>>& saveList,
                                Route& route, const size_t index,const bool lineHaulOrBackhaul){


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
            if(lineHaulOrBackhaul){
                //per i Linehaul
                if(route.addLinehaul(otherNode)){
                    eraseFromSaveListByItem(saveList,index);
                    //se si restituisco true
                    return true;
                }

                //return false;
                it++;

            }
            else {
                //per i Backhaul
                if(route.addBackhaul(otherNode)){
                    eraseFromSaveListByItem(saveList,index);
                    //se si restituisco true
                    return true;
                }

                //return false;
                it++;
            }
        }

        else {
            it++;
        }
    }
    //non ho trovato l'indice,restituisco fallimento
    return false;
}

bool nodeInOtherRoutes(size_t nodeIndex,std::vector<Route>& routes){
    for (size_t i = 0;i < routes.size();i++) {
        if(nodeIndex == routes[i].getLastNode().getIndex()){
            return true;
        }
    }

    return false;
}


bool addBestAdjacentNodeByIndexPar(const std::vector<Node>& nodeList,std::list<std::array<size_t,2>>& saveList,std::vector<Route> routes,
                                size_t routeIndex, const size_t index,const bool lineHaulOrBackhaul){


    Route route = routes[routeIndex];
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
            if(nodeInOtherRoutes(otherNode.getIndex(),routes)){

                if(lineHaulOrBackhaul){
                    //per i Linehaul

                    if(route.addLinehaul(otherNode)){
                        eraseFromSaveListByItem(saveList,index);
                        //se si restituisco true
                        return true;
                    }

                    return false;

                }
                else {
                    //per i Backhaul
                    if(route.addBackhaul(otherNode)){
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

void cWseqTresh(const Topology& topology,Routes& routes){
    int threshold=(topology.getLinehaulNodes().size()-1)/(topology.getVehicle_num());
    int module = (topology.getLinehaulNodes().size()-1)%(topology.getVehicle_num());

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
    bool hasNotFailed;

    int unassignedLinehaulsNodes = topology.getLinehaulNodes().size() -1;

    size_t nodeLeftBehindL = 0;
    size_t nodeLeftBehindB = 0;



    for (int i=0;i<topology.getVehicle_num();i++) {

        int tmpThresh = threshold;
        int tmpModuleThresh = module;

        tmpRoute = Route(topology.getCapacity());

        //Richiama la capacità standard di un veicolo

        //Inserimento del magazzino base all'interno della route
        tmpRoute.addStartingPoint(topology.getLinehaulNodes()[0]);
        //Le righe di sopra vanno inserite in bootRoute()


        if(saveListLinehaul.size() > 0){

            //aggiungo alla route i primi due Linehaul con il saving più alto
            nodeCouple = saveListLinehaul.front();
            saveListLinehaul.erase(saveListLinehaul.begin());
            tmpRoute.addLinehaul(topology.getLinehaulNodes()[nodeCouple[0]]);
            tmpRoute.addLinehaul(topology.getLinehaulNodes()[nodeCouple[1]]);
            eraseFromSaveListByItem(saveListLinehaul,nodeCouple[0]);
            tmpThresh -= 2;
            unassignedLinehaulsNodes -= 2;
        }

        //segnala se il veicolo ha capacità sufficiente a soddisfare il delivery del successore ottimale(saving più alto) nella route
        hasNotFailed = true;


        while(hasNotFailed && (tmpThresh > 0 || tmpModuleThresh > 0) && (saveListLinehaul.size() > 0 || nodeLeftBehindL != 0)){
            lastNodeAdded = tmpRoute.getLastNode().getIndex() - (topology.getBackhaulNodes().size() - 1);

            //prova ad aggiungere il miglior successore e aggiorna hasNotFailed con true se è riuscito o false se ha fallito
            if(nodeLeftBehindL != 0){
                if(tmpRoute.addLinehaul(topology.getLinehaulNodes()[nodeLeftBehindL])){
                    nodeLeftBehindL = 0;
                }
            }
            else {
                hasNotFailed = addBestAdjacentNodeByIndex(topology.getLinehaulNodes(),saveListLinehaul,
                                                       tmpRoute,lastNodeAdded,true);
            }

            if(hasNotFailed){

                if(saveListLinehaul.size() == 1){
                    if(saveListLinehaul.front()[0] == (tmpRoute.getLastNode().getIndex() - (topology.getBackhaulNodes().size() - 1))){
                        nodeLeftBehindL = saveListLinehaul.front()[1];
                    }
                    else {
                        nodeLeftBehindL = saveListLinehaul.front()[0];
                    }
                    //nodeLeftBehindL = saveListLinehaul.front()[1];
                    saveListLinehaul.erase(saveListLinehaul.begin());
                }

                if(tmpThresh > 0){
                    tmpThresh -= 1;
                }
                else {
                    tmpModuleThresh -=1;
                }

                unassignedLinehaulsNodes -= 1;
            }
            else {

                if(tmpThresh > 0){
                    threshold = unassignedLinehaulsNodes/(topology.getVehicle_num() - i +1);
                    module = unassignedLinehaulsNodes/(topology.getVehicle_num() - i +1);
                }
            }
        }

        lastNodeAdded = tmpRoute.getLastNode().getIndex() - (topology.getBackhaulNodes().size() - 1);

        //aggiungo il backhaul ottimale dato l'ultimo linehaul aggiunto
        size_t best = findBestBackhaulSuccessorFromLinehaul
                (topology.getLinehaulNodes()[lastNodeAdded],topology.getBackhaulNodes(),saveListBackhaul);
        if(best != 0){
            tmpRoute.addBackhaul(topology.getBackhaulNodes()[best]);
        }

        eraseFromSaveListByItem(saveListLinehaul, lastNodeAdded);

        //segnala se il veicolo ha capacità sufficiente a soddisfare il pickup del successore ottimale(saving più alto) nella route
        hasNotFailed = true;

        while(hasNotFailed && (saveListBackhaul.size() > 0 || nodeLeftBehindB != 0)){
            lastNodeAdded = tmpRoute.getLastNode().getIndex();

            if(nodeLeftBehindB != 0){
                if(tmpRoute.addBackhaul(topology.getBackhaulNodes()[nodeLeftBehindB])){
                    nodeLeftBehindB = 0;
                }
                else {
                    hasNotFailed = false;
                }
            }
            else {
                //prova ad aggiungere il miglior successore e aggiorna hasNotFailed con true se è riuscito o false se ha fallito
                hasNotFailed = addBestAdjacentNodeByIndex(topology.getBackhaulNodes(),saveListBackhaul,
                                                          tmpRoute,lastNodeAdded,false);
            }


            if(hasNotFailed){

                if(saveListBackhaul.size() == 1){
                    if(saveListBackhaul.front()[0] == tmpRoute.getLastNode().getIndex()){
                        nodeLeftBehindB = saveListBackhaul.front()[1];
                    }
                    else {
                        nodeLeftBehindB = saveListBackhaul.front()[0];
                    }
                    saveListBackhaul.erase(saveListBackhaul.begin());
                }
            }
            else {
                eraseFromSaveListByItem(saveListBackhaul,tmpRoute.getLastNode().getIndex());
            }
        }

        tmpRoute.addLinehaul(topology.getLinehaulNodes()[0]);

        routes.addRoute(tmpRoute);
    }

    writeOnFile(routes);

}

void cWseq(const Topology& topology,Routes& routes){

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
    bool hasNotFailed;

    int unassignedLinehaulsNodes = topology.getLinehaulNodes().size() -1;

    size_t nodeLeftBehindB = 0;



    for (int i=0;i<topology.getVehicle_num();i++) {

        tmpRoute = Route(topology.getCapacity());

        //Richiama la capacità standard di un veicolo

        //Inserimento del magazzino base all'interno della route
        tmpRoute.addStartingPoint(topology.getLinehaulNodes()[0]);
        //Le righe di sopra vanno inserite in bootRoute()


        if(saveListLinehaul.size() > 0){

            //aggiungo alla route i primi due Linehaul con il saving più alto
            nodeCouple = saveListLinehaul.front();
            saveListLinehaul.erase(saveListLinehaul.begin());
            tmpRoute.addLinehaul(topology.getLinehaulNodes()[nodeCouple[0]]);
            tmpRoute.addLinehaul(topology.getLinehaulNodes()[nodeCouple[1]]);
            eraseFromSaveListByItem(saveListLinehaul,nodeCouple[0]);
            unassignedLinehaulsNodes -= 2;
        }

        //segnala se il veicolo ha capacità sufficiente a soddisfare il delivery del successore ottimale(saving più alto) nella route
        hasNotFailed = true;


        while(hasNotFailed && (unassignedLinehaulsNodes > 2*(topology.getVehicle_num()-i-1)) && (saveListLinehaul.size() > 0)){
            lastNodeAdded = tmpRoute.getLastNode().getIndex() - (topology.getBackhaulNodes().size() - 1);

                hasNotFailed = addBestAdjacentNodeByIndex(topology.getLinehaulNodes(),saveListLinehaul,
                                                       tmpRoute,lastNodeAdded,true);


            if(hasNotFailed){
                unassignedLinehaulsNodes -= 1;
            }
        }

        lastNodeAdded = tmpRoute.getLastNode().getIndex() - (topology.getBackhaulNodes().size() - 1);

        //aggiungo il backhaul ottimale dato l'ultimo linehaul aggiunto
        size_t best = findBestBackhaulSuccessorFromLinehaul
                (topology.getLinehaulNodes()[lastNodeAdded],topology.getBackhaulNodes(),saveListBackhaul);
        if(best != 0){
            tmpRoute.addBackhaul(topology.getBackhaulNodes()[best]);
        }

        eraseFromSaveListByItem(saveListLinehaul, lastNodeAdded);

        //segnala se il veicolo ha capacità sufficiente a soddisfare il pickup del successore ottimale(saving più alto) nella route
        hasNotFailed = true;

        while(hasNotFailed && (saveListBackhaul.size() > 0 || nodeLeftBehindB != 0)){
            lastNodeAdded = tmpRoute.getLastNode().getIndex();

            if(nodeLeftBehindB != 0){
                if(tmpRoute.addBackhaul(topology.getBackhaulNodes()[nodeLeftBehindB])){
                    nodeLeftBehindB = 0;
                }
                else {
                    hasNotFailed = false;
                }
            }
            else {
                //prova ad aggiungere il miglior successore e aggiorna hasNotFailed con true se è riuscito o false se ha fallito
                hasNotFailed = addBestAdjacentNodeByIndex(topology.getBackhaulNodes(),saveListBackhaul,
                                                          tmpRoute,lastNodeAdded,false);
            }


            if(hasNotFailed){

                if(saveListBackhaul.size() == 1){
                    if(saveListBackhaul.front()[0] == tmpRoute.getLastNode().getIndex()){
                        nodeLeftBehindB = saveListBackhaul.front()[1];
                    }
                    else {
                        nodeLeftBehindB = saveListBackhaul.front()[0];
                    }
                    saveListBackhaul.erase(saveListBackhaul.begin());
                }
            }
            else {
                eraseFromSaveListByItem(saveListBackhaul,tmpRoute.getLastNode().getIndex());
            }
        }

        tmpRoute.addLinehaul(topology.getLinehaulNodes()[0]);

        routes.addRoute(tmpRoute);
    }

    writeOnFile(routes);

}


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
        mustDoIntermediatePass[i] = true;
        finished[i] = false;

        std::list<std::array<size_t,2>>::iterator it = saveListLinehaul.begin();

        hasAssigned = false;

        tmpRoute = Route(topology.getCapacity());

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

        tmpRoute = tmpRoutes[k];

        //Se la rotta non è finita
        if(!finished[k]){
            //Fase di soli Linehaul
            if(!hasFinshedLinehaul[k] && saveListLinehaul.size() > 0){
                lastNodeAdded = tmpRoute.getLastNode().getIndex() - (topology.getBackhaulNodes().size() - 1);

                hasFinshedLinehaul[k] = !(addBestAdjacentNodeByIndexPar(topology.getLinehaulNodes(),saveListLinehaul,tmpRoutes,
                                                           k,lastNodeAdded,true));

                if(hasFinshedLinehaul[k]) mustDoIntermediatePass[k]=true;
            }


            //Fase passo intermedio
            if(hasFinshedLinehaul[k] && mustDoIntermediatePass[k]){

                size_t best = findBestBackhaulSuccessorFromLinehaul
                        (topology.getLinehaulNodes()[lastNodeAdded],topology.getBackhaulNodes(),saveListBackhaul);
                if(best != 0){
                    tmpRoute.addBackhaul(topology.getBackhaulNodes()[best]);
                }

                lastNodeAdded = tmpRoute.getLastNode().getIndex() - (topology.getBackhaulNodes().size() - 1);

                eraseFromSaveListByItem(saveListLinehaul, lastNodeAdded);

                mustDoIntermediatePass[k]=false;


            }

            //Fase di soli Backhaul
            if(hasFinshedLinehaul[k] && !mustDoIntermediatePass[k] && saveListBackhaul.size() > 0){

                lastNodeAdded = tmpRoute.getLastNode().getIndex();

                finished[k] = !(addBestAdjacentNodeByIndexPar(topology.getBackhaulNodes(),saveListBackhaul,tmpRoutes,
                                                           k,lastNodeAdded,false));
                if(finished[k]){
                    tmpRoute.addLinehaul(topology.getLinehaulNodes()[0]);
                    finishedRoutes ++;
                }
            }
        }

        k = ++k % topology.getVehicle_num();
    }

    routes.setRoutes(tmpRoutes);

    writeOnFile(routes);

}




