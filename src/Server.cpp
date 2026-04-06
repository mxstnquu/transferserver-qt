#include <Server.hpp>
#include <FileHandler.hpp>


MainServer::MainServer()
{
    if (this->listen(QHostAddress::Any, 6953)) {
        qDebug() << "Start server, port: 6953 \n";
    }
    else {
        qDebug() << "Error while starting server";
    }
}

MainServer::~MainServer()
{
    for (auto context : clients) {
        delete context;
    }
    clients.clear();
}

void MainServer::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket* socket = new QTcpSocket(this);
    // Дескриптор из аргумента ф.
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        qDebug() << "Ошибка привязки сокета к дескриптору.";
        delete socket;
        return;
    }

    ClientContext* clientContext = new ClientContext();
    clients[socket] = clientContext;

    connect(socket,
            SIGNAL(readyRead()),
            this,
            SLOT(slotReadyRead()));

    connect(socket,
            &QTcpSocket::disconnected,
            this,
            &MainServer::slotClientDisconnected);

    connect(socket,
            &QTcpSocket::disconnected,
            socket,
            &QTcpSocket::deleteLater);

    qDebug() << "Client connected. Socket descriptor: "
             << socketDescriptor << "\n";
}

void MainServer::slotReadyRead()
{
    //socket, с которого пришел запрос
    QTcpSocket* socket = (QTcpSocket*)sender();
    if (!socket) return;

    ClientContext* clientContext = clients.value(socket);
    if (!clientContext) return;
    auto& ctx = *clientContext;

    while (socket->bytesAvailable() > 0) {
        // if для приема тела файла
        if (ctx.isReceivingFile) {
            while (socket -> bytesAvailable() > 0){
                qint64 available = socket -> bytesAvailable();
                qint64 leftToRead = ctx.totalExp - ctx.totalRec;
                qint64 toRead = qMin(available, leftToRead);

                QByteArray chunk = socket -> read(toRead);
                if(ctx.targetFile && ctx.targetFile -> isOpen()) {
                    ctx.targetFile -> write(chunk);
                }
                ctx.totalRec += chunk.size();

                // тут завершение скачивания файла
                if(ctx.totalRec >= ctx.totalExp) {
                    ctx.currentFileName = ctx.targetFile->fileName();
                    ctx.targetFile -> close();

                    QString finalName = ctx.currentFileName;

                    finalName = FileHandler::fileRename(finalName);
                    QFile::rename(ctx.currentFileName, finalName);
                    qDebug() << "Файл сохранен как: " << finalName;

                    delete ctx.targetFile;
                    ctx.targetFile = nullptr;
                    ctx.isReceivingFile = false;
                    ctx.nextBlockSize = 0;
                    qDebug() << "Файл принят.";
                    break;
                }
            }
            continue;
        }

        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_6_2);

        if (ctx.nextBlockSize == 0) {
            if(socket->bytesAvailable() < sizeof(quint32)) {
                qDebug() << "Data < quint32. Break.";
                return;
            }
            in >> ctx.nextBlockSize;
        }

        if(socket->bytesAvailable() < ctx.nextBlockSize) {
            qDebug() << "Data is not full. Break.";
            return;
        }

        quint8 data_type;
        in >> data_type;

        // if work with file
        if (data_type == 1) {
            // Проверка наличия директории для скачанных файлов
            FileHandler::mkSavingDir();

            QString file_name;
            qint64 file_size;
            in >> file_name >> file_size;
            ctx.currentFileName = file_name;

            ctx.isReceivingFile = true;
            ctx.totalExp = file_size;
            ctx.totalRec = 0;
            ctx.nextBlockSize = 0;

            ctx.targetFile = new QFile("file_downloads/" + file_name + ".temp");
            if(!ctx.targetFile -> open(QIODevice::WriteOnly)) {
                qDebug() << "Ошибка создания файла на диске.";
                ctx.isReceivingFile = false;
                return;
            }

            qDebug() << "Ожидаем файл: " << file_name << ". Размер: " << file_size;
        }
        // if work w message
        else if (data_type == 0) {
            QString msg;
            in >> msg;

            qDebug() << "Получено сообщение:" << msg;
            sendToClient(msg);
            ctx.nextBlockSize = 0;
        }
    }
}

void MainServer::slotClientDisconnected()
{
    // Указатель на сокет, который сгенерировал disconnected
    QTcpSocket* disconnectedSocket = (QTcpSocket*)sender();

    if(disconnectedSocket) {
        qDebug() << "Клиент отключился от сервера. Дескриптор освобожден.";
    }
    else return;

    // Очистка context
    if (clients.contains(disconnectedSocket)) {
        ClientContext* ctx = clients.take(disconnectedSocket);
        if (ctx->targetFile) {
            ctx->targetFile->close();
            delete ctx->targetFile;
        }
        delete ctx;
    }
}


void MainServer::sendToClient(QString str)
{
    Data.clear();

    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);

    out << qint32(0) << str;
    out.device()->seek(0);
    out << qint32(Data.size() - sizeof(qint32));

    for (QTcpSocket* socket : clients.keys()) {
        socket->write(Data);
    }
}







