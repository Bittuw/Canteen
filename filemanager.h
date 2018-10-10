#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QSet>
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
        QString getDir();
    signals:
    public slots:
    private:
        QString base_dir = QDir::currentPath() + "/data/";
        QDir directory = base_dir;
        QSet<QString> existing_files;
        Q_DISABLE_COPY(FileManager)
    };
}
#endif // FILEMANAGER_H
