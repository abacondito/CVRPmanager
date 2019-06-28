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
    mainWindow(static_cast<cg3::viewer::MainWindow&>(*parent)),
    boundingBox(cg3::Point2Dd(-BOUNDINGBOX, -BOUNDINGBOX),
                cg3::Point2Dd(BOUNDINGBOX, BOUNDINGBOX)),routes(SCENECENTER,SCENERADIUS)
{
    //UI setup
    ui->setupUi(this);
    connect(&mainWindow, SIGNAL(point2DClicked(cg3::Point2Dd)), this, SLOT(point2DClicked(cg3::Point2Dd)));

    //Setting options for the canvas
    mainWindow.disableRotation();
    mainWindow.setSelectLeftButton();

    //Add the drawable object to the mainWindow.
    //The mainWindow will take care of rendering the bounding box
    //mainWindow.pushObj(&boundingBox, "Bounding box");
    mainWindow.pushObj(&routes, "Routes");

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
    mainWindow.deleteObj(&routes);
    /********************************************************************************************************************/

    //Delete the bounding box drawable object
    //mainWindow.deleteObj(&boundingBox);

    delete ui; //Delete interface
}



/* ----- Private main methods ----- */

/**
 * @brief Launch randomized incremental Delaunay triangulation algorithm for a vector of points.
 * Note that the vector could contain duplicates.
 * @param[in] points Vector of points
 */
void CVRPmanager::computeDelaunayTriangulation(const std::vector<cg3::Point2Dd>& inputPoints) {
    //Launch your Delaunay triangulation algorithm here on the vector "inputPoints" in input
    //(note that the vector could contain duplicates!!).
    //Here you should call an algorithm (obviously defined in ANOTHER FILE!) which
    //fills your output Triangulation data structure.
    /********************************************************************************************************************/

    /* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */

    /********************************************************************************************************************/
    CG3_SUPPRESS_WARNING(inputPoints);
}

/**
 * @brief Launch incremental algorithm on a Delaunay Triangulation given a point
 * @param p Point clicked
 */
void CVRPmanager::addPointToDelaunayTriangulation(const cg3::Point2Dd& p) {
    //Here you have to launch the incremental algorithm for the insertion of a new single point into the current triangulation.
    /********************************************************************************************************************/

    /* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */

    /********************************************************************************************************************/
    CG3_SUPPRESS_WARNING(p);
}

/**
 * @brief Clear data of the Delaunay Triangulation
 */
void CVRPmanager::clearDelaunayTriangulation() {
    //Clear here your Delaunay Triangulation data structure.
    /********************************************************************************************************************/
    routes = Drawable_routes(SCENECENTER,SCENERADIUS);
    /* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */

    /********************************************************************************************************************/
}



/**
 * @brief Draw the Delaunay Triangulation in the canvas
 */
void CVRPmanager::drawDelaunayTriangulation() {
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

    /* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */

    /********************************************************************************************************************/

    //Canvas update
    mainWindow.updateGlCanvas();
}

/**
 * @brief Erase drawn Delaunay Triangulation from the canvas
 */
void CVRPmanager::eraseDrawnDelaunayTriangulation() {
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

    /* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */

    /********************************************************************************************************************/

    //Canvas update
    mainWindow.updateGlCanvas();
}

/**
 * @brief Set whether or not the bounding triangle is visible
 * @param visible True if the bounding box is set to be visible, false otherwise
 */
void CVRPmanager::setVisibilityBoundingTriangle(const bool visible)
{
    //Set the visibility of your bounding triangle here
    /********************************************************************************************************************/

    /* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */

    /********************************************************************************************************************/
    CG3_SUPPRESS_WARNING(visible);
}

/**
 * @brief Check if the current triangulation is a Delaunay Triangulation
 */
