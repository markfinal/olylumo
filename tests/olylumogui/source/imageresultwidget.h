#ifndef OLYLUMOGUI_IMAGERESULTWIDGET_H
#define OLYLUMOGUI_IMAGERESULTWIDGET_H

#include "QtWidgets/QWidget"

namespace olylumogui
{

class ImageResultWidget final :
    public QWidget
{
    Q_OBJECT

public:
    ImageResultWidget();

    void
    update_frame_size(
        const QSize &inNewSize);

    void
    update_image(
        QImage *inImage);

protected:
    void
    paintEvent(
        QPaintEvent *inEvent) override;

    QSize
    minimumSizeHint() const override;

public:
    QSize _frame_size;
    QImage *_image = nullptr;
};

} // namespace olylumogui

#endif // OLYLUMOGUI_IMAGERESULTWIDGET_H
