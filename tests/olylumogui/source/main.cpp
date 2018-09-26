#include "QtWidgets/QApplication"
#include "QtWidgets/QMainWindow"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow window;
    window.show();
    auto result = app.exec();
    return result;
}
