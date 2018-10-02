#ifndef PROVIDER_H
#define PROVIDER_H

#include <QImage>
#include <QQuickPaintedItem>

namespace Images {

    class ImageItem : public QQuickPaintedItem {
        Q_OBJECT
        Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    public:
        ImageItem(QQuickItem *parent = nullptr);
        QImage image() const;
        void setImage(const QImage &image);
        void paint(QPainter *painter);
    signals:
        void imageChanged();
    private:
        QImage m_image;
    };

    class Provider: public QObject {
        Q_OBJECT
        Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    public:
        Provider(QObject *parent = nullptr) : QObject(parent) {}
        QImage image() const {return m_image;}
        void setImage(const  QImage &image) {
            if(m_image == image)
                return;
            m_image = image;
            emit imageChanged();
        }
    signals:
        void imageChanged();
    private:
        QImage m_image;
    };

}
#endif // PROVIDER_H
