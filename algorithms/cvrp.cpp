#include "cvrp.h"


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




bool gyakuKeiroNoJutsu(Route& route,std::vector<Node>& backhauls,std::list<std::array<size_t,2>>& saveBackhaulList){

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

void ultimateGyakuKeiroNoJutsu(Route& routeLinehaul,Route& routeBackhaul,Route& finalRoute){

    if(routeBackhaul.getRouteSize() > 1){
        std::vector<double> savings;
        Node candidateLinehauls[2];
        Node candidateBackhauls[2];

        double distLinehaulBackhaul, distLinehaulFromStart, distBackhaulFromStart;

        candidateLinehauls[0] = routeLinehaul.getNodeByIndex(1);
        candidateLinehauls[1] = routeLinehaul.getLastNode();
        candidateBackhauls[0] = routeBackhaul.getNodeByIndex(1);
        candidateBackhauls[1] = routeBackhaul.getLastNode();

        for (size_t i = 0;i<2;i++) {
            Node currentLinehaul = candidateLinehauls[i];
            for (size_t j = 0;j<2;j++) {
                Node currentBackhaul = candidateBackhauls[j];

                distLinehaulBackhaul = currentLinehaul.getCoordinates().dist(currentBackhaul.getCoordinates());
                distLinehaulFromStart = currentLinehaul.getCoordinates().dist(routeLinehaul.getNodeByIndex(0).getCoordinates());
                distBackhaulFromStart = currentBackhaul.getCoordinates().dist(routeLinehaul.getNodeByIndex(0).getCoordinates());

                savings.push_back(distBackhaulFromStart + distLinehaulFromStart - distLinehaulBackhaul);
            }
        }

        double tmpMaxSaving = 0.0;
        int bestCouple = 0;

        for (size_t i = 0;i<savings.size();i++) {
            if(savings[i] > tmpMaxSaving){
                tmpMaxSaving = savings[i];
                bestCouple = i;
            }
        }



        switch (bestCouple) {
            case(0):
            routeLinehaul.reverse();
            break;

            case(1):
            routeLinehaul.reverse();
            routeBackhaul.reverse();
            break;

            case(2):
            break;

            case(3):
            routeBackhaul.reverse();
            break;
        }

    }


    for (size_t i = 0;i<routeLinehaul.getRouteSize();i++) {
        finalRoute.addLinehaul(routeLinehaul.getNodeByIndex(i));
    }

    for (size_t i = 1;i<routeBackhaul.getRouteSize();i++) {
        finalRoute.addBackhaul(routeBackhaul.getNodeByIndex(i));
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

//cerca il miglior successore accontentandosi di saving più bassi

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

//CW sequenziale che aggiunge due coppie agli ultimi veicoli rimasti,con inversione di rotta

void cWseqInversion(const Topology& topology,std::vector<Drawable_route>& routes){

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
    bool hasNotFailed;

    int unassignedLinehaulsNodes = topology.getLinehaulNodes().size() -1;

    size_t nodeLeftBehindB = 0;

    for (int i=0;i<topology.getVehicle_num();i++) {

        tmpRoute = Drawable_route(topology.getCapacity(),i);

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

            hasNotFailed = addBestAdjacentNodeByIndexOptimized(topology.getLinehaulNodes(),saveListLinehaul,
                                                       tmpRoute,lastNodeAdded,true);


            if(hasNotFailed){
                unassignedLinehaulsNodes -= 1;
            }
        }

        lastNodeAdded = tmpRoute.getLastNode().getIndex() - (topology.getBackhaulNodes().size() - 1);

        //inizio passo intermedio con inversione
        std::vector<Node> tmpNodes = topology.getBackhaulNodes();

        gyakuKeiroNoJutsu(tmpRoute,tmpNodes,saveListBackhaul);

        //fine passo intermedio con inversione

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

        routes.push_back(tmpRoute);
    }
}

//CW sequenziale schizzinoso che aggiunge due coppie agli ultimi veicoli rimasti,con inversione di rotta

void cWseqSqueamishInversion(const Topology& topology,std::vector<Drawable_route>& routes){

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
    bool hasNotFailed;

    int unassignedLinehaulsNodes = topology.getLinehaulNodes().size() -1;

    size_t nodeLeftBehindB = 0;

    for (int i=0;i<topology.getVehicle_num();i++) {

        tmpRoute = Drawable_route(topology.getCapacity(),i);

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

        //inizio passo intermedio con inversione

        std::vector<Node> tmpNodes = topology.getBackhaulNodes();

        gyakuKeiroNoJutsu(tmpRoute,tmpNodes,saveListBackhaul);

        //fine passo intermedio con inversione

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

        routes.push_back(tmpRoute);
    }

    adjustLinehauls(routes,topology);

}

//CW sequenziale standard che aggiunge due coppie agli ultimi veicoli rimasti

void cWseq(const Topology& topology,std::vector<Drawable_route>& routes){

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
    bool hasNotFailed;

    int unassignedLinehaulsNodes = topology.getLinehaulNodes().size() -1;

    size_t nodeLeftBehindB = 0;

    for (int i=0;i<topology.getVehicle_num();i++) {

        tmpRoute = Drawable_route(topology.getCapacity(),i);

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

            hasNotFailed = addBestAdjacentNodeByIndexOptimized(topology.getLinehaulNodes(),saveListLinehaul,
                                                       tmpRoute,lastNodeAdded,true);


            if(hasNotFailed){
                unassignedLinehaulsNodes -= 1;
            }
        }

        lastNodeAdded = tmpRoute.getLastNode().getIndex() - (topology.getBackhaulNodes().size() - 1);


        //passo intermedio standard

        size_t best = findBestBackhaulSuccessorFromLinehaul
                (topology.getLinehaulNodes()[lastNodeAdded],topology.getBackhaulNodes(),saveListBackhaul);
        if(best != 0){
            tmpRoute.addBackhaul(topology.getBackhaulNodes()[best]);
        }
        //fine passo intermedio standarde

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

        routes.push_back(tmpRoute);
    }
}

//CW sequenziale schizzinoso che aggiunge due coppie agli ultimi veicoli rimasti

void cWseqSqueamish(const Topology& topology,std::vector<Drawable_route>& routes){

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
    bool hasNotFailed;

    int unassignedLinehaulsNodes = topology.getLinehaulNodes().size() -1;

    size_t nodeLeftBehindB = 0;

    for (int i=0;i<topology.getVehicle_num();i++) {

        tmpRoute = Drawable_route(topology.getCapacity(),i);

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

        //inizio passo intermedio standard

        //aggiungo il backhaul ottimale dato l'ultimo linehaul aggiunto
        size_t best = findBestBackhaulSuccessorFromLinehaul
                (topology.getLinehaulNodes()[lastNodeAdded],topology.getBackhaulNodes(),saveListBackhaul);
        if(best != 0){
            tmpRoute.addBackhaul(topology.getBackhaulNodes()[best]);
        }
        //fine passo intermedio standard

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

        routes.push_back(tmpRoute);
    }

    adjustLinehauls(routes,topology);

}

//CW sequenziale che assembla separatamente linehaul e backhaul e poi li unisce

void cWseqMerge(const Topology& topology,std::vector<Drawable_route>& routes){

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
    Drawable_route tmpRouteLinehaul = Drawable_route(topology.getCapacity(),0);
    Drawable_route tmpRouteBackhaul = Drawable_route(topology.getCapacity(),0);
    size_t lastNodeAdded;
    std::array<size_t,2> nodeCouple;
    bool hasNotFailed;

    int unassignedLinehaulsNodes = topology.getLinehaulNodes().size() -1;

    size_t nodeLeftBehindB = 0;

    for (int i=0;i<topology.getVehicle_num();i++) {

        tmpRouteLinehaul = Drawable_route(topology.getCapacity(),i);
        tmpRouteBackhaul = Drawable_route(topology.getCapacity(),i);

        //Richiama la capacità standard di un veicolo

        //Inserimento del magazzino base all'interno della route
        tmpRouteLinehaul.addStartingPoint(topology.getLinehaulNodes()[0]);
        //Le righe di sopra vanno inserite in bootRoute()


        if(saveListLinehaul.size() > 0){

            //aggiungo alla route i primi due Linehaul con il saving più alto
            nodeCouple = saveListLinehaul.front();
            saveListLinehaul.erase(saveListLinehaul.begin());
            tmpRouteLinehaul.addLinehaul(topology.getLinehaulNodes()[nodeCouple[0]]);
            tmpRouteLinehaul.addLinehaul(topology.getLinehaulNodes()[nodeCouple[1]]);
            eraseFromSaveListByItem(saveListLinehaul,nodeCouple[0]);
            unassignedLinehaulsNodes -= 2;
        }

        //segnala se il veicolo ha capacità sufficiente a soddisfare il delivery del successore ottimale(saving più alto) nella route
        hasNotFailed = true;


        while(hasNotFailed && (unassignedLinehaulsNodes > 2*(topology.getVehicle_num()-i-1)) && (saveListLinehaul.size() > 0)){
            lastNodeAdded = tmpRouteLinehaul.getLastNode().getIndex() - (topology.getBackhaulNodes().size() - 1);

            hasNotFailed = addBestAdjacentNodeByIndex(topology.getLinehaulNodes(),saveListLinehaul,
                                                       tmpRouteLinehaul,lastNodeAdded,true);


            if(hasNotFailed){
                unassignedLinehaulsNodes -= 1;
            }
        }

        lastNodeAdded = tmpRouteLinehaul.getLastNode().getIndex() - (topology.getBackhaulNodes().size() - 1);

        eraseFromSaveListByItem(saveListLinehaul, lastNodeAdded);

        tmpRouteBackhaul.addStartingPoint(topology.getLinehaulNodes()[0]);

        if(saveListBackhaul.size() > 0){

            //aggiungo alla route i primi due Linehaul con il saving più alto
            nodeCouple = saveListBackhaul.front();
            saveListBackhaul.erase(saveListBackhaul.begin());
            tmpRouteBackhaul.addBackhaul(topology.getBackhaulNodes()[nodeCouple[0]]);
            tmpRouteBackhaul.addBackhaul(topology.getBackhaulNodes()[nodeCouple[1]]);
            eraseFromSaveListByItem(saveListBackhaul,nodeCouple[0]);
        }


        //segnala se il veicolo ha capacità sufficiente a soddisfare il pickup del successore ottimale(saving più alto) nella route
        hasNotFailed = true;

        while(hasNotFailed && (saveListBackhaul.size() > 0 || nodeLeftBehindB != 0)){
            lastNodeAdded = tmpRouteBackhaul.getLastNode().getIndex();

            if(nodeLeftBehindB != 0){
                if(tmpRouteBackhaul.addBackhaul(topology.getBackhaulNodes()[nodeLeftBehindB])){
                    nodeLeftBehindB = 0;
                }
                else {
                    hasNotFailed = false;
                }
            }
            else {
                //prova ad aggiungere il miglior successore e aggiorna hasNotFailed con true se è riuscito o false se ha fallito
                hasNotFailed = addBestAdjacentNodeByIndex(topology.getBackhaulNodes(),saveListBackhaul,
                                                          tmpRouteBackhaul,lastNodeAdded,false);
            }


            if(hasNotFailed){

                if(saveListBackhaul.size() == 1){
                    if(saveListBackhaul.front()[0] == tmpRouteBackhaul.getLastNode().getIndex()){
                        nodeLeftBehindB = saveListBackhaul.front()[1];
                    }
                    else {
                        nodeLeftBehindB = saveListBackhaul.front()[0];
                    }
                    saveListBackhaul.erase(saveListBackhaul.begin());
                }
            }
            else {
                eraseFromSaveListByItem(saveListBackhaul,tmpRouteBackhaul.getLastNode().getIndex());
            }
        }

        Drawable_route finalRoute = Drawable_route(topology.getCapacity(),i);

        ultimateGyakuKeiroNoJutsu(tmpRouteLinehaul,tmpRouteBackhaul,finalRoute);



        finalRoute.addLinehaul(topology.getLinehaulNodes()[0]);

        routes.push_back(finalRoute);
    }

    adjustLinehauls(routes,topology);

}





