#include "imageresultwidget.h"

#include "QtGui/QPainter"

namespace olylumogui
{

ImageResultWidget::ImageResultWidget()
    :
    _frame_size(1, 1) // invalid size - it'll be updated in the UI setup
{
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void
ImageResultWidget::clear()
{
    this->_image_queue.clear();
}

void
ImageResultWidget::update_frame_size(
    const QSize &inNewSize)
{
    this->_frame_size = inNewSize;
    this->updateGeometry();
}

void
ImageResultWidget::queue_image_tile(
    const uint32_t inX,
    const uint32_t inY,
    QImage *inTile)
{
    QRect region(inX, inY, inTile->width(), inTile->height());
    this->_image_queue.emplace_back(region, inTile);
    this->repaint(region);
}

void
ImageResultWidget::paintEvent(
    QPaintEvent *inEvent)
{
    QWidget::paintEvent(inEvent);
    QPainter painter(this);
    painter.fillRect(this->rect(), Qt::red); // marking the non-updated region
    for (const auto &tile : this->_image_queue)
    {
        painter.drawImage(tile._region, *tile._tile);
    }
}

QSize ImageResultWidget::minimumSizeHint() const
{
    return this->_frame_size;
}

} // namespace olylumogui
