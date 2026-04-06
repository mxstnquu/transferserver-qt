#include "Server.hpp"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MainServer server;

    return a.exec();
}
