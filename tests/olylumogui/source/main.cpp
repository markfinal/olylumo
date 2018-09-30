#include "olylumoray/raycast.h"

#include "QtWidgets/QApplication"
#include "QtWidgets/QMainWindow"
#include "QtWidgets/QToolBar"

namespace
{

void
do_ray_cast()
{
    auto image = olylumoray::raycast();
    auto current_pixel = image->pixels();
    for (auto row = 0u; row < image->height(); ++row)
    {
        for (auto col = 0u; col < image->width(); ++col)
        {
            current_pixel++;
        }
    }
}

} // anonymous namespace

int
main(
    int argc,
    char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow window;
    auto toolbar = window.addToolBar("Rendering");
    auto rayTrace = toolbar->addAction("Ray trace");
    QObject::connect(rayTrace, &QAction::triggered, do_ray_cast);
    auto pathTrace = toolbar->addAction("Path trace");
    pathTrace->setEnabled(false);
    window.show();
    auto result = app.exec();
    return result;
}
