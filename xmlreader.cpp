#include <algorithm>

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

Xml::XmlInfo Xml::XmlReader::getInfo() {
    return xml_info;
}

void Xml::XmlReader::parse_clients() {
    while(!xml.atEnd() && !xml.hasError()) {
        auto token = xml.readNext();

        if(token == QXmlStreamReader::StartDocument)
            continue;
        if(token == QXmlStreamReader::StartElement)
            parce_package();
        if(token == QXmlStreamReader::EndDocument) {

            ///
            /// Обсудить надо
            ///

            xml_info.parce_data.unite(insert_person.subtract(update_person));
            xml_info.parce_data.unite(update_person);
            xml_info.parce_data = xml_info.parce_data.subtract(delete_person);
        }
    }
    if(xml.hasError()) {
        QSourceLocation sl;
        sl.setLine(xml.lineNumber());
        sl.setColumn(xml.columnNumber());
        m_handle->message(QtMsgType::QtWarningMsg, xml.errorString(), {}, sl);
        qWarning() << Q_FUNC_INFO << "Xml parsing error: " << QObject::tr("%1, line: %2, number%3")
                   .arg(dynamic_cast<MessageHandle*>(m_handle)->get_error())
                   .arg(dynamic_cast<MessageHandle*>(m_handle)->get_line())
                   .arg(dynamic_cast<MessageHandle*>(m_handle)->get_column()
                        ) << this;
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
    //qDebug() << Q_FUNC_INFO << "batch detected with context:"<< batch_context << this;
}

void Xml::XmlReader::parce_item() {
    auto attrs = xml.attributes();
    if(batch_context == "insert") {
       insert_person.insert(parce_item_attrs(attrs));
    };
    if(batch_context == "refresh") {
        update_person.insert(parce_item_attrs(attrs));
    }
    if(batch_context == "delete") {
        delete_person.insert(parce_item_attrs(attrs));
    }
}

Core::Person Xml::XmlReader::parce_item_attrs(QXmlStreamAttributes& attrs) {

    Core::Person person;
    person.tab_number = attrs.value("tab_number").toInt();

    if(attrs.hasAttribute("pass_number"))
        person.pass_number = attrs.value("pass_number").toString();
    if(attrs.hasAttribute("discount_percent"))
        person.discount = attrs.value("discount_percent").toInt();
    if(attrs.hasAttribute("pass_card"))
        person.image_url = attrs.value("pass_card").toString();
    if(attrs.hasAttribute("name"))
        person.full_name = attrs.value("name").toString();
    //qDebug() << Q_FUNC_INFO << "item detected:"<< person.tab_number << this;
    return person;
}
