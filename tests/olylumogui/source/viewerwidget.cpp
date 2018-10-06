#include "viewerwidget.h"

#include "olylumoray/raycast.h"
#include "olylumoray/image.h"
#include "olylumoray/rgba.h"

#include "QtWidgets/QMdiArea"
#include "QtWidgets/QLabel"
#include "QtWidgets/QLayout"
#include "QtWidgets/QToolBar"
#include "QtWidgets/QComboBox"

namespace olylumogui
{

ViewerWidget::ViewerWidget(
    QMdiArea *inParent,
    const QString &inTitle,
    const EViewerType inType)
    :
    QWidget(inParent),
    _type(inType)
{
    this->setWindowTitle(inTitle);

    auto layout = new QVBoxLayout;
    this->setLayout(layout);

    auto frame_size = new QComboBox;
    frame_size->addItem("320x240", QVariant::fromValue(QSize(320, 240)));
    frame_size->addItem("640x480", QVariant::fromValue(QSize(640, 480)));
    //connect(frame_size, &QComboBox::currentIndexChanged, this, )

    auto toolbar = new QToolBar;
    toolbar->addWidget(frame_size);
    layout->addWidget(toolbar);

    this->_image_label = new QLabel(this);
    layout->addWidget(this->_image_label);

    layout->addStretch();
}

EViewerType ViewerWidget::type() const
{
    return this->_type;
}

void
ViewerWidget::set_image(
    QImage *inImage)
{
    this->_image_label->setPixmap(QPixmap::fromImage(*inImage));
    this->_image_label->adjustSize();
    this->adjustSize();
    this->update();
}

void
ViewerWidget::paintEvent(
    QPaintEvent *e)
{
    this->do_ray_cast();
    QWidget::paintEvent(e);
}

void
ViewerWidget::do_ray_cast()
{
    auto image = olylumoray::raycast();
    auto qimage = new QImage(image->width(), image->height(), QImage::Format_RGBA8888);
    auto src = image->pixels();
    for (auto row = 0u; row < image->height(); ++row)
    {
        auto dst = qimage->scanLine(row);
        for (auto col = 0u; col < image->width(); ++col)
        {
            src->convert_to_bytes(dst);
            ++src;
            dst += 4;
        }
    }

    this->set_image(qimage);
}

} // namespace olylumogui
