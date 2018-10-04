#include "filemanager.h"

FileManagment::FileManager::FileManager(QObject *parent) : QObject(parent)
{
    if(!QDir(base_dir).exists())
        QDir::current().mkdir("data");
}

QString FileManagment::FileManager::save_file(QString abs_file_path) {
    QFileInfo old_file(abs_file_path);
    auto str = old_file.fileName();
    if(QFile(base_dir + old_file.fileName()).exists())
        return old_file.fileName();
    QFile::copy(old_file.absoluteFilePath(), base_dir +  old_file.fileName());
    QFile new_file(base_dir + old_file.fileName());
    return old_file.fileName();
}

void FileManagment::FileManager::delete_file(QString file_name) {
    QFile file(base_dir + file_name);
    file.remove();
}

bool FileManagment::FileManager::exist_file(QString file_name) {
    return QFile(base_dir + file_name).exists();
}

QFile FileManagment::FileManager::get_file(QString file_name) {
    if(QFile(base_dir + file_name).exists())
        return { file_name };
    else
        return {};
}

QFileInfo FileManagment::FileManager::get_file_info(QString file_name) {
    QFile file(base_dir + file_name);
    if(file.exists())
        return {file};
    else
        return {};
}
