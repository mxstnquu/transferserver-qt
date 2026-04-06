#ifndef SERVER_HPP
#define SERVER_HPP

#include <QTcpServer>
#include <QTcpSocket>
#include <QDir>
#include <QFile>


class MainServer : public QTcpServer
{
    Q_OBJECT

public:
    MainServer();
    ~MainServer();

    struct ClientContext {
        bool isReceivingFile = false;
        qint32 nextBlockSize = 0;
        qint64 totalExp = 0;
        qint64 totalRec = 0;
        QFile *targetFile = nullptr;
        QString currentFileName;
    };

private:
    QMap<QTcpSocket*, ClientContext*> clients;

    // Данные между сокетами передаются через
    QByteArray Data;
    void sendToClient(QString send_string);

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
    void slotClientDisconnected();
};


#endif // SERVER_HPP
