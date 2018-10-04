#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QUrl>

namespace Core {

    struct Person {
        int tab_number; // Внутренний номер
        QString full_name; // Полное имя
        QString pass_number; // Номер пропуска
        QUrl image_url; // Ссылка на картинку (Какую хотите)
        int discount = 0; // Скидка

        inline bool operator<(const Person& obj) const {
            return this->tab_number<obj.tab_number;
        }

        inline bool operator==(const Person& obj) const {
           return this->tab_number==obj.tab_number;
        }
    };

    inline uint qHash(const Person& person) {
        return ::qHash(person.tab_number);
    }
}
#endif // PERSON_H
