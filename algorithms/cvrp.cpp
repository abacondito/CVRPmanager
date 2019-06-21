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

size_t findBestBackhaulSuccessorFromLinehaul(Node& linehaul,const std::vector<Node>& backhauls){

    size_t maxIndex = 0;
    double maxSaving = 0.0;
    double saving = 0.0;
    double distLinehaulBackhaul, distLinehaulFromStart, distBackhaulFromStart;

    for (size_t i = 1;i < backhauls.size();i++) {

        Node currentBackhaul = backhauls[i];
        saving=0.0;
        distLinehaulBackhaul = linehaul.getCoordinates().dist(currentBackhaul.getCoordinates());
        distLinehaulFromStart = linehaul.getCoordinates().dist(backhauls[0].getCoordinates());
        distBackhaulFromStart = currentBackhaul.getCoordinates().dist(backhauls[0].getCoordinates());

        saving = distBackhaulFromStart + distLinehaulFromStart - distLinehaulBackhaul;

        if(saving > maxSaving){
            maxSaving = saving;
            maxIndex = i;
        }
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

bool addBestAdjacentNodeByIndex(const std::vector<Node> nodeList,std::list<std::array<size_t,2>>& saveList,
                                std::vector<Node>& route, const size_t index,
                                const double& max_capacity,double& current_capacity,const bool lineHaulOrBackhaul){


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
                if(otherNode.getDelivery() > current_capacity){
                    //se no restituisco fallimento
                    return false;
                }
                else {
                    //se si:
                    //elimino tutte le coppie che includono il predecessore
                    eraseFromSaveListByItem(saveList,index);
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
                    eraseFromSaveListByItem(saveList,index);
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
    Routes routes;
    std::vector<Node> tmpRoute;
    size_t lastNodeAdded;
    double current_capacity;
    std::array<size_t,2> nodeCouple;
    bool hasNotFailed;

    /*int usedLinehaulNodes=0;
    int usableLinehaulLimit=topology.getLinehaulNodes().size()-usedLinehaulNodes;
    if (usableLinehaulLimit>(topology.getVehicle_num()-i)){
        //roba normale
    }
    else {
        tmpRoute.push_back(topology.getLinehaulNodes())
    }*/

    for (int i=0;i<topology.getVehicle_num();i++) {

        //Richiama la capacità standard di un veicolo
        current_capacity = topology.getCapacity();

        //Inserimento del magazzino base all'interno della route
        tmpRoute.push_back(topology.getLinehaulNodes()[0]);
        //Le righe di sopra vanno inserite in bootRoute()


        int unassignedLinehaulsNodes = topology.getLinehaulNodes().size() -1;
        int tmpSumThresh = threshold + module;

        if(saveListLinehaul.size() > 0){

            //aggiungo alla route i primi due Linehaul con il saving più alto
            nodeCouple = saveListLinehaul.front();
            saveListLinehaul.erase(saveListLinehaul.begin());
            tmpRoute.push_back(topology.getLinehaulNodes()[nodeCouple[0]]);
            tmpRoute.push_back(topology.getLinehaulNodes()[nodeCouple[1]]);
            tmpSumThresh -= 2;
            unassignedLinehaulsNodes -= 2;
        }

        //segnala se il veicolo ha capacità sufficiente a soddisfare il delivery del successore ottimale(saving più alto) nella route
        hasNotFailed = true;


        while(hasNotFailed && (tmpSumThresh > 0)){
            lastNodeAdded = tmpRoute.back().getIndex();

            //prova ad aggiungere il miglior successore e aggiorna hasNotFailed con true se è riuscito o false se ha fallito
            hasNotFailed = addBestAdjacentNodeByIndex(topology.getLinehaulNodes(),saveListLinehaul,
                                                      tmpRoute,lastNodeAdded,topology.getCapacity(),
                                                      current_capacity,true);
            if(hasNotFailed){
                tmpSumThresh -= 1;
                unassignedLinehaulsNodes -= 1;
            }
            else {
                threshold = unassignedLinehaulsNodes/(topology.getVehicle_num() - i +1);
            }
        }

        //aggiungo il backhaul ottimale dato l'ultimo linehaul aggiunto
        tmpRoute.push_back(topology.getBackhaulNodes()[findBestBackhaulSuccessorFromLinehaul
                           (topology.getBackhaulNodes()[lastNodeAdded],topology.getBackhaulNodes())]);


        if(saveListBackhaul.size() > 0){

            //aggiungo alla route i primi due Backhaul con il saving più alto
            nodeCouple = saveListBackhaul.front();
            saveListBackhaul.erase(saveListBackhaul.begin());
            tmpRoute.push_back(topology.getBackhaulNodes()[nodeCouple[0]]);
            tmpRoute.push_back(topology.getBackhaulNodes()[nodeCouple[1]]);
        }

        //segnala se il veicolo ha capacità sufficiente a soddisfare il pickup del successore ottimale(saving più alto) nella route
        hasNotFailed = true;

        while(hasNotFailed){
            lastNodeAdded = tmpRoute.back().getIndex();

            //prova ad aggiungere il miglior successore e aggiorna hasNotFailed con true se è riuscito o false se ha fallito
            hasNotFailed = addBestAdjacentNodeByIndex(topology.getBackhaulNodes(),saveListBackhaul,
                                                      tmpRoute,lastNodeAdded,topology.getCapacity(),
                                                      current_capacity,false);
        }

        routes.addRoute(tmpRoute);
        tmpRoute.clear();
    }

    std::ofstream myfile ("TestRoutes.txt");

    for (size_t i = 0;i < routes.getRoutes().size();i++) {

        if (myfile.is_open())
        {
            myfile << i << "   ";
        }

        for (size_t j = 0; j < routes.getRoutes()[i].size();j++) {

            if (myfile.is_open())
            {
              Node node = routes.getRoutes()[i][j];
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
        myfile.close();
    }
}




