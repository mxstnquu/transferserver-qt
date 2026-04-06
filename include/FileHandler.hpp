#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP
#include <QString>
#include <QDebug>
#include <QFile>
#include <QDir>


class FileHandler
{
    FileHandler();
    ~FileHandler();

public:
    static QString fileRename(const QString &filename);
    static void mkSavingDir();
};

#endif // FILEHANDLER_HPP
