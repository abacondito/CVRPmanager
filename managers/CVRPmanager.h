#ifndef CVRPMANAGER_H
#define CVRPMANAGER_H

#include <QFrame>

#include <cg3/viewer/mainwindow.h>

#include <cg3/viewer/drawable_objects/2d/drawable_bounding_box2d.h>

#include <data_structures/topology.h>

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


    std::vector<Drawable_route> singleRoutes;

    double time;


    /* ----- Private main methods ----- */

    void computeCvrpAlgorithm(const Topology& topology);

    void clearRoutes();

    void drawRoutes();
    void eraseDrawnRoutes();
	


private slots:



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
