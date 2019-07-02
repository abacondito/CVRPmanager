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

//controlla se tutti i linehaul sono nelle route,altrimenti forza i rimanenti al loro interno

void adjustLinehauls(std::vector<Drawable_route>& routes,const Topology& topology){

    std::vector<Node> linehauls = topology.getLinehaulNodes();
    int nNodes = linehauls.size();

    //vettore per verificare la presenza di tutti i nodi
    bool isPresent[nNodes];
    //lo inizializzo a false
    for (size_t i = 0;i < nNodes;i++) {
        isPresent[i] = false;
    }
    //imposto la presenza della partenza a true
    isPresent[0] = true;

    //scorro i linehaul delle route,per ognuno imposto la sua presenza a true
    for (size_t i = 0; i< routes.size();i++) {
        size_t j = 1;
        while(j<routes[i].getRouteSize() && routes[i].getNodeByIndex(j).getDelivery() > 0){
            size_t linehaulIndex = routes[i].getNodeByIndex(j).getIndex() - (topology.getBackhaulNodes().size()-1);
            isPresent[linehaulIndex] = true;
            j++;
        }
    }

    //vettore che conterrà gli indici dei nodi mancanti
    std::vector<size_t> missingNodes;

    //scorro il vettore delle presenze e se un nodo manca lo metto nell'array dei mancanti
    for (size_t i = 0;i < nNodes;i++) {
        if(isPresent[i] == false){
            missingNodes.push_back(i);
        }
    }
    //se alcuni nodi non sono stati inseriti:
    if(missingNodes.size() > 0){

        bool hasBeenInserted;
        //scorro i nodi mancanti
        for (size_t m = 0;m<missingNodes.size();m++) {
            //l'attuale nodo mancante
            Node missingNode = linehauls[missingNodes[m]];

            hasBeenInserted = false;

            //scorro tutti i nodi nelle route
            size_t i = 0;
            while(i< routes.size() && !hasBeenInserted){
                size_t j = 1;
                while(j<routes[i].getRouteSize() && routes[i].getNodeByIndex(j).getDelivery() > 0 && !hasBeenInserted){

                    Node currentNode = routes[i].getNodeByIndex(j);

                    double tmpCapacity = routes[i].getCurrent_capacity_linehaul();
                    /*controllo se il nodo mancante ha un delivery maggiore del nodo corrente e se posso sostituirli
                    senza superare la capacità massima*/
                    if(currentNode.getDelivery() < missingNode.getDelivery() &&
                            (tmpCapacity - missingNode.getDelivery() + currentNode.getDelivery() >= 0))
                    {

                        //controllo per tutte le route diversa da quella in cui vorrei scambiare il nodo corrente con il nodo mancante
                        size_t x = 0;
                        while (x<routes.size() && !hasBeenInserted){
                            if(x != i){
                                //se è possibile aggiungere alla rotta il valore del nodo corrente
                                if(routes[x].getCurrent_capacity_linehaul() - currentNode.getDelivery() >= 0){
                                    //scambio il nodo corrente con il nodo mancante nella route i
                                    routes[i].setNodeAtIndex(j,missingNode);
                                    //reinserisco il nodo corrente estratto dalla route i nella route x
                                    //creo una nuova route uguale alla vecchia ma aggiungendo il nodo corrente
                                    Drawable_route newRoute = Drawable_route(routes[0].getMax_capacity(),x);

                                    size_t lastLinehaul = 0;

                                    for (size_t z = 1;z<routes[x].getRouteSize();z++) {
                                        if(lastLinehaul == 0 && routes[x].getNodeByIndex(z).getDelivery() == 0){
                                            lastLinehaul = z -1;
                                        }
                                    }

                                    for (size_t z = 0;z<routes[x].getRouteSize();z++) {
                                        if(z<=lastLinehaul){
                                            newRoute.addLinehaul(routes[x].getNodeByIndex(z));
                                            if(z==lastLinehaul){
                                                newRoute.addLinehaul(currentNode);
                                            }
                                        }
                                        else{
                                            newRoute.addBackhaul(routes[x].getNodeByIndex(z));
                                        }
                                    }
                                    //il nodo è stato inserito
                                    routes[x] = newRoute;
                                    hasBeenInserted = true;
                                }
                            }
                            x++;
                        }


                    }
                    j++;
                }
                i++;
            }
        }
    }
}

void writeOnFile(std::vector<Drawable_route>& routes,int nNodes){

    std::ofstream myfile ("TestRoutes.txt");

    std::vector<int> allNodes;

    double totCost = 0.0;

    for (size_t i = 0;i < routes.size();i++) {

        if (myfile.is_open())
        {
            //myfile << i << "   ";
            myfile << "Max cost:" << routes[i].getTotCost() << "   ";
            totCost += routes[i].getTotCost();
        }

        for (size_t j = 0; j < routes[i].getRouteSize();j++) {

            if (myfile.is_open())
            {
              Node node = routes[i].getNodeByIndex(j);
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

void writeOnExistingFile(std::vector<Drawable_route>& routes,int nNodes,std::string& path){

    std::ofstream myfile (path);

    std::vector<int> allNodes;

    double totCost = 0.0;

    for (size_t i = 0;i < routes.size();i++) {

        if (myfile.is_open())
        {
            //myfile << i << "   ";
            myfile << "Max cost:" << routes[i].getTotCost() << "   ";
            totCost += routes[i].getTotCost();
        }

        for (size_t j = 0; j < routes[i].getRouteSize();j++) {

            if (myfile.is_open())
            {
              Node node = routes[i].getNodeByIndex(j);
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
