#include "aaserver.h"

#include <QDataStream>

AAServer::AAServer(QObject *parent, HashTable *htable, QTextBrowser *chatWidget) : QTcpServer(parent) {
    _users = new QStringList();
    _htable = htable;
    _chatWidget = chatWidget;
}

void AAServer::newUser() {
    //передаем дескрпитор сокета, указатель на сервер (для вызова его методов), и стандартный параметр - parent
    AAClient *client = new AAClient(this, this->nextPendingConnection(), _htable);
    //подключаем сигналы напрямую к виджету, если его передали в конструктор сервера
    connect(client, SIGNAL(addUserToGui(QString)), this, SLOT(on_addUserToGui(QString)));
    connect(client, SIGNAL(removeUserFromGui(QString)), this, SLOT(on_removeUserFromGui(QString)));
    connect(client, SIGNAL(messageToGui(QString)), this, SLOT(on_messageToGui(QString)));
    connect(client, SIGNAL(removeUser(AAClient*)), this, SLOT(on_removeUser(AAClient*)));
    connect(client, SIGNAL(doSendListOfUsersOnline(QTcpSocket*)), this, SLOT(doSendListOfUsersOnline(QTcpSocket*)));
    connect(client, SIGNAL(doSendMessageHistory(QTcpSocket*)), this, SLOT(doSendMessageHistory(QTcpSocket*)));
    connect(client, SIGNAL(doSendMessageToUsers(QString,QString)), this, SLOT(doSendMessageToUsers(QString,QString)));
    _clients.append(client);
    emit messageToGui("A new connection!");
    qDebug() << "A new connection!";
}

/*
При рассылке сообщения всем нужно делать проверку авторизован ли текущий пользователь, ибо в массиве _clients,
возможно, находятся не авторизованные
*/
void AAServer::doSendServerMessageToUsers(QString message) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << AAClient::comPublicServerMessage << message;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    //отправка сообщения всем (тут отсутствует проверка, ибо все пользователи в _users гарантированно авторизованы)
    for (int j = 0; j < _clients.length(); ++j)
        if (_users->contains(_clients.at(j)->getUsername()))
            _clients.at(j)->_socket->write(block);
}

void AAServer::doSendListOfUsersOnline(QTcpSocket *_sock) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << AAClient::comUsersOnline << *_users;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
    qDebug() << "wrote to socket " << *_users;
}

void AAServer::doSendMessageHistory(QTcpSocket *_sock) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << AAClient::comGetMessageHistory << _chatWidget->toPlainText();
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sock->write(block);
}

void AAServer::doSendMessageToUsers(QString message, QString fromUsername) {
    _chatWidget->append(fromUsername + ": " + message);

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << AAClient::comPublicMessage << message << fromUsername;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    //отправка сообщения всем (тут отсутствует проверка, ибо все пользователи в _users гарантированно авторизованы)
    for (int j = 0; j < _clients.length(); ++j)
        if (_users->contains(_clients.at(j)->getUsername()))
            _clients.at(j)->_socket->write(block);
}

bool AAServer::doStartServer(QHostAddress _IP, qint16 _Port) {
    if (this->listen(_IP,_Port)) {
        qDebug() << QString::fromUtf8("Server successfully started!");
        connect(this, SIGNAL(newConnection()), this, SLOT(newUser()));
        return true;
    }
    else {
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(this->errorString());
        return false;
    }
}

void AAServer::doServerInterrupt() {
    _htable->Save(QString::QString("htable.rc"));
    for (int j = 0; j < _clients.length(); ++j)
        _clients.at(j)->_socket->close();
    _clients.clear();
}

void AAServer::on_addUserToGui(QString username) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << AAClient::comUserJoin << username;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    //отправка сообщения всем (тут отсутствует проверка, ибо все пользователи в _users гарантированно авторизованы)
    for (int j = 0; j < _clients.length(); ++j)
        if (_users->contains(_clients.at(j)->getUsername()))
            _clients.at(j)->_socket->write(block);

    *_users << username;
    emit addUserToGui(username);
}

void AAServer::on_removeUserFromGui(QString username) {
    for (int i = 0; i < _users->size(); ++i) {
        if (_users->at(i) == username) {
            _users->removeAt(i);
            break;
        }
    }
    emit removeUserFromGui(_users);

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << AAClient::comUserLeft << *_users;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    for (int j = 0; j < _clients.length(); ++j)
        if (_users->contains(_clients.at(j)->getUsername()))
            _clients.at(j)->_socket->write(block);
}

void AAServer::on_messageToGui(QString message) {
    emit messageToGui(message);
}

void AAServer::on_removeUser(AAClient *client) {
    _clients.removeOne(client);
}
