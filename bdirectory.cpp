#include "bdirectory.h"

BDirectory::BDirectory() :
    directory(0)
{
    SORT_FLAGS = (QDir::Name | QDir::LocaleAware);
    FILTER_FLAGS = QDir::Files;
    directory = QDir::current();
    LoadFileInfos();
}

BDirectory::~BDirectory() {

}

BDirectory::file_iterator BDirectory::file_begin() const {
    return fileInfosList.begin();
}

BDirectory::file_iterator BDirectory::file_end() const {
    return fileInfosList.end();
}

void BDirectory::SetPath(std::wstring path) {
    QString dir_path = QString::fromStdWString(path);
    if (directory.exists(dir_path)) {
        directory.setPath(dir_path);
        LoadFileInfos();
    } else {
        throw new std::exception();
    }
}

void BDirectory::LoadFileInfos() {
    fileInfosList = directory.entryInfoList(FILTER_FLAGS, SORT_FLAGS);
}

