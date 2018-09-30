#include "viewerwidget.h"

#include "QtWidgets/QMdiArea"
#include "QtWidgets/QLabel"

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
    inParent->addSubWindow(this);

    this->_image_label = new QLabel(this);
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
