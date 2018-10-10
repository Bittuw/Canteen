#include "filemanager.h"

#include <QDirIterator>
#include <QStringList>
#include <QDebug>

FileManagment::FileManager::FileManager(QObject *parent) : QObject(parent)
{
    if(!directory.exists())
        QDir::current().mkdir("data");
    else {
        QDirIterator it(
                    directory.absolutePath(),
                    QStringList() << "*.xml" << ".png",
                    QDir::Files, QDirIterator::NoIteratorFlags
                    );

        while(it.hasNext()) {
            it.next();
            existing_files.insert(it.fileName());
        }
    }

}

QString FileManagment::FileManager::save_file(QString abs_file_path) {
    QFileInfo old_file(abs_file_path);
    if(directory.exists(old_file.fileName()) && existing_files.contains(old_file.fileName()))
        return old_file.fileName();
    else {
        auto file_name = old_file.fileName();
        auto result = QFile::copy(old_file.absoluteFilePath(), directory.absolutePath() + "/" + old_file.fileName());
        qDebug() << Q_FUNC_INFO
                 << (result ? QObject::tr("file %1 created").arg(file_name) : QObject::tr("file %1 not created").arg(file_name))
                 << this;
        if(result) existing_files.insert(file_name);
        return result ? file_name : "";
    }
}

void FileManagment::FileManager::delete_file(QString file_name) {
    if(directory.exists(file_name) && existing_files.contains(file_name)) {
        QFile file(base_dir + file_name);
        file.remove();
        existing_files.remove(file_name);
    }
}

bool FileManagment::FileManager::exist_file(QString file_name) {
    return directory.exists(QFile(file_name).fileName());
}

QFile FileManagment::FileManager::get_file(QString file_name) {
    if(directory.exists(QFile(file_name).fileName()))
        return { file_name };
    else
        return {};
}

QFileInfo FileManagment::FileManager::get_file_info(QString file_name) {
    if(directory.exists(QFile(file_name).fileName()))
        return { directory.filePath(file_name) };
    else
        return {};
}

QString FileManagment::FileManager::getDir() {
    return directory.absolutePath();
}
