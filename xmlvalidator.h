#ifndef XMLVALIDATOR_H
#define XMLVALIDATOR_H

#include <QtXmlPatterns/QtXmlPatterns>
#include <QObject>
#include <QFile>


namespace Xml {

    class MessageHandler : public QAbstractMessageHandler
    {
        Q_OBJECT
    public:
        MessageHandler(QObject *parent = nullptr)
            : QAbstractMessageHandler(parent)
        {
        }

        QString statusMessage() const
        {
            return m_description;
        }

        int line() const
        {
            return static_cast<int>(m_sourceLocation.line());
        }

        int column() const
        {
            return static_cast<int>(m_sourceLocation.column());
        }

    protected:
        virtual void handleMessage(QtMsgType type, const QString &description,
                                   const QUrl &identifier, const QSourceLocation &sourceLocation)
        {
            Q_UNUSED(type);
            Q_UNUSED(identifier);

            m_description = description;
            m_sourceLocation = sourceLocation;
        }

    private:
        QString m_description;
        QSourceLocation m_sourceLocation;
    };

    class XmlValidator : public QObject
    {
        Q_OBJECT
    public:
        explicit XmlValidator(QObject *parent = nullptr);
        bool validate(QString instance_file);
        bool loadSchema(QString schema);

    signals:

    public slots:

    private:
        QXmlSchemaValidator validator;
        MessageHandler messageHandler;
        QString current_schema;
    };

}
#endif // XMLVALIDATOR_H
