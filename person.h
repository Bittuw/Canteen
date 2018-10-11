#ifndef PERSON_H
#define PERSON_H

#include <QObject>
#include <QString>
#include <QUrl>

namespace Core {

    struct Enums : public QObject {
        Q_OBJECT
    public:
        enum class FirstRes {
            FOUND = 1,
            NOT_FOUND = 2
        };

        enum class SecondRes {
            ALLOWED = 1,
            FORBIDDEN = 2,
            NOT_FOUND = 3
        };

        Q_ENUM(FirstRes)
        Q_ENUM(SecondRes)
        Q_DISABLE_COPY(Enums)
    };

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
