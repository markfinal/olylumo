#include "QtWidgets/QApplication"
#include "QtWidgets/QMainWindow"
#include "QtWidgets/QToolBar"

namespace
{

void
do_ray_cast()
{
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
