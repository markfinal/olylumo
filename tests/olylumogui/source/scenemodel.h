#ifndef OLYLUMOGUI_MODEL_H
#define OLYLUMOGUI_MODEL_H

#include "QtCore/QAbstractItemModel"
#include "QtXml/QDomDocument"

#include <memory>

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
        const QString &inPath);
    ~SceneModel();

    // Inherited via QAbstractItemModel
    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex & child) const override;
    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    int columnCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

private:
    QDomDocument             _doc;
    std::unique_ptr<DomItem> _root;
};

} // namespace olylumogui

#endif // OLYLUMOGUI_MODEL_H