void CVRPmanager::checkTriangulation() {
	std::vector<cg3::Point2Dd> points;
    cg3::Array2D<unsigned int> triangles;

    //Get your triangulation here and save the data in the vector of
    //points and in the matrix of triangles (respectively "points" and
    //"triangles").
    //"points" is a vector which contains all the points of the triangulation.
    //"triangle" is a 2D matrix with n rows (n = number of triangles) and
    //3 columns.
    //The i-th row represents the i-th triangle, which is composed of three
    //unsigned integers which are the indices of the points in the vector
    //"points" that are the vertices to the i-th triangle. 
	//Note that the points of each triangle must be in COUNTER-CLOCKWISE order.
    //You should initially resize (for efficiency reasons) the matrix "triangles" 
	//by calling "triangles.resize(n, 3)", and then fill the matrix using the
    //assignment operator: "triangles(i,j) = a"; 
    /********************************************************************************************************************/

    /* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */

    /********************************************************************************************************************/
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
void CVRPmanager::launchAlgorithmAndMeasureTime() { //Do not write code here
    //Output message
    //std::cout << "Executing the algorithm for " << this->points.size() << " points..." << std::endl;

    //Timer for evaluating the efficiency of the algorithm
    cg3::Timer t("Computing cvrp");

    //Launch delaunay algorithm on the vector of input points
    computeDelaunayTriangulation(this->points);

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
void CVRPmanager::on_loadPointsPushButton_clicked() { //Do not write code here
    //File selector
    QString filename = QFileDialog::getOpenFileName(nullptr,
                       "Open points",
                       ".",
                       "*.txt");

    if (!filename.isEmpty()) {
        //Clear current data
        clearDelaunayTriangulation();

        //Delete from the canvas the Delaunay Triangulation
        eraseDrawnDelaunayTriangulation();

        //Load input points in the vector (deleting the previous ones)
        //this->points = FileUtils::getPointsFromFile(filename.toStdString());

        Topology topology = FileUtils::getTopologyFromFile(filename.toStdString());

        //Launch the algorithm on the current vector of points and measure
        //its efficiency with a timer
        int selected = this->ui->selectVersion->currentIndex();

        switch(selected)
        {
        case(0):
            cWseqRefined(topology,routes);
            break;
        case(1):
            cWseqRaw(topology,routes);
            break;
        case(2):
            cWpar(topology,routes);
            break;
        case(3):
            cWseqBoh(topology,routes);
        }
        launchAlgorithmAndMeasureTime();

        //Draw Delaunay Triangulation
        drawDelaunayTriangulation();
    }
}

/**
 * @brief Clear points button event handler.
 *
 * It allows us to clear our Delaunay Triangulation input points.
 */
void CVRPmanager::on_clearPointsPushButton_clicked() { //Do not write code here
    //Clear the vector of points
    this->points.clear();

    //Clear current data
    clearDelaunayTriangulation();

    //Delete from the canvas the Delaunay Triangulation
    eraseDrawnDelaunayTriangulation();

    //Clear timer data

    ui->timeLabel->setText("");
}




/**
 * @brief Point 2D clicked handler.
 *
 * This method is called whenever the user clicks the canvas.
 * The coordinates (x and y) of the clicked point are given by
 * the 2D point p.
 *
 * @param[in] p Point clicked
 */
void CVRPmanager::point2DClicked(const cg3::Point2Dd& p) { //Do not write code here
    if (!boundingBox.isInside(p)) {
        //Error message if the point is not inside the bounding box
        QMessageBox::warning(this, "Cannot insert point", "Point [" +
                             QString::number(p.x()) + "," + QString::number(p.y()) +
                             "] is not contained in the bounding box.");
        return;
    }
    else {
        addPointToDelaunayTriangulation(p);
    }
}




/* ----- UI slots for utilities ----- */

/**
 * @brief Enable picking checkbox handler.
 *
 * Checkbox to enable/disable the point clicking on the canvas.
 *
 * @param[in] arg1 It contains Qt::Checked if the checkbox is checked,
 * Qt::Unchecked otherwise
 */
void CVRPmanager::on_enablePickingCheckBox_stateChanged(int arg1) { //Do not write code here
    if (arg1 == Qt::Checked){
        mainWindow.setSelectLeftButton();
    }
    else {
        mainWindow.disableRotation();
    }
}

/**
 * @brief Reset scene event handler.
 *
 * It allows us to reset the camera of the canvas to
 * show/center the scene in the bounding box.
 */
void CVRPmanager::on_resetScenePushButton_clicked() { //Do not write code here
    fitScene();
}

/**
 * @brief Generate random points handler.
 *
 * With this button we can generate files that contains
 * points which are inside the bounding box.
 */
void CVRPmanager::on_generatePointsFilePushButton_clicked() { //Do not write code here
    QString selectedFilter;
    QString filename = QFileDialog::getSaveFileName(nullptr,
                       "File containing points",
                       ".",
                       "TXT(*.txt)", &selectedFilter);

    if (!filename.isEmpty()){
        int number = QInputDialog::getInt(
                    this,
                    tr("Generate file"),
                    tr("Number of random points:"), 1000, 0, 1000000000, 1);

        //Generate points and save them in the chosen file
        FileUtils::generateRandomPointFile(filename.toStdString(), BOUNDINGBOX, number);
    }
}


/**
 * @brief Check triangulation event handler.
 *
 * It allows us to check if the triangulation is a Delaunay one.
 */
void CVRPmanager::on_checkTriangulationPushButton_clicked() {
	checkTriangulation();
}

/**
 * @brief Show bounding triangle checkbox event handler.
 *
 * It allows us to choose if bounding triangle should be drawn or not.
 *
 * @param[in] arg1 It contains Qt::Checked if the checkbox is checked,
 * Qt::Unchecked otherwise
 */
void CVRPmanager::on_showBoundingTriangleCheckBox_stateChanged(int arg1) {
    //If arg1 is Qt::Checked, then you must set the drawable triangulation
    //to draw the bounding triangle as well
    setVisibilityBoundingTriangle(arg1 == Qt::Checked);

    mainWindow.updateGlCanvas(); //Canvas update
}
