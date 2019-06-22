#ifndef CVRPMANAGER_H
#define CVRPMANAGER_H

#include <QFrame>

#include <cg3/viewer/mainwindow.h>

#include <cg3/viewer/drawable_objects/2d/drawable_bounding_box2d.h>

#include <drawable_objects/drawable_routes.h>


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

    //It is const because, once defined and initialized, it will never change!
    const cg3::DrawableBoundingBox2D boundingBox;

    //Vector of points (the input of the algorithm)
    std::vector<cg3::Point2Dd> points;


    //Declare your private attributes here
    /********************************************************************************************************************/
    Drawable_routes routes;
    /* WRITE YOUR CODE HERE! Read carefully the above comments! This line can be deleted */

    /********************************************************************************************************************/




    /* ----- Private main methods ----- */

    void computeDelaunayTriangulation(const std::vector<cg3::Point2Dd>& points);
    void addPointToDelaunayTriangulation(const cg3::Point2Dd& p);

    void clearDelaunayTriangulation();

    void drawDelaunayTriangulation();
    void eraseDrawnDelaunayTriangulation();
	
    void setVisibilityBoundingTriangle(const bool visible);

	void checkTriangulation();


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
    void launchAlgorithmAndMeasureTime();


private slots:

    /* ----- UI slots for loading points ----- */

    void on_loadPointsPushButton_clicked();
    void on_clearPointsPushButton_clicked();

    void point2DClicked(const cg3::Point2Dd& p);



    /* ----- UI slots for utilities ----- */

    void on_enablePickingCheckBox_stateChanged(int arg1);
    void on_resetScenePushButton_clicked();

    void on_generatePointsFilePushButton_clicked();	
	
    void on_checkTriangulationPushButton_clicked();

    void on_showBoundingTriangleCheckBox_stateChanged(int arg1);
};

#endif // CVRPMANAGER_H
