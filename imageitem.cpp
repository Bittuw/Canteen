#include "imageitem.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QPainter>
#include <QPixmap>
#include <QDebug>

Images::ImageItem::ImageItem(QQuickItem *parent) : QQuickPaintedItem(parent) {
    qDebug() << Q_FUNC_INFO << "initializing ImageItem" << this;
    setImage(QImage(":/resources/image/logo.png"));
}

//void Images::ImageItem::Install() {
//    qmlRegisterType<Images::ImageItem>("bittumworker.org", 1, 0, "DynamicImage");
//}

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

//    QRectF bounding_rect = boundingRect();
//    QImage scaled = m_image.scaledToHeight(static_cast<int>(bounding_rect.height()));
//    QPointF center = boundingRect().topLeft();/* - scaled.rect().center();*/

//    if(center.x() < 0)
//        center.setX(0);
//    if(center.y() < 0)
//        center.setY(0);
    //painter->drawImage(center, scaled);

    painter->drawImage(
                boundingRect().topLeft(),
                m_image.scaled(
                    static_cast<int>(width()),
                    static_cast<int>(height())
                           )
                );

//    painter->drawPixmap(
//                boundingRect().topLeft(),
//                QPixmap::fromImage(m_image)
//                    .scaled(
//                        static_cast<int>(width()),
//                        static_cast<int>(height())
//                    )
//                );
}

//void Images::Provider::Install(const QQmlApplicationEngine& engine) {
//    engine.rootContext()->setContextProperty("DIProvider", this);
//}
