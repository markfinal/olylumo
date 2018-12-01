#ifndef OLYLUMOGUI_IMAGERESULTWIDGET_H
#define OLYLUMOGUI_IMAGERESULTWIDGET_H

#include "QtWidgets/QWidget"

#include <list>

namespace olylumogui
{

class ImageResultWidget final :
    public QWidget
{
    Q_OBJECT

public:
    ImageResultWidget();

    void
    clear();

    void
    update_frame_size(
        const QSize &inNewSize);

    void
    queue_image_tile(
        const uint32_t inX,
        const uint32_t inY,
        QImage *inTile);

protected:
    void
    paintEvent(
        QPaintEvent *inEvent) override;

    QSize
    minimumSizeHint() const override;

public:
    QSize _frame_size;
    struct Data
    {
        QRect _region;
        QImage *_tile;

        Data(const QRect &inRegion, QImage *inTile)
            :
            _region(inRegion),
            _tile(inTile)
        {}
    };
    std::list<Data> _image_queue;
};

} // namespace olylumogui

#endif // OLYLUMOGUI_IMAGERESULTWIDGET_H
