#include "CVRPmanager.h"
#include "ui_CVRPmanager.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

#include <ctime>

#include "utils/fileutils.h"

#include <cg3/data_structures/arrays/arrays.h>
#include <cg3/utilities/timer.h>

#include <algorithms/cvrp.h>
#include <algorithms/cvrpar.h>


//Limits for the bounding box
//It defines where points can be added
//Do not change the following line
//#define BOUNDINGBOX 1e+6
#define BOUNDINGBOX 1e+5


//Coordinates of the bounding triangle.
//Use these coordinates to initialize the bounding triangle
//of your triangulation.
//Do not change these coordinates, they are made to avoid
//numeric errors with the test input files.
const cg3::Point2Dd BT_P1(1e+10, 0);
const cg3::Point2Dd BT_P2(0, 1e+10);
const cg3::Point2Dd BT_P3(-1e+10, -1e+10);


//Constants for scene reset
//Do not change the following lines
const double SCENERADIUS = BOUNDINGBOX;
const cg3::Pointd SCENECENTER(0,0,0);



//----------------------------------------------------------------------------------------------
//                         You have to write your code in the area below.
//----------------------------------------------------------------------------------------------



/* ----- Constructors/Destructors ----- */

/**
 * @brief Default constructor of the manager
 */
CVRPmanager::CVRPmanager(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CVRPmanager),
    mainWindow(static_cast<cg3::viewer::MainWindow&>(*parent))
{
    //UI setup
    ui->setupUi(this);

    //Setting options for the canvas
    mainWindow.disableRotation();
    mainWindow.setSelectLeftButton();

    //Add the drawable object to the mainWindow.
    //The mainWindow will take care of rendering the bounding box

    //This updates the canvas (call it whenever you change or
    //add some drawable object)
    mainWindow.updateGlCanvas();

    fitScene();

    //You probably need to initialize your objects in the constructor. Write it here if you need, or add in the constructor
    //for member initialization.
    /********************************************************************************************************************/

    /* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */

    /********************************************************************************************************************/
}


/**
 * @brief Destructor of the manager
 */
CVRPmanager::~CVRPmanager() {
    //Try to avoid using dynamic objects whenever it is possible.
    //
    //In case you allocated dynamic objects in this manager, you
    //should delete (deallocate) all of them when the application
    //closes.
    //Remember that each class which allocates dynamic objects
    //should implement a destructor which deallocates them.    
    //
    //Example:
    //      delete dynamicObject;
    //      dynamicObject = nullptr;
    /********************************************************************************************************************/

    /* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */

    /********************************************************************************************************************/

    //When the manager is destroyed, the mainWindow should
    //not have any reference to drawable objects.
    //
    //Example:
    //      mainWindow.deleteObj(&delaunayTriangulationDrawableObject);
    //  or
    //      mainWindow.deleteObj(pointerTo_DelaunayTriangulationDrawableObject);
    //
    //Try to avoid using dynamic objects whenever it is possible.
    /********************************************************************************************************************/

    /* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */
    /********************************************************************************************************************/

    //Delete the bounding box drawable object

    delete ui; //Delete interface
}



/* ----- Private main methods ----- */

/**
 * @brief Launch randomized incremental Delaunay triangulation algorithm for a vector of points.
 * Note that the vector could contain duplicates.
 * @param[in] points Vector of points
 */
void CVRPmanager::computeCvrpAlgorithm(const Topology& topology) {
    //Launch your Delaunay triangulation algorithm here on the vector "inputPoints" in input
    //(note that the vector could contain duplicates!!).
    //Here you should call an algorithm (obviously defined in ANOTHER FILE!) which
    //fills your output Triangulation data structure.
    /********************************************************************************************************************/

    int selected = this->ui->selectVersion->currentIndex();

    switch(selected)
    {
    case(0):
        cWseqRefined(topology,this->singleRoutes);
        break;
    case(1):
        cWseqRaw(topology,this->singleRoutes);
        break;
    case(2):
        cWseqUltimate(topology,this->singleRoutes);
        break;
    case(3):
        cWpar(topology,this->singleRoutes);
        break;
    }
    /* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */

    /********************************************************************************************************************/
}


/**
 * @brief Clear data of the Delaunay Triangulation
 */
void CVRPmanager::clearRoutes() {
    //Clear here your Delaunay Triangulation data structure.
    /********************************************************************************************************************/
    eraseDrawnRoutes();
    this->singleRoutes.clear();
    /* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */

    /********************************************************************************************************************/
}



/**
 * @brief Draw the Delaunay Triangulation in the canvas
 */
void CVRPmanager::drawRoutes() {
    //Note that you could keep a Drawable Delaunay Triangulation object always
    //rendered (even when it is empty), instead of deleting it from the main
    //window and re-draw it again.
    //Take as example DrawableBoundingBox2D in this manager (although it is const).
    //This is actually a good approach: if you choose it, you probably
    //do not need to write anything in the following area, since the canvas
    //will automatically draw the updated Delaunay Triangulation.
    //
    //Draw your Delaunay Triangulation in the canvas here if you choose another
    //approach.
    /********************************************************************************************************************/

    std::string routeName;

    for(size_t i = 0;i < this->singleRoutes.size();i++){

        routeName = "Route ";
        routeName += std::to_string(i+1);

        mainWindow.pushObj(&this->singleRoutes[i],routeName);
    }

    /* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */

    /********************************************************************************************************************/

    //Canvas update
    mainWindow.updateGlCanvas();
}

