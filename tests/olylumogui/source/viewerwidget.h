#ifndef OLYLUMOGUI_VIEWERWIDGET_H
#define OLYLUMOGUI_VIEWERWIDGET_H

#include "QtWidgets/QWidget"

class QMdiArea;
class QLabel;
class QComboBox;
class QSpinBox;

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

private:
    void
    setup_ui();

    void
    do_ray_cast();

private:
    EViewerType       _type;
    QLabel           *_image_label;
    QComboBox        *_frame_size;
    QComboBox        *_render_mode;
    QSpinBox         *_sample_count;
    int               _current_frame_size_index = 0;
    olylumoray::EMode _current_render_mode;
};

} // namespace olylumogui

#endif // OLYLUMOGUI_VIEWERWIDGET_H
