#ifndef OLYLUMOGUI_SCENEWIDGET_H
#define OLYLUMOGUI_SCENEWIDGET_H

#include "QtWidgets/QTreeView"

namespace olylumogui
{

class SceneWidget final :
    public QTreeView
{
    Q_OBJECT

public:
    SceneWidget();
};

} // namespace olylumogui

#endif // OLYLUMOGUI_SCENEWIDGET_H
