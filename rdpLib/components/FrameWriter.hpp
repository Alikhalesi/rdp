#pragma once
#include <QObject>

#include <QtQuick/QQuickPaintedItem>
#include "rdpLib_global.h"
#include <QPainter>
#include <QSGSimpleTextureNode>
class RDPLIB_EXPORT ImageWriter : public QQuickItem
{
    Q_OBJECT

public:

    ImageWriter(QQuickItem *parent = 0):QQuickItem(parent){};
    QSGNode* updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData) override;

    void SetImageData( std::pair<unsigned char*,unsigned int> data);
    QImage image_; //your image
private:


};
