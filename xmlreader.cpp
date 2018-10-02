#include "xmlreader.h"
#include "core.h"

#include <QXmlStreamAttributes>
#include <QXmlStreamReader>

Xml::XmlReader::XmlReader(QObject *parent) : QObject(parent)
{

}

bool Xml::XmlReader::setXmlFile(QString& file) {
    QFile xml_file(file);
    if(!xml_file.open(QIODevice::ReadOnly)) {
        qCritical() << Q_FUNC_INFO << "can not open file:" << file << this;
        return false;
    }

    xml.clear();
    insert_person.clear();
    update_person.clear();
    delete_person.clear();

    xml.addData(xml_file.readAll());
    xml_info.xml_info.setFile(xml_file);
    return true;
}

Xml::XmlInfo Xml::XmlReader::parse_clients() {
    while(!xml.atEnd() && !xml.hasError()) {
        auto token = xml.readNext();

        if(token == QXmlStreamReader::StartDocument)
            continue;
        if(token == QXmlStreamReader::StartElement)
            parce_package();
        if(token == QXmlStreamReader::EndDocument) {
//            xml_info.parce_data.insert(insert_person);
//            xml_info
        }
    }
}

void Xml::XmlReader::parce_package() {
    if(xml.name() == "package") {
        auto attrs = xml.attributes();
        xml_info.id = attrs.value("id").toInt();
        parce_batch();
    }
    if(xml.name() == "batch") {
        parce_batch();
    }
    if(xml.name() == "item") {
        parce_item();
    }
}

void Xml::XmlReader::parce_batch() {
    auto attrs = xml.attributes();
    batch_context = attrs.value("command").toString();
}

void Xml::XmlReader::parce_item() {
    auto attrs = xml.attributes();
    if(batch_context == "insert") {
       insert_person.insert(parce_item_attrs(attrs));
    };
    if(batch_context == "update") {
        insert_person.insert(parce_item_attrs(attrs));
    }
    if(batch_context == "delete") {
        insert_person.insert(parce_item_attrs(attrs));
    }
}

Core::Person Xml::XmlReader::parce_item_attrs(QXmlStreamAttributes& ) {
    return {};
}
