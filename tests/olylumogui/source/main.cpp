#include "viewerwidget.h"

#include "QtWidgets/QApplication"
#include "QtWidgets/QMainWindow"
#include "QtWidgets/QToolBar"
#include "QtWidgets/QMdiArea"
#include "QtWidgets/QLabel"

int
main(
    int argc,
    char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    auto mdi = new QMdiArea;
    window.setCentralWidget(mdi);

    auto rayTraceViewer = new olylumogui::ViewerWidget(mdi, "Ray Trace", olylumogui::EViewerType::RayTrace);
    mdi->addSubWindow(rayTraceViewer);
    ///*auto pathTraceViewer = */new olylumogui::ViewerWidget(mdi, "Path Trace", olylumogui::EViewerType::PathTrace);
    mdi->tileSubWindows();

    window.show();
    auto result = app.exec();
    return result;
}
