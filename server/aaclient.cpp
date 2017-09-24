#include "aaclient.h"
#include <QDataStream>

AAClient::AAClient(QObject *parent) : QObject(parent) {

}

AAClient::AAClient(QObject *parent, QTcpSocket *_sock, HashTable *htable) : QObject(parent) {
    emit messageToGui("A new connection!");
    _username = "";
    _socket = _sock;
    _htable = htable;
    _isAutched = false;
    _blockSize = 0;
    connect(_socket,SIGNAL(readyRead()),this, SLOT(on_readyRead()));
    connect(_socket,SIGNAL(disconnected()),this,SLOT(on_disconnected()));
}

void AAClient::on_readyRead() {
    //тут обрабатываются данные от клиента
    qDebug() << "Ready read";
    QDataStream in(_socket);
    //если считываем новый блок первые 2 байта это его размер
    if (_blockSize == 0) {
        //если пришло меньше 2 байт ждем пока будет 2 байта
        if (_socket->bytesAvailable() < (int)sizeof(quint16))
            return;
        //считываем размер (2 байта)
        in >> _blockSize;
    }

    //ждем пока блок прийдет полностью
    if (_socket->bytesAvailable() < _blockSize)
       return;
    else
        //можно принимать новый блок
        _blockSize = 0;

    //3 байт - команда серверу
    quint8 command;
    in >> command;
    qDebug() << "command is " << QString::number(command);

    switch (command) {

    case AAClient::comUserAutorized: {
        in >> _username >> _hash;

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);

        // проверяем на авторизацию
        if (_htable->SearchFor(_hash,_username)) {
            emit messageToGui("Entered user: " + _username + " " + _hash);
            emit addUserToGui(_username);
            qDebug() << "Entered user: " << _username << " " << _hash;
            out << (quint16)0 << (quint8)AAClient::comUserAutorized;
            _isAutched = true;
        }
        else {
            emit messageToGui("Unsuccessful attempt of login: " + _username + " " + _hash);
            qDebug() << "Unsuccessful attempt of login: " << _username << " " << _hash;
            out << (quint16)0 << (quint8)AAClient::comUserAutorizationError;
        }

        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        _socket->write(block);
    }
    break;

    case AAClient::comUserSigned: {
        in >> _username >> _hash;
        
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);

        if (!_htable->Add(_hash,_username)) {
            emit messageToGui("Unsuccessful attempt of signing: " + _username + " " + _hash);
            qDebug() << "Unsuccessful attempt of signing: " << _username << " " << _hash;
            out << (quint16)0 << (quint8)AAClient::comUserSigningError;
        }
        else {
            emit messageToGui("Signed user: " + _username + " " + _hash);
            qDebug() << "Signed user: " << _username << " " << _hash;
            out << (quint16)0 << (quint8)AAClient::comUserSigned;
        }

        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        _socket->write(block);
    }
    break;

    case AAClient::comUsersOnline: {
        qDebug() << "request from client to send users list";
        emit doSendListOfUsersOnline(_socket);
    }
    break;

    case AAClient::comGetMessageHistory: {
        qDebug() << "request from client to get message history";
        emit doSendMessageHistory(_socket);
    }
    break;

    case AAClient::comPublicMessage: {
        QString message;
        in >> message;
        emit doSendMessageToUsers(message, _username);
    }
    break;

    }
}

void AAClient::on_disconnected() {
    emit messageToGui("User " + _username + " disconected.");
    emit removeUserFromGui(_username);
    emit removeUser(this);
}
