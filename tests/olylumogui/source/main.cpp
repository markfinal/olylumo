#include "viewerwidget.h"

#include "olylumoray/raycast.h"

#include "QtWidgets/QApplication"
#include "QtWidgets/QMainWindow"
#include "QtWidgets/QToolBar"
#include "QtWidgets/QMdiArea"
#include "QtWidgets/QLabel"

namespace
{

olylumogui::ViewerWidget *
find_viewer_widget(
    const olylumogui::EViewerType inType)
{
    auto main_window = qobject_cast<QMainWindow*>(qApp->activeWindow());
    auto viewers = main_window->findChildren<olylumogui::ViewerWidget*>();
    for (const auto &viewer : viewers)
    {
        auto type = viewer->type();
        if (inType == type)
        {
            return viewer;
        }
    }
    return nullptr;
}

void
do_ray_cast()
{
    auto image = olylumoray::raycast();
    auto qimage = new QImage(image->width(), image->height(), QImage::Format_RGBA8888);
    auto current_pixel = image->pixels();
    for (auto row = 0u; row < image->height(); ++row)
    {
        const auto bytes_per_line = qimage->bytesPerLine();
        memcpy(
            qimage->scanLine(row),
            current_pixel,
            bytes_per_line
        );
        current_pixel += bytes_per_line / sizeof(olylumoray::RGBA);
    }

    auto viewer = find_viewer_widget(olylumogui::EViewerType::RayTrace);
    viewer->set_image(qimage);
}

} // anonymous namespace

int
main(
    int argc,
    char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    auto mdi = new QMdiArea;
    window.setCentralWidget(mdi);

    /*auto rayTraceViewer = */new olylumogui::ViewerWidget(mdi, "Ray Trace", olylumogui::EViewerType::RayTrace);
    /*auto pathTraceViewer = */new olylumogui::ViewerWidget(mdi, "Path Trace", olylumogui::EViewerType::PathTrace);
    mdi->tileSubWindows();

    auto toolbar = window.addToolBar("Rendering");
    auto rayTrace = toolbar->addAction("Ray trace");
    QObject::connect(rayTrace, &QAction::triggered, do_ray_cast);
    auto pathTrace = toolbar->addAction("Path trace");
    pathTrace->setEnabled(false);

    window.show();
    auto result = app.exec();
    return result;
}
