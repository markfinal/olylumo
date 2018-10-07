#include "scenemodel.h"

#include "QtCore/QFile"
#include "QtCore/QDebug"

namespace olylumogui
{

struct DomItem
{
    DomItem(
        QDomNode &inNode,
        const int inRow,
        DomItem *inParent)
        :
        _node(inNode),
        _rowIndex(inRow),
        _parent(inParent)
    {}

    ~DomItem()
    {
        for (auto childIt = this->_childItems.begin(); childIt != this->_childItems.end(); ++childIt)
        {
            delete childIt.value();
        }
    }

    DomItem *
    child(
        const int inRow)
    {
        if (this->_childItems.contains(inRow))
        {
            return this->_childItems[inRow];
        }

        if (inRow >= 0 && inRow < this->_node.childNodes().count())
        {
            auto childNode = this->_node.childNodes().item(inRow);
            auto childItem = new DomItem(childNode, inRow, this);
            this->_childItems[inRow] = childItem;
            return childItem;
        }

        return nullptr;
    }

    QDomNode             _node;
    int                  _rowIndex;
    DomItem             *_parent;
    QHash<int, DomItem*> _childItems;
};

SceneModel::SceneModel(
    const QString &inPath)
{
    QFile scene_file(inPath);
    if (!scene_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw "Cannot open path";
    }
    this->_doc.setContent(&scene_file);
    scene_file.close();
    qDebug() << this->_doc.toString();

    this->_root.reset(new DomItem(this->_doc, 0, nullptr));
}

SceneModel::~SceneModel() = default;

QModelIndex SceneModel::index(int row, int column, const QModelIndex & parent) const
{
    if (!this->hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    DomItem *parentItem;
    if (!parent.isValid())
    {
        parentItem = this->_root.get();
    }
    else
    {
        parentItem = static_cast<DomItem*>(parent.internalPointer());
    }

    auto childItem = parentItem->child(row);
    if (nullptr != childItem)
    {
        return this->createIndex(row, column, childItem);
    }
    return QModelIndex();
}

QModelIndex SceneModel::parent(const QModelIndex & child) const
{
    if (!child.isValid())
    {
        return QModelIndex();
    }

    auto childItem = static_cast<DomItem*>(child.internalPointer());
    auto parentItem = childItem->_parent;

    if (nullptr == parentItem || parentItem == this->_root.get())
    {
        return QModelIndex();
    }

    return this->createIndex(parentItem->_rowIndex, 0, parentItem);
}

int SceneModel::rowCount(const QModelIndex & parent) const
{
    if (parent.column() > 0)
    {
        return 0;
    }

    DomItem *parentItem;
    if (!parent.isValid())
    {
        parentItem = this->_root.get();
    }
    else
    {
        parentItem = static_cast<DomItem*>(parent.internalPointer());
    }

    return parentItem->_node.childNodes().count();
}

int SceneModel::columnCount(const QModelIndex & parent) const
{
    (void)parent;
    return 1;
}

QVariant SceneModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    if (Qt::DisplayRole != role)
    {
        return QVariant();
    }
    auto item = static_cast<DomItem*>(index.internalPointer());
    auto node = item->_node;
    return node.nodeName();
}

} // namespace olylumogui
