#include "viewerwidget.h"
#include "scenewidget.h"
#include "scenemodel.h"

#include "olylumoray/scene.h"

#include "QtWidgets/QApplication"
#include "QtWidgets/QMainWindow"
#include "QtWidgets/QToolBar"
#include "QtWidgets/QMdiArea"
#include "QtWidgets/QLabel"
#include "QtCore/QDirIterator"
#include "QtWidgets/QMenuBar"
#include "QtWidgets/QMenu"
#include "QtWidgets/QDockWidget"

#ifdef D_BAM_PLATFORM_WINDOWS
#include <Windows.h>
#endif

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
#ifdef D_BAM_PLATFORM_WINDOWS
    OutputDebugString(localMsg.constData());
    OutputDebugString("\n");
#endif
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

    olylumoray::Scene scene;
    olylumogui::SceneModel model(scene);

    QMenuBar menu_bar;
    window.setMenuBar(&menu_bar);

    auto file_menu = menu_bar.addMenu("File");
    auto load_menu = file_menu->addMenu("Load");
    auto sample_menu = load_menu->addMenu("Samples");
    QDirIterator it(":/olylumo_sample_scenes/", QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        const auto path = it.next();
        auto sample = sample_menu->addAction(path);
        QObject::connect(
            sample,
            &QAction::triggered,
            [&model, path]()
            {
                model.load(path);
            }
        );
    }

    auto rayTraceViewer = new olylumogui::ViewerWidget("Ray Trace", olylumogui::EViewerType::RayTrace, &scene);
    QObject::connect(
        &model,
        &olylumogui::SceneModel::scene_changed,
        rayTraceViewer,
        &olylumogui::ViewerWidget::refresh
    );
    mdi->addSubWindow(rayTraceViewer);
    ///*auto pathTraceViewer = */new olylumogui::ViewerWidget(mdi, "Path Trace", olylumogui::EViewerType::PathTrace);
    mdi->tileSubWindows();

    auto dock_widget = new QDockWidget;
    dock_widget->setAllowedAreas(Qt::RightDockWidgetArea);
    window.addDockWidget(Qt::RightDockWidgetArea, dock_widget);

    auto sceneView = new olylumogui::SceneWidget(&model);
    dock_widget->setWidget(sceneView);

    window.showMaximized();
    auto result = app.exec();
    return result;
}
