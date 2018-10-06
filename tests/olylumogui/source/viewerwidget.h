#ifndef OLYLUMOGUI_VIEWERWIDGET_H
#define OLYLUMOGUI_VIEWERWIDGET_H

#include "QtWidgets/QWidget"
#include "QtCore/QFutureWatcher"

class QMdiArea;
class QLabel;
class QComboBox;
class QSpinBox;
class QImage;
class QProgressBar;

namespace olylumoray
{
    enum class EMode;
} // namespace olylumoray

namespace olylumogui
{

enum class EViewerType
{
    RayTrace,
    PathTrace
};

class RayCastWorker;

class ViewerWidget final :
    public QWidget
{
    Q_OBJECT

public:
    ViewerWidget(
        QMdiArea *inParent,
        const QString &inTitle,
        const EViewerType inType);

    EViewerType
    type() const;

protected:
    void
    on_frame_size_changed(
        int inNewIndex);

    void
    on_render_mode_changed(
        int inNewIndex);

    void
    on_sample_count_changed(
        int inNewValue
    );

    void
    on_new_image();

private:
    void
    setup_ui();

    void
    do_ray_cast();

private:
    EViewerType             _type;
    QLabel                 *_image_label;
    QComboBox              *_frame_size;
    QComboBox              *_render_mode;
    QSpinBox               *_sample_count;
    QProgressBar           *_progress;
    int                     _current_frame_size_index = 0;
    olylumoray::EMode       _current_render_mode;
    RayCastWorker          *_worker = nullptr;
};

} // namespace olylumogui

#endif // OLYLUMOGUI_VIEWERWIDGET_H
