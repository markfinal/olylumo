#include "scenewidget.h"
#include "scenemodel.h"

#include "QtGui/QMouseEvent"
#include "QtWidgets/QHeaderView"

namespace olylumogui
{

SceneWidget::SceneWidget(
    SceneModel *inModel)
{
    this->setWindowTitle("Scene");
    this->setModel(inModel);
    this->header()->hide();
}

void
SceneWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    auto index = this->indexAt(event->pos());
    if (!index.isValid())
    {
        return;
    }

    qobject_cast<SceneModel*>(this->model())->double_click(index);
}

} // namespace olylumogui
