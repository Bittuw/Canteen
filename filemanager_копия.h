#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QDir>

namespace FileManagment {

    class FileManager : public QObject
    {
        Q_OBJECT
    public:
        explicit FileManager(QObject *parent = nullptr);
        QString save_file(QString);
        void delete_file(QString);
        bool exist_file(QString);
        QFile get_file(QString);
        QFileInfo get_file_info(QString);
    signals:

    public slots:
    private:
        QString base_dir = QDir::currentPath() + "/data/";
        Q_DISABLE_COPY(FileManager)
    };
}
#endif // FILEMANAGER_H
