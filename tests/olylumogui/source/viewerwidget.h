#ifndef OLYLUMOGUI_VIEWERWIDGET_H
#define OLYLUMOGUI_VIEWERWIDGET_H

#include "QtWidgets/QWidget"
#include "QtCore/QFutureWatcher"

class QMdiArea;
class QComboBox;
class QSpinBox;
class QImage;
class QProgressBar;

namespace olylumoray
{
    enum class EMode;
    class Scene;
} // namespace olylumoray

namespace olylumogui
{

enum class EViewerType
{
    RayTrace,
    PathTrace
};

class RayCastWorker;
class ImageResultWidget;

class ViewerWidget final :
    public QWidget
{
    Q_OBJECT

public:
    ViewerWidget(
        const QString &inTitle,
        const EViewerType inType,
        olylumoray::Scene *inScene
    );

    EViewerType
    type() const;

    void
    refresh();

protected:
    void
    on_frame_size_changed(
        int inNewIndex);

    void
    on_render_mode_changed(
        int inNewIndex);

    void
    on_sample_count_changed(
        int inNewValue);

    void
    on_max_rays_cast_changed(
        int inNewIndex);

    void
    on_tile_count_changed(
        int inNewValue);

    void
    on_new_tile(
        const uint32_t inX,
        const uint32_t inY,
        QImage *inImage);

private:
    void
    setup_ui();

    void
    do_ray_cast();

private:
    EViewerType             _type;
    olylumoray::Scene      *_scene;
    ImageResultWidget      *_image_widget;
    QComboBox              *_frame_size;
    QComboBox              *_render_mode;
    QSpinBox               *_sample_count;
    QComboBox              *_max_rays_cast;
    QSpinBox               *_tile_count;
    QProgressBar           *_progress;
    int                     _current_frame_size_index = 0;
    olylumoray::EMode       _current_render_mode;
    int                     _current_max_rays_cast_index = 0;
    RayCastWorker          *_worker = nullptr;
};

} // namespace olylumogui

#endif // OLYLUMOGUI_VIEWERWIDGET_H
