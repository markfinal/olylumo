#include "viewerwidget.h"

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

} // namespace olylumogui
