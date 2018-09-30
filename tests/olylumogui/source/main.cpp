#include "QtWidgets/QApplication"
#include "QtWidgets/QMainWindow"
#include "QtWidgets/QToolBar"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow window;
    auto toolbar = window.addToolBar("Rendering");
    auto rayTrace = toolbar->addAction("Ray trace");
    (void)rayTrace;
    auto pathTrace = toolbar->addAction("Path trace");
    pathTrace->setEnabled(false);
    window.show();
    auto result = app.exec();
    return result;
}
