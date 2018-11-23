#include "statistic.h"

#include <algorithm>

#include <QXmlStreamAttributes>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDateTime>
#include <QSettings>
#include <QDebug>
#include <QDate>
#include <QFile>
#include <QSet>

Statistics::SalesReport::SalesReport(QObject *parent) : QObject(parent)
{
    if(!QDir(default_path).exists())
        QDir::current().mkdir("statistic");
    QSettings ftp_settings(QDir::currentPath() + "/ftp.ini", QSettings::IniFormat);
//    m_pass_through_numbering = ftp_settings.value("FTP/ptn").toInt();
    m_complex_old = ftp_settings.value("FTP/cost").toInt();
    m_complex = m_complex_old;
}


bool Statistics::SalesReport::add(const Core::Person& person) {
    if(!current_persons.contains(person)) {
        //current_persons.insert(person);
        current_persons.append(person);
        return true;
    }
    else
        return false;
}

Core::Person& Statistics::SalesReport::get_person(Core::Person& person) {
    auto result = std::find(current_persons.begin(), current_persons.end(), person);
    if (result != current_persons.end())
        return const_cast<Core::Person&>(*result);
    else
        return *result;
}

QString Statistics::SalesReport::flush_sales_report(QString date, QString start_time, QString end_time) {
    auto xml_file_path = default_path + QStringLiteral("report_full_") + date + ".xml";
    return flush_report(xml_file_path, date, start_time, end_time, current_persons);
}

QString Statistics::SalesReport::flush_abort(QString date, QString start_time, QString end_time) {
    Q_UNUSED(start_time)
    Q_UNUSED(end_time)

    QFile xml_file(default_path + QStringLiteral("report_") + date + QStringLiteral(".xml"));

    qDebug() << Q_FUNC_INFO << "flushing statistic into" << xml_file.fileName() << this;
    xml_file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QXmlStreamWriter stream(&xml_file);
    stream.setCodec("UTF-8");

    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("wrapper");

        auto id_bill = 0;

        for(auto& person : current_persons) {

            id_bill++;

            stream.writeEmptyElement("item");
            QXmlStreamAttributes attrs;

            attrs.append(QStringLiteral("tab_number"), QString::number(person.tab_number));
            attrs.append(QStringLiteral("name"), person.full_name);
            attrs.append(QStringLiteral("pass_number"), person.pass_number);
            attrs.append(QStringLiteral("discount"), QString::number(person.discount));
            attrs.append(QStringLiteral("time"), person.time);

            stream.writeAttributes(attrs);
        }

    stream.writeEndElement();
    stream.writeEndDocument();

    xml_file.close();

    return QFileInfo(xml_file).absoluteFilePath();
}

QString Statistics::SalesReport::flush_menu(QString date, QString start_time, QString end_time) {
    QFile xml_file(default_path + QStringLiteral("pricelist_") + date + ".xml");

    qInfo() << Q_FUNC_INFO << "flushing statistic into" << xml_file.fileName() << this;

    xml_file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QXmlStreamWriter stream(&xml_file);
    stream.setCodec("UTF-8");

    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement(QStringLiteral("package"));
    stream.writeAttribute(QStringLiteral("id"),date);
    stream.writeAttribute(QStringLiteral("xmlns:xsi"),QStringLiteral("http://www.w3.org/2001/XMLSchema-instance"));
    stream.writeAttribute(QStringLiteral("xmlns:xsd"),QStringLiteral("http://www.w3.org/2001/XMLSchema"));
    stream.writeAttribute(QStringLiteral("SAP_SYSTEM"),QStringLiteral("UTE"));

    stream.writeStartElement(QStringLiteral("batch"));
    stream.writeAttribute(QStringLiteral("command"), QStringLiteral("insert"));
    stream.writeAttribute(QStringLiteral("entity"), QStringLiteral("complex"));

    stream.writeEmptyElement("item");
    stream.writeAttribute(QStringLiteral("id"), QString::number(1));
    auto complex = "Комплексный обед";
    stream.writeAttribute(QStringLiteral("name"), complex);
    stream.writeAttribute(QStringLiteral("start"), start_time);
    stream.writeAttribute(QStringLiteral("finish"), end_time);
    stream.writeAttribute(QStringLiteral("price"), QString::number(m_complex_old));

    stream.writeEndElement();
    stream.writeEndElement();
    stream.writeEndDocument();

    xml_file.close();

    return QFileInfo(xml_file).absoluteFilePath();
}

QString Statistics::SalesReport::flush_transition_sales_report(QString date, QString start_time, QString end_time) {
    auto hour = QDateTime::fromString(start_time, "yyyy-MM-dd hh:mm:ss").toString("hh");
    date = date + "_" + hour;
    auto xml_file_path = default_path + QStringLiteral("report_") + date + ".xml";
    QVector<Core::Person> temp_cp_to_xml;
    for(auto& person: current_persons) {
        if(QDateTime::fromString(person.time, "yyyy-MM-dd hh:mm:ss").time().hour() == hour.toInt())
            temp_cp_to_xml.append(person);
    }
    return flush_report(xml_file_path, date, start_time, end_time, temp_cp_to_xml);
}

