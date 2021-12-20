#include "FrameWriter.hpp"
//=============================================================================
//=============================================================================
void ImageWriter::SetImageData(std::pair<unsigned char *, unsigned int> data)
{
   image_.loadFromData(data.first,data.second,"JPG");
}
//=============================================================================
QSGNode* ImageWriter::updatePaintNode(
    QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData) {

    auto node = dynamic_cast<QSGSimpleTextureNode *>(oldNode);

    if (!node) {
        node = new QSGSimpleTextureNode();
    }

    QSGTexture *texture = window()->createTextureFromImage(image_, QQuickWindow::TextureIsOpaque);
    node->setOwnsTexture(true);
    node->setRect(boundingRect());
    node->markDirty(QSGNode::DirtyForceUpdate);
    node->setTexture(texture);
    return node;
}
