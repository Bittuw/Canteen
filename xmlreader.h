#ifndef XMLREADER_H
#define XMLREADER_H

#include "person.h"

#include <QtXmlPatterns/QAbstractMessageHandler>
#include <QXmlStreamReader>
#include <QFileInfo>
#include <QObject>
#include <QDate>
#include <QSet>

namespace Xml {

    struct XmlInfo {
        QSet<Core::Person> parce_data;
        QFileInfo xml_info;
        QDate date;
        int id;
    };

    struct MessageHandle : public QAbstractMessageHandler {
        Q_OBJECT
    public:
        MessageHandle(QObject *parent = nullptr)
            : QAbstractMessageHandler(parent)
        {
        }
        bool has_error() {return m_has_error;}
        QString get_error() { return m_error;}
        int get_line() { return static_cast<int>(m_sourcelocation.line());}
        int get_column(){ return static_cast<int>(m_sourcelocation.column());}
        QtMsgType get_error_type() {return m_error_type;}

    protected:
        virtual void handleMessage(
                QtMsgType error_type,
                const QString& error,
                const QUrl& identifier,
                const QSourceLocation& sourcelocation
                )
        {
            Q_UNUSED(identifier)
            m_error = error;
            m_has_error = true;
            m_error_type = error_type;
            m_sourcelocation = sourcelocation;
        }

    private:
        QString m_error;
        QSourceLocation m_sourcelocation;
        QtMsgType m_error_type;
        bool m_has_error = false;
        Q_DISABLE_COPY(MessageHandle)
    };

    class XmlReader : public QObject
    {
        Q_OBJECT
    public:
        explicit XmlReader(QObject *parent = nullptr);
        void parse_clients();
        XmlInfo getInfo();

        void setHandle(QAbstractMessageHandler* handle) {m_handle = handle;}
        bool setXmlFile(QString&);
    signals:

    public slots:
    private:
        QAbstractMessageHandler* m_handle;

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
