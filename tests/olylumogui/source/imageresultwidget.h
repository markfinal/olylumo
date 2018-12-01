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
        uint32_t _x;
        uint32_t _y;
        QImage *_tile;

        Data(const uint32_t inX, const uint32_t inY, QImage *inTile)
            :
            _x(inX),
            _y(inY),
            _tile(inTile)
        {}
    };
    std::list<Data> _image_queue;
};

} // namespace olylumogui

#endif // OLYLUMOGUI_IMAGERESULTWIDGET_H
