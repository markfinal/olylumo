#ifndef OLYLUMOGUI_VIEWERWIDGET_H
#define OLYLUMOGUI_VIEWERWIDGET_H

#include "QtWidgets/QWidget"

class QMdiArea;
class QLabel;
class QComboBox;

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
    on_frame_size_change(
        int inNewIndex);

private:
    void
    setup_ui();

    void
    do_ray_cast();

private:
    EViewerType  _type;
    QLabel      *_image_label;
    QComboBox   *_frame_size;
    int          _current_frame_size_index = 0;
};

} // namespace olylumogui

#endif // OLYLUMOGUI_VIEWERWIDGET_H