/**
 * @brief Erase drawn Delaunay Triangulation from the canvas
 */
void CVRPmanager::eraseDrawnRoutes() {
    //Note that you could keep a Drawable Delaunay Triangulation object always
    //rendered (even when it is empty), instead of deleting it from the main
    //window and re-draw it again.
    //Take as example DrawableBoundingBox2D in this manager, even if it never
    //changes.
    //This is actually a good approach: if you choose it, you probably do
    //not need to write anything in the following area because you should
    //have cleared your Delaunay Triangulation some lines above. The canvas will
    //automatically draw the updated Delaunay Triangulation.
    //
    //Remove the Drawable Delaunay Triangulation object from the canvas
    //(mainWindow.deleteObj() which takes a reference) if you choose another
    //approach.
    //Deallocate it if you allocated it dynamically, even if you should try
    //to avoid using dynamic objects whenever it is possible.
    /********************************************************************************************************************/


    for(size_t i = 0;i < this->singleRoutes.size();i++){

        mainWindow.deleteObj(&this->singleRoutes[i]);
    }
    /* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */

    /********************************************************************************************************************/

    //Canvas update
    mainWindow.updateGlCanvas();
}


//Define your private methods here if you need some
/********************************************************************************************************************/

/* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */

/********************************************************************************************************************/



//Define your private slots methods here if you need some
//They are needed if you want to implement voronoi
/********************************************************************************************************************/

/* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */

/********************************************************************************************************************/




//----------------------------------------------------------------------------------------------
//              Most likely, you will NOT need to write/edit code in the area below.
//----------------------------------------------------------------------------------------------


/* ----- Private utility methods ----- */

/**
 * @brief Launch the algorithm for computing the Delaunay Triangulation
 * on the input points (a vector) of this manager and measure
 * its time efficiency.
 */
void CVRPmanager::launchAlgorithmAndMeasureTime(const Topology& topology) { //Do not write code here
    //Output message
    //std::cout << "Executing the algorithm for " << this->points.size() << " points..." << std::endl;

    //Timer for evaluating the efficiency of the algorithm
    cg3::Timer t("Computing cvrp");

    //Launch delaunay algorithm on the vector of input points
    computeCvrpAlgorithm(topology);

    //Timer stop and visualization (both on console and UI)
    t.stopAndPrint();
    ui->timeLabel->setNum(t.delay());

    std::cout << std::endl;
}

/**
 * @brief Change camera of the canvas to fit the scene
 * on the bounding box in which the points can be added.
 */
void CVRPmanager::fitScene() { //Do not write code here
    mainWindow.fitScene(SCENECENTER, SCENERADIUS);
}




/* ----- UI slots for loading points ----- */

/**
 * @brief Load points event handler.
 *
 * Load input points from a file.
 */
void CVRPmanager::on_loadFilePushButton_clicked() { //Do not write code here
    //File selector
    QString filename = QFileDialog::getOpenFileName(nullptr,
                       "Open points",
                       ".",
                       "*.txt");

    if (!filename.isEmpty()) {

        //Delete from the canvas the Delaunay Triangulation
        eraseDrawnRoutes();

        //Clear current data
        clearRoutes();

        //Load input points in the vector (deleting the previous ones)
        //this->points = FileUtils::getPointsFromFile(filename.toStdString());

        std::string name = filename.toStdString();

        Topology topology = FileUtils::getTopologyFromFile(filename.toStdString());


        //Launch the algorithm on the current vector of points and measure
        //its efficiency with a timer
        int selected = this->ui->selectVersion->currentIndex();

        switch(selected)
        {
        case(0):
            name += "_cWseqRefined_test";
            break;
        case(1):
            name += "_cWseqRaw_test";
            break;
        case(2):
            name += "_cWseqMastered_test";
            break;
        case(3):
            name += "_cWpar_test";
            break;
        }

        //std::ofstream myfile (name);

        launchAlgorithmAndMeasureTime(topology);

        writeOnExistingFile(this->singleRoutes,topology.getNode_num(),name);

        //Draw Delaunay Triangulation
        drawRoutes();
    }
}

/**
 * @brief Clear points button event handler.
 *
 * It allows us to clear our Delaunay Triangulation input points.
 */
void CVRPmanager::on_clearRoutesPushButton_clicked() { //Do not write code here
    //Clear current data
    clearRoutes();

    //Delete from the canvas the Delaunay Triangulation
    eraseDrawnRoutes();

    //Clear timer data

    ui->timeLabel->setText("");
}

/* ----- UI slots for utilities ----- */

/**
 * @brief Reset scene event handler.
 *
 * It allows us to reset the camera of the canvas to
 * show/center the scene in the bounding box.
 */
void CVRPmanager::on_resetScenePushButton_clicked() { //Do not write code here
    fitScene();
}
