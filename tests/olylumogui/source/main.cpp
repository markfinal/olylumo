#include "viewerwidget.h"
#include "scenewidget.h"
#include "olylumoray/hitablelist.h"
#include "olylumoray/sphere.h"
#include "olylumoray/lambertian.h"
#include "olylumoray/metal.h"

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

    olylumoray::HitableList world;
    world.append(new olylumoray::Sphere({ 0,0,-1,1 }, 0.5f, new olylumoray::Lambertian({ 0.8f, 0.3f, 0.3f, 1 })));
    world.append(new olylumoray::Sphere({ 0,-100.5f,-1,1 }, 100, new olylumoray::Lambertian({ 0.8f, 0.8f, 0.0f, 1 })));
    world.append(new olylumoray::Sphere({ 1,0,-1,1 }, 0.5f, new olylumoray::Metal({ 0.8f, 0.6f, 0.2f, 1.0f }, 1)));
    world.append(new olylumoray::Sphere({ -1,0,-1,1 }, 0.5f, new olylumoray::Metal({ 0.8f, 0.8f, 0.8f, 1.0f }, 0.3f)));

    auto sceneView = new olylumogui::SceneWidget();
    mdi->addSubWindow(sceneView);

    auto rayTraceViewer = new olylumogui::ViewerWidget(mdi, "Ray Trace", olylumogui::EViewerType::RayTrace, &world);
    mdi->addSubWindow(rayTraceViewer);
    ///*auto pathTraceViewer = */new olylumogui::ViewerWidget(mdi, "Path Trace", olylumogui::EViewerType::PathTrace);
    mdi->tileSubWindows();

    window.showMaximized();
    auto result = app.exec();
    return result;
}
