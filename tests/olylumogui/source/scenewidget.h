#ifndef OLYLUMOGUI_SCENEWIDGET_H
#define OLYLUMOGUI_SCENEWIDGET_H

#include "QtWidgets/QTreeView"

namespace olylumogui
{

class SceneModel;

class SceneWidget final :
    public QTreeView
{
    Q_OBJECT

public:
    SceneWidget(
        SceneModel *inModel);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
};

} // namespace olylumogui

#endif // OLYLUMOGUI_SCENEWIDGET_H
