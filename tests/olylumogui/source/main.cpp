#include "viewerwidget.h"

#include "QtWidgets/QApplication"
#include "QtWidgets/QMainWindow"
#include "QtWidgets/QToolBar"
#include "QtWidgets/QMdiArea"
#include "QtWidgets/QLabel"

namespace
{

void
myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    }
}

} // anonymous namespace

int
main(
    int argc,
    char *argv[])
{
    qInstallMessageHandler(myMessageOutput);

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
