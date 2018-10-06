#include "viewerwidget.h"

#include "olylumoray/raycast.h"
#include "olylumoray/image.h"
#include "olylumoray/rgba.h"

#include "QtWidgets/QMdiArea"
#include "QtWidgets/QLabel"
#include "QtWidgets/QLayout"
#include "QtWidgets/QToolBar"
#include "QtWidgets/QComboBox"
#include "QtWidgets/QSpinBox"

namespace olylumogui
{

ViewerWidget::ViewerWidget(
    QMdiArea *inParent,
    const QString &inTitle,
    const EViewerType inType)
    :
    QWidget(inParent),
    _type(inType),
    _current_render_mode(olylumoray::EMode::Colour)
{
    this->setWindowTitle(inTitle);
    this->setup_ui();
    this->do_ray_cast();
}

EViewerType ViewerWidget::type() const
{
    return this->_type;
}

void
ViewerWidget::on_frame_size_changed(
    int inNewIndex)
{
    this->_current_frame_size_index = inNewIndex;
    this->do_ray_cast();
}

void
ViewerWidget::on_render_mode_changed(
    int inNewIndex)
{
    this->_current_render_mode = static_cast<olylumoray::EMode>(inNewIndex);
    this->do_ray_cast();
}

void
ViewerWidget::on_sample_count_changed(
    int inNewValue)
{
    (void)inNewValue;
    this->do_ray_cast();
}

void
ViewerWidget::do_ray_cast()
{
    const auto frame_size = this->_frame_size->itemData(this->_current_frame_size_index).toSize();
    auto image = olylumoray::raycast(
        frame_size.width(),
        frame_size.height(),
        this->_sample_count->value(),
        this->_current_render_mode
    );
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

void
ViewerWidget::setup_ui()
{
    auto layout = new QVBoxLayout;
    this->setLayout(layout);

    this->_frame_size = new QComboBox;
    this->_frame_size->addItem("320x240", QVariant::fromValue(QSize(320, 240)));
    this->_frame_size->addItem("640x480", QVariant::fromValue(QSize(640, 480)));
    this->_frame_size->addItem("1280x720", QVariant::fromValue(QSize(1280, 720)));
    connect(
        this->_frame_size,
        static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this,
        &ViewerWidget::on_frame_size_changed
    );

    this->_render_mode = new QComboBox;
    this->_render_mode->addItem("Colour"); // olylumoray::EMode::Colour
    this->_render_mode->addItem("World space normals"); // olylumo::EMode::WorldSpaceNormals
    connect(
        this->_render_mode,
        static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this,
        &ViewerWidget::on_render_mode_changed
    );

    this->_sample_count = new QSpinBox;
    this->_sample_count->setMinimum(1);
    this->_sample_count->setMaximum(10);
    this->_sample_count->setSingleStep(1);
    connect(
        this->_sample_count,
        static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this,
        &ViewerWidget::on_sample_count_changed
    );

    auto toolbar = new QToolBar;
    toolbar->addWidget(this->_frame_size);
    toolbar->addWidget(this->_render_mode);
    toolbar->addWidget(this->_sample_count);
    layout->addWidget(toolbar);

    this->_image_label = new QLabel(this);
    layout->addWidget(this->_image_label);

    layout->addStretch();
}

} // namespace olylumogui
