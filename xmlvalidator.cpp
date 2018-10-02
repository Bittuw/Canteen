#include "xmlvalidator.h"

#include <QByteArray>
#include <QDebug>

Xml::XmlValidator::XmlValidator(QObject *parent) : QObject(parent),
    default_path(QDir::currentPath() + "/schemas/")
{
    if(!QDir(default_path).exists())
        qCritical() << Q_FUNC_INFO << "directory 'schemas' is not exist!" << this;
}

bool Xml::XmlValidator::validate(QString instance_file_apath) {

    QFile instance(instance_file_apath);


    if(!instance.open(QIODevice::ReadOnly)) {
        qCritical() << Q_FUNC_INFO << "can not open instance file:" << instance.fileName() << this;
        return false;
    }


    const auto instanceData = instance.readAll();

    if(!validator.validate(instanceData)) {
        qWarning() << QObject::tr("Warning: %1, line: %2, column: %2")
            .arg(messageHandler.statusMessage())
            .arg(messageHandler.line())
            .arg(messageHandler.column()
                 );
        return false;
    }

    return true;
}

bool Xml::XmlValidator::loadSchema(QString schema_name) {
    QFile schema_file;

    schema_file.setFileName(default_path + schema_name + ".xsd");
    if(!schema_file.open(QIODevice::ReadOnly)) {
        qCritical() << Q_FUNC_INFO << "can not open schema file:" << schema_file.fileName() << this;
        return false;
    }

    const auto schema_data = schema_file.readAll();

    QXmlSchema schema;
    schema.setMessageHandler(&messageHandler);
    schema.load(schema_data);

    if(!schema.isValid()) {
        qCritical() << Q_FUNC_INFO << "not valid schema:" << schema_name << this;
        return false;
    }

    validator.setSchema(schema);
    current_schema = schema_name;
    return true;
}
