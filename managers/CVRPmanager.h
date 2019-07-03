#ifndef CVRPMANAGER_H
#define CVRPMANAGER_H

#include <QFrame>

#include <cg3/viewer/mainwindow.h>

#include <cg3/viewer/drawable_objects/2d/drawable_bounding_box2d.h>

#include <data_structures/topology.h>

#include <drawable_objects/drawable_routes.h>
#include <drawable_objects/drawable_route.h>


namespace Ui {
    class CVRPmanager;
}

class CVRPmanager : public QFrame {
    Q_OBJECT

public:

    /* ----- Constructors/destructors ----- */

    explicit CVRPmanager(QWidget *parent = 0);
    ~CVRPmanager();


private:

    /* ----- Private fields ----- */

    //UI fields
    Ui::CVRPmanager *ui;
    cg3::viewer::MainWindow& mainWindow;    

    //Declare your private attributes here
    /********************************************************************************************************************/
    std::vector<Drawable_route> singleRoutes;
    /* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */

    /********************************************************************************************************************/




    /* ----- Private main methods ----- */

    void computeCvrpAlgorithm(const Topology& topology);

    void clearRoutes();

    void drawRoutes();
    void eraseDrawnRoutes();
	
    //Declare your private methods here if you need some
    /********************************************************************************************************************/

    /* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */

    /********************************************************************************************************************/


private slots:

    //Declare your private slots methods here if you need some. They are needed if you want to implement Voronoi
    /********************************************************************************************************************/

    /* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */

    /********************************************************************************************************************/


private:

    /* ----- Private utility methods ----- */

    void fitScene();
    void launchAlgorithmAndMeasureTime(const Topology& topology);


private slots:

    /* ----- UI slots for loading points ----- */

    void on_loadFilePushButton_clicked();
    void on_clearRoutesPushButton_clicked();



    /* ----- UI slots for utilities ----- */

    void on_resetScenePushButton_clicked();
};

#endif // CVRPMANAGER_H
