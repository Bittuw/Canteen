#ifndef XMLREADER_H
#define XMLREADER_H

#include <core.h>

#include <QXmlStreamReader>
#include <QFileInfo>
#include <QObject>
#include <QDate>
#include <QSet>

namespace Xml {

    struct XmlInfo {
        QList<Core::Person> parce_data;
        QFileInfo xml_info;
        QDate date;
        int id;
    };

    class XmlReader : public QObject
    {
        Q_OBJECT
    public:
        explicit XmlReader(QObject *parent = nullptr);
        bool setXmlFile(QString&);
        XmlInfo parse_clients();

    signals:

    public slots:
    private:
        QList<Core::Person> temp_list;
        QXmlStreamReader xml;
        XmlInfo xml_info;
        QString batch_context;

        QSet<Core::Person> insert_person;
        QSet<Core::Person> update_person;
        QSet<Core::Person> delete_person;

        void parce_package();
        void parce_batch();
        void parce_item();
        Core::Person parce_item_attrs(QXmlStreamAttributes&);
    };
}
#endif // XMLREADER_H
