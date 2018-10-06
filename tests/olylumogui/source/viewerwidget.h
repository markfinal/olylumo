#ifndef OLYLUMOGUI_VIEWERWIDGET_H
#define OLYLUMOGUI_VIEWERWIDGET_H

#include "QtWidgets/QWidget"

class QMdiArea;
class QLabel;

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

    void
    set_image(
        QImage *inImage);

protected:
    void
    paintEvent(
        QPaintEvent *e) override;

private:
    void
    do_ray_cast();

private:
    EViewerType  _type;
    QLabel      *_image_label;
};

} // namespace olylumogui

#endif // OLYLUMOGUI_VIEWERWIDGET_H
