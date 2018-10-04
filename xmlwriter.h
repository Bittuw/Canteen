#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <QXmlStreamAttributes>
#include <QXmlStreamWriter>
#include <QObject>

namespace Xml {
    class XmlWriter : public QObject
    {
        Q_OBJECT
    public:
        explicit XmlWriter(QObject *parent = nullptr);

    signals:

    public slots:
    private:
//        QXmlStreamWriter xml;
    };
}
#endif // XMLWRITER_H
