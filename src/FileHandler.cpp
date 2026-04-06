#include "FileHandler.hpp"


FileHandler::FileHandler() {}

QString FileHandler::fileRename(const QString &tempFilePath)
{
    // Убираем .temp из названия файла
    QString cleanPath = tempFilePath;
    if(cleanPath.endsWith(".temp")) {
        cleanPath.chop(5);
    }

    QFileInfo fileInfo(cleanPath);
    QString baseName = fileInfo.baseName();
    QString baseSuf = fileInfo.completeSuffix();
    QString dirPath = fileInfo.path();

    if (QFile::exists(cleanPath)) {
        int count = 1;
        do {
            cleanPath = QString("%1/%2 (%3).%4")
                        .arg(dirPath)
                        .arg(baseName)
                        .arg(count++)
                        .arg(baseSuf);
        } while (QFile::exists(cleanPath));
    }

    return cleanPath;
}

void FileHandler::mkSavingDir()
{
    // Директория для хранения файлов
    QDir saving_dir;
    if (!saving_dir.exists("file_downloads")) {
        saving_dir.mkdir("file_downloads");
        qDebug() << "Создана директория для скачанных файлов.";
    }
}


