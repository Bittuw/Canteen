#include "statistic.h"

#include <QXmlStreamAttributes>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDebug>
#include <QDate>
#include <QFile>

Statistics::SalesReport::SalesReport(QObject *parent) : QObject(parent)
{
    if(!QDir(default_path).exists())
        QDir::current().mkdir("statistic");
}


bool Statistics::SalesReport::add(const Core::Person& person) {
    if(!next_writeble.contains(person)) {
        next_writeble.insert(person);
        return true;
    }
    else
        return false;
}

QString Statistics::SalesReport::flush(QString date, QString start_time, QString end_time) {
    QFile xml_file(default_path + QStringLiteral("report_") + date);

    qDebug() << Q_FUNC_INFO << "flushing statistic into" << xml_file.fileName() << this;
    xml_file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QXmlStreamWriter stream(&xml_file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement(QStringLiteral("package"));
    stream.writeAttribute(QStringLiteral("id"),date);
    stream.writeAttribute(QStringLiteral("xmlns:xsi"),QStringLiteral("http://www.w3.org/2001/XMLSchema-instance"));
    stream.writeAttribute(QStringLiteral("xmlns:xsd"),QStringLiteral("http://www.w3.org/2001/XMLSchema"));
    stream.writeAttribute(QStringLiteral("SAP_SYSTEM"),QStringLiteral("UTE"));

    {
        stream.writeStartElement(QStringLiteral("batch"));
        stream.writeAttribute(QStringLiteral("command"), QStringLiteral("insert"));
        stream.writeAttribute(QStringLiteral("entity"), QStringLiteral("report"));

        {
            stream.writeEmptyElement(QStringLiteral("item"));
            stream.writeAttribute(QStringLiteral("id"), date);
            stream.writeAttribute(QStringLiteral("start"),start_time);
            stream.writeAttribute(QStringLiteral("finish"),end_time);
        }

        stream.writeEndElement();
    }
    for(auto& person : next_writeble) {
        stream.writeEmptyElement("item");
        QXmlStreamAttributes attrs;

        attrs.append("tab_number", QString::number(person.tab_number));
        attrs.append("name", person.full_name);
        attrs.append("pass_number", person.pass_number);

        stream.writeAttributes(attrs);
    }

    stream.writeEndElement();
    stream.writeEndDocument();


    next_writeble.clear();
    xml_file.close();

    return QFileInfo(xml_file).absoluteFilePath();
}


void Statistics::SalesReport::reestablish(QString file) {
    auto person_file = file;

    if(QDir(default_path).exists(person_file)) {

        next_writeble.clear();

        QFile xml_file(default_path + person_file);
        xml_file.open(QIODevice::ReadOnly);

        QXmlStreamReader stream(&xml_file);
        stream.readNext();

        while(!stream.atEnd() || !stream.hasError()) {
            auto token = stream.readNext();

            if(token == QXmlStreamReader::StartElement)
            {
                Core::Person person;
                auto attrs = stream.attributes();

                person.tab_number = attrs.value("tab_number").toInt();
                person.full_name = attrs.value("name").toString();
                person.pass_number = attrs.value("pass_number").toString();

                next_writeble.insert(person);
            }
        }
    }
}

bool Statistics::SalesReport::contains(const Core::Person& person) {
    return next_writeble.contains(person);
}

//void Statistic::FtpStatistic::transparent(QSet<Core::Person> persons, QString file_path, QString date) {
//    qDebug() << Q_FUNC_INFO << "flushing statistic into" << file_path << this;

//    QFile xml_file(file_path);
//    xml_file.open(QIODevice::WriteOnly | QIODevice::Truncate);

//    QXmlStreamWriter stream(&xml_file);
//    stream.setAutoFormatting(true);
//    stream.writeStartDocument();

//    stream.writeStartElement("package");
//    stream.writeAttribute("id", date);
//    stream.writeAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
//    stream.writeAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
//    stream.writeAttribute("SAP_SYSTEM", "UTE");

//    stream.writeStartElement("batch");
//    stream.writeAttribute("command", "insert");
//    stream.writeAttribute("entity", "bill");

//    for(auto& person : persons) {
//        stream.writeEmptyElement("item");
//        QXmlStreamAttributes attrs;

//        attrs.append("tab_number", QString::number(person.tab_number));
//        attrs.append("name", person.full_name);
//        attrs.append("pass_number", person.pass_number);

//        stream.writeAttributes(attrs);
//    }

//    stream.writeEndElement();
//    stream.writeEndElement();
//    stream.writeEndDocument();
//    //next_writeble.clear();
//}

