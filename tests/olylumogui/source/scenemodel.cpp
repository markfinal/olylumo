#include "scenemodel.h"

#include "olylumoray/scene.h"
#include "olylumoray/vec4.h"
#include "olylumoray/rgba.h"

#include "QtCore/QFile"
#include "QtCore/QDebug"
#include "QtGui/QColor"
#include "QtWidgets/QColorDialog"

#include <cassert>

namespace
{

QColor
node_to_qcolor(
    const QDomNode &inNode)
{
    auto value = inNode.firstChild().nodeValue();
    auto split = value.split(' ');
    assert(4 == split.length());
    return QColor::fromRgbF(
        split[0].toFloat(),
        split[1].toFloat(),
        split[2].toFloat(),
        split[3].toFloat()
    );
}

void
set_node_value_as_colour(
    QDomNode &inNode,
    const QColor &inColour)
{
    QString colour = QString("%1 %2 %3 %4").arg(inColour.redF()).arg(inColour.greenF()).arg(inColour.blueF()).arg(inColour.alphaF());
    inNode.firstChild().setNodeValue(colour);
}

olylumoray::RGBA
node_to_rgba(
    const QDomNode &inNode)
{
    auto value = inNode.firstChild().nodeValue();
    auto split = value.split(' ');
    assert(4 == split.length());
    return olylumoray::RGBA(
        split[0].toFloat(),
        split[1].toFloat(),
        split[2].toFloat(),
        split[3].toFloat()
    );
}

olylumoray::Vec4
node_to_vec4(
    const QDomNode &inNode)
{
    auto value = inNode.firstChild().nodeValue();
    auto split = value.split(' ');
    assert(4 == split.length());
    return olylumoray::Vec4(
        split[0].toFloat(),
        split[1].toFloat(),
        split[2].toFloat(),
        split[3].toFloat()
    );
}

float
node_to_float(
    const QDomNode &inNode)
{
    auto value = inNode.firstChild().nodeValue();
    return value.toFloat();
}

} // anonymous namespace

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
    const QString &inPath,
    olylumoray::Scene &inScene)
    :
    _scene(inScene)
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
    this->sync_to_scene(_scene);
}

SceneModel::~SceneModel() = default;

void
SceneModel::sync_to_scene(
    olylumoray::Scene &outScene)
{
    outScene.clear();

    auto scene = this->_doc.firstChild();
    auto environment = scene.firstChild();

    {
        auto gradient = environment.firstChild();
        auto top = gradient.firstChild();
        assert(top.attributes().contains("id") && top.attributes().namedItem("id").nodeValue() == "top");
        auto bottom = top.nextSibling();
        assert(bottom.attributes().contains("id") && bottom.attributes().namedItem("id").nodeValue() == "bottom");

        outScene.set_environment_gradient(
            node_to_rgba(top),
            node_to_rgba(bottom)
        );
    }

    {
        auto world = environment.nextSibling();
        auto hitable = world.firstChild();
        while (!hitable.isNull())
        {
            if (hitable.nodeName() == "sphere")
            {
                olylumoray::Vec4 position{ 0,0,0,1 };
                float radius = 1.0f;
                std::string material_name = "Lambertian";
                olylumoray::RGBA albedo{ 1,1,1,1 };
                float roughness = 0.0f;
                auto properties = hitable.firstChild();
                while (!properties.isNull())
                {
                    if (properties.nodeName() == "position")
                    {
                        position = node_to_vec4(properties);
                    }
                    else if (properties.nodeName() == "radius")
                    {
                        radius = node_to_float(properties);
                    }
                    else if (properties.nodeName() == "material")
                    {
                        if (properties.attributes().contains("type"))
                        {
                            material_name = properties.attributes().namedItem("type").nodeValue().toStdString();
                        }

                        auto material_properties = properties.firstChild();
                        while (!material_properties.isNull())
                        {
                            if (material_properties.nodeName() == "colour")
                            {
                                if (material_properties.attributes().contains("id"))
                                {
                                    if (material_properties.attributes().namedItem("id").nodeValue() == "albedo")
                                    {
                                        albedo = node_to_rgba(material_properties);
                                    }
                                }
                            }
                            else if (material_properties.nodeName() == "float")
                            {
                                if (material_properties.attributes().contains("id"))
                                {
                                    if (material_properties.attributes().namedItem("id").nodeValue() == "roughness")
                                    {
                                        roughness = node_to_float(material_properties);
                                    }
                                }
                            }
                            material_properties = material_properties.nextSibling();
                        }
                    }
                    properties = properties.nextSibling();
                }

                outScene.append_sphere(
                    position,
                    radius,
                    material_name,
                    albedo,
                    roughness
                );
            }

            hitable = hitable.nextSibling();
        }
    }
}

void
SceneModel::double_click(
    const QModelIndex &inIndex)
{
    auto item = static_cast<DomItem*>(inIndex.internalPointer());
    if (item->_node.nodeName() == "colour")
    {
        auto new_color = QColorDialog::getColor(node_to_qcolor(item->_node));
        if (new_color.isValid())
        {
            set_node_value_as_colour(item->_node, new_color);
            emit this->dataChanged(inIndex, inIndex);
            this->sync_to_scene(this->_scene);
            emit this->scene_changed();
        }
    }
}

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
    auto item = static_cast<DomItem*>(index.internalPointer());
    auto node = item->_node;
    auto node_name = node.nodeName();
    QString id;
    if (node.attributes().contains("id"))
    {
        id = node.attributes().namedItem("id").nodeValue();
    }
    switch (role)
    {
    case Qt::DisplayRole:
        {
            if (!id.isEmpty())
            {
                return id;
            }
            else
            {
                return node_name;
            }
        }
        break;

    case Qt::DecorationRole:
        {
            if ("colour" == node_name)
            {
                return QVariant::fromValue(node_to_qcolor(node));
            }
        }
        break;

    default:
        return QVariant();
    }
    return QVariant();
}

} // namespace olylumogui