void Statistics::SalesReport::reestablish(QString file) {
    auto person_file = default_path + file + ".xml";

    if(QDir(default_path).exists(file + ".xml")) {

        current_persons.clear();

        QFile xml_file(person_file);
        xml_file.open(QIODevice::ReadOnly);

        QXmlStreamReader stream(&xml_file);

        while(!stream.atEnd()) {
            auto token = stream.readNext();

            if(token == QXmlStreamReader::StartElement && stream.name() == "item")
            {
                Core::Person person;
                auto attrs = stream.attributes();

                person.tab_number = attrs.value("tab_number").toInt();
                person.full_name = attrs.value("name").toString();
                person.pass_number = attrs.value("pass_number").toString();
                person.discount = attrs.value("discount").toInt();
                person.time = attrs.value("time").toString();

                current_persons.append(person);
            }
        }
    }
}

void Statistics::SalesReport::set_complex(quint16 complex) {
    m_complex = complex;
}

void Statistics::SalesReport::clear() {
    current_persons.clear();
    m_complex_old = m_complex;
    QSettings ftp_settings(QDir::currentPath() + "/ftp.ini", QSettings::IniFormat);
    ftp_settings.setValue("FTP/cost", m_complex_old);
}

bool Statistics::SalesReport::contains(const Core::Person& person) {
    return current_persons.contains(person);
}

QString Statistics::SalesReport::flush_report(QString xml_file_path, QString date, QString start_time, QString end_time, QVector<Core::Person>& person_set) {
    QFile xml_file(xml_file_path);

    qInfo() << Q_FUNC_INFO << "Flush statistic into" << xml_file.fileName() << this;
    xml_file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QXmlStreamWriter stream(&xml_file);
    stream.setCodec("UTF-8");

    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement(QStringLiteral("package"));
    stream.writeAttribute(QStringLiteral("id"), date);
    stream.writeAttribute(QStringLiteral("xmlns:xsi"),QStringLiteral("http://www.w3.org/2001/XMLSchema-instance"));
    stream.writeAttribute(QStringLiteral("xmlns:xsd"),QStringLiteral("http://www.w3.org/2001/XMLSchema"));
    stream.writeAttribute(QStringLiteral("SAP_SYSTEM"),QStringLiteral("UTE"));

    /// batch report
    {
        stream.writeStartElement(QStringLiteral("batch"));
        stream.writeAttribute(QStringLiteral("command"), QStringLiteral("insert"));
        stream.writeAttribute(QStringLiteral("entity"), QStringLiteral("report"));

        {
            stream.writeEmptyElement(QStringLiteral("item"));

            stream.writeAttribute(QStringLiteral("id"), date);
            stream.writeAttribute(QStringLiteral("start"), start_time);
            stream.writeAttribute(QStringLiteral("finish"), end_time);
        }

        stream.writeEndElement();
    }

    auto id_bill = 0;

    /// batch bill
    {
        stream.writeStartElement("batch");

        stream.writeAttribute(QStringLiteral("command"), QStringLiteral("insert"));
        stream.writeAttribute(QStringLiteral("entity"), QStringLiteral("bill"));


        for(auto& person : person_set) {
            id_bill++;

            stream.writeEmptyElement("item");
            QXmlStreamAttributes attrs;

            attrs.append(QStringLiteral("id"), QString::number(id_bill));
            attrs.append(QStringLiteral("amount"), QString::number(m_complex_old - (m_complex_old/100) * person.discount ));
            attrs.append("tab_number", QString::number(person.tab_number));
            attrs.append("bill_datetime", person.time);
            attrs.append("pass_number", person.pass_number);

            attrs.append("cash_register", QStringLiteral(""));
            attrs.append("shift_number", QStringLiteral(""));
            attrs.append("bill_number", QStringLiteral(""));

            stream.writeAttributes(attrs);
        }
        stream.writeEndElement();
    }

    /// batch line
    {
        stream.writeStartElement("batch");
        stream.writeAttribute(QStringLiteral("command"), QStringLiteral("insert"));
        stream.writeAttribute(QStringLiteral("entity"), QStringLiteral("line"));

        auto id_line = 0;

        for(auto& person : person_set) {
            id_bill++;
            id_line++;

            stream.writeEmptyElement("item");
            QXmlStreamAttributes attrs;

            attrs.append(QStringLiteral("id"), QString::number(id_bill));
            attrs.append(QStringLiteral("bill_id"), QString::number(id_line));
            attrs.append(QStringLiteral("dish_id"), QString::number(1));
            attrs.append(QStringLiteral("name"), "Комплексный обед");
            attrs.append(QStringLiteral("quantity"), QString::number(1));
            attrs.append(QStringLiteral("price"), QString::number(m_complex_old));
            attrs.append(QStringLiteral("amount"), QString::number(m_complex_old));
            attrs.append(QStringLiteral("discount_percent"), QString::number(person.discount));
            attrs.append(QStringLiteral("total_amount"), QString::number(static_cast<double>(m_complex_old - (m_complex_old/100) * person.discount )));
            attrs.append(QStringLiteral("volume"), QStringLiteral(""));

            stream.writeAttributes(attrs);
        }

        stream.writeEndElement();
    }


    stream.writeEndElement();
    stream.writeEndDocument();

    xml_file.close();

    return QFileInfo(xml_file).absoluteFilePath();
}

