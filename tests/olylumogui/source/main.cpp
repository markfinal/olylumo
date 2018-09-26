#include "QtWidgets/QApplication"
#include "QtWidgets/QMainWindow"
#include "QtWidgets/QToolBar"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow window;
    auto toolbar = window.addToolBar("Rendering");
    toolbar->addAction("Make render");
    window.show();
    auto result = app.exec();
    return result;
}
