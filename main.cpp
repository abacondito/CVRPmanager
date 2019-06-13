#include <cg3/viewer/mainwindow.h>
#include "managers/CVRPmanager.h"

int main(int argc, char *argv[]) {

    CG3_SUPPRESS_WARNING(argc);
    CG3_SUPPRESS_WARNING(argv);

    QApplication app(argc, argv);
    cg3::viewer::MainWindow gui;  //Main window, it contains QGLViewer canvas
    gui.set2DMode(true);
    gui.setSelectLeftButton();

    CVRPmanager dm(&gui);
    gui.addManager(&dm, "CVRPmanager");

    //Show the GUI
    gui.updateGlCanvas();
    gui.show();

    return app.exec();
}
