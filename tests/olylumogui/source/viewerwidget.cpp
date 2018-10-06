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

    this->_frame_size = new QComboBox;
    this->_frame_size->addItem("320x240", QVariant::fromValue(QSize(320, 240)));
    this->_frame_size->addItem("640x480", QVariant::fromValue(QSize(640, 480)));
    connect(
        this->_frame_size,
        static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this,
        &ViewerWidget::on_frame_size_change
    );

    auto toolbar = new QToolBar;
    toolbar->addWidget(this->_frame_size);
    layout->addWidget(toolbar);

    this->_image_label = new QLabel(this);
    layout->addWidget(this->_image_label);

    layout->addStretch();

    this->do_ray_cast();
}

EViewerType ViewerWidget::type() const
{
    return this->_type;
}

void
ViewerWidget::on_frame_size_change(
    int inNewIndex)
{
    this->_current_frame_size_index = inNewIndex;
    this->do_ray_cast();
}

void
ViewerWidget::do_ray_cast()
{
    const auto frame_size = this->_frame_size->itemData(this->_current_frame_size_index).toSize();
    auto image = olylumoray::raycast(frame_size.width(), frame_size.height());
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

    this->_image_label->setPixmap(QPixmap::fromImage(*qimage));
    this->update();
}

} // namespace olylumogui
