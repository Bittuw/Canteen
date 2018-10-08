#include "statistic.h"

#include <QXmlStreamAttributes>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDebug>
#include <QDate>
#include <QFile>

Statistic::Statistic::Statistic(QObject *parent) : QObject(parent)
{
    if(!QDir(default_path).exists())
        QDir::current().mkdir("statistic");
}


bool Statistic::Statistic::add(const Core::Person& person) {
    if(!next_writeble.contains(person)) {
        next_writeble.insert(person);
        return true;
    }
    else
        return false;
}

void Statistic::Statistic::flush(QString file) {

    qDebug() << Q_FUNC_INFO << "flushing statistic into" << file << this;

    QFile xml_file(default_path + file);
    xml_file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QXmlStreamWriter stream(&xml_file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    for(auto& person : next_writeble) {
        stream.writeEmptyElement("item");
        QXmlStreamAttributes attrs;

        attrs.append("tab_number", QString::number(person.tab_number));
        attrs.append("name", person.full_name);
        attrs.append("pass_number", person.pass_number);

        stream.writeAttributes(attrs);
    }

    stream.writeEndDocument();
    next_writeble.clear();
}


void Statistic::Statistic::reestablish(QString file) {
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

bool Statistic::Statistic::contains(const Core::Person& person) {
    return next_writeble.contains(person);
}

