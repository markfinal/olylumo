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
ImageResultWidget::update_frame_size(
    const QSize &inNewSize)
{
    this->_frame_size = inNewSize;
    this->updateGeometry();
}

void
ImageResultWidget::update_image(
    QImage *inImage)
{
    if (nullptr != this->_image)
    {
        delete this->_image;
    }
    this->_image = inImage;
    this->repaint();
}

void
ImageResultWidget::paintEvent(
    QPaintEvent *inEvent)
{
    QWidget::paintEvent(inEvent);
    QPainter painter(this);
    painter.fillRect(this->rect(), Qt::red); // marking the non-updated region
    if (nullptr != this->_image)
    {
        painter.drawImage(this->_image->rect(), *this->_image);
    }
}

QSize ImageResultWidget::minimumSizeHint() const
{
    return this->_frame_size;
}

} // namespace olylumogui
