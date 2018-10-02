#include "imageitem.h"
#include <QDebug>
#include <QPainter>

Images::ImageItem::ImageItem(QQuickItem *parent) : QQuickPaintedItem(parent) {
    qDebug() << Q_FUNC_INFO << "initializing ImageItem" << this;
    setImage(QImage(":/resources/image/logo.png"));
}

QImage Images::ImageItem::image() const {
    qDebug() << Q_FUNC_INFO << "image requested..." << this;
    return m_image;
}

void Images::ImageItem::setImage(const QImage &image) {
    if(image == m_image)
        return;
     qDebug() << Q_FUNC_INFO << "setting new image." << this;
    m_image = image;
    emit imageChanged();
    update();
}

void Images::ImageItem::paint(QPainter *painter) {
    if(m_image.isNull())
        return;

    qDebug() << Q_FUNC_INFO << "paint requested..." << this;

    QRectF bounding_rect = boundingRect();
    QImage scaled = m_image.scaledToHeight(static_cast<int>(bounding_rect.height()));
    QPointF center = bounding_rect.topLeft() - scaled.rect().center();

    if(center.x() < 0)
        center.setX(0);
    if(center.y() < 0)
        center.setY(0);
    painter->drawImage(center, scaled);
}
