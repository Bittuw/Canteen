#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include <QObject>

class TextField : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ getText NOTIFY changeText)
public:
    TextField(QObject* parent = nullptr) : QObject(parent) {}
    QString getText() const {
        return text_;
    }
    void setText(QString text) {
        text_ = text;
        emit changeText();
    }
signals:
    void changeText();
public slots:

private:
    QString text_;
    Q_DISABLE_COPY(TextField)
};

#endif // TEXTFIELD_H
