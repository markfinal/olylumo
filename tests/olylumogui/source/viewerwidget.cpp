#include "viewerwidget.h"
#include "raycastworker.h"

#include "olylumoray/raycast.h"

#include "QtWidgets/QMdiArea"
#include "QtWidgets/QLabel"
#include "QtWidgets/QLayout"
#include "QtWidgets/QToolBar"
#include "QtWidgets/QComboBox"
#include "QtWidgets/QSpinBox"
#include "QtWidgets/QStatusBar"
#include "QtWidgets/QProgressBar"

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
ViewerWidget::on_max_rays_cast_changed(
    int inNewIndex)
{
    this->_current_max_rays_cast_index = inNewIndex;
    this->do_ray_cast();
}

void
ViewerWidget::on_new_image()
{
    auto qimage = this->_worker->result();
    this->_image_label->setPixmap(QPixmap::fromImage(*qimage));
    this->_progress->setVisible(false);
    this->_progress->setValue(0);
}

void
ViewerWidget::do_ray_cast()
{
    if (nullptr != this->_worker)
    {
        // TODO: wait for it to finish
    }

    this->_worker = new RayCastWorker(
        this->_frame_size->itemData(this->_current_frame_size_index).toSize(),
        this->_sample_count->value(),
        this->_max_rays_cast->itemData(this->_current_max_rays_cast_index).toUInt(),
        this->_current_render_mode
    );
    connect(
        this->_worker,
        &QThread::finished,
        this,
        &ViewerWidget::on_new_image,
        Qt::QueuedConnection
    );
    connect(
        this->_worker,
        &RayCastWorker::progress_changed,
        this->_progress,
        &QProgressBar::setValue
    );
    this->_progress->setVisible(true);
    this->_progress->setMinimum(0);
    this->_progress->setMaximum(this->_worker->progress_max());
    this->_progress->setValue(0);
    this->_worker->start();
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

    this->_max_rays_cast = new QComboBox;
    this->_max_rays_cast->addItem("Infinite", QVariant::fromValue(std::numeric_limits<uint32_t>::max()));
    this->_max_rays_cast->addItem("1024", QVariant::fromValue(1024));
    this->_max_rays_cast->addItem("256", QVariant::fromValue(256u));
    this->_max_rays_cast->addItem("16", QVariant::fromValue(16u));
    this->_max_rays_cast->addItem("4", QVariant::fromValue(4u));
    this->_max_rays_cast->addItem("2", QVariant::fromValue(2u));
    this->_max_rays_cast->addItem("1", QVariant::fromValue(1u));
    this->_max_rays_cast->addItem("0", QVariant::fromValue(0u));
    connect(
        this->_max_rays_cast,
        static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this,
        &ViewerWidget::on_max_rays_cast_changed
    );

    auto toolbar = new QToolBar;
    toolbar->addWidget(this->_frame_size);
    toolbar->addWidget(this->_render_mode);
    toolbar->addWidget(this->_sample_count);
    toolbar->addWidget(this->_max_rays_cast);
    layout->addWidget(toolbar);

    this->_image_label = new QLabel(this);
    layout->addWidget(this->_image_label);

    layout->addStretch();

    this->_progress = new QProgressBar;

    auto status_bar = new QStatusBar;
    status_bar->addWidget(this->_progress);
    layout->addWidget(status_bar);
}

} // namespace olylumogui
