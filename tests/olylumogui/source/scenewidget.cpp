#include "scenewidget.h"
#include "scenemodel.h"

namespace olylumogui
{

SceneWidget::SceneWidget(
    SceneModel *inModel)
{
    this->setWindowTitle("Scene");
    this->setModel(inModel);
    this->expandAll();
}

} // namespace olylumogui
