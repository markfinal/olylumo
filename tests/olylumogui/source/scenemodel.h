#ifndef OLYLUMOGUI_MODEL_H
#define OLYLUMOGUI_MODEL_H

#include "QtCore/QAbstractItemModel"
#include "QtXml/QDomDocument"

#include <memory>

namespace olylumoray
{

class Scene;

} // namespace olylumoray

namespace olylumogui
{

struct DomItem;

// based on http://doc.qt.io/qt-5/qtwidgets-itemviews-simpledommodel-example.html
class SceneModel final :
    public QAbstractItemModel
{
    Q_OBJECT

public:
    SceneModel(
        olylumoray::Scene &inScene);
    ~SceneModel();

    void
    load(
        const QString &inPath);

    void
    double_click(
        const QModelIndex &inIndex);

    // Inherited via QAbstractItemModel
    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex & child) const override;
    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    int columnCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

signals:
    void scene_changed();

private:
    void
    sync_to_scene(
        olylumoray::Scene &outScene);

private:
    QDomDocument             _doc;
    std::unique_ptr<DomItem> _root;
    olylumoray::Scene       &_scene;
};

} // namespace olylumogui

#endif // OLYLUMOGUI_MODEL_H
