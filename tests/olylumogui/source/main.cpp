#include "olylumoray/raycast.h"

#include "QtWidgets/QApplication"
#include "QtWidgets/QMainWindow"
#include "QtWidgets/QToolBar"
#include "QtWidgets/QMdiArea"
#include "QtWidgets/QLabel"

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

    auto main_window = qobject_cast<QMainWindow*>(qApp->activeWindow());
    auto mdi_area = qobject_cast<QMdiArea*>(main_window->centralWidget());
    auto widget = new QWidget;
    mdi_area->addSubWindow(widget);
    auto label = new QLabel(widget);
    label->setPixmap(QPixmap::fromImage(*qimage));
    widget->showMaximized();
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
    auto toolbar = window.addToolBar("Rendering");
    auto rayTrace = toolbar->addAction("Ray trace");
    QObject::connect(rayTrace, &QAction::triggered, do_ray_cast);
    auto pathTrace = toolbar->addAction("Path trace");
    pathTrace->setEnabled(false);
    window.show();
    auto result = app.exec();
    return result;
}
