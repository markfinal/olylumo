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
