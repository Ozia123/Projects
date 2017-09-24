#ifndef AACLIENT_H
#define AACLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "hashtable.h"

class AAClient : public QObject {
    Q_OBJECT

    friend class AAServer;

private:
    QTcpSocket *_socket; //сокет
    quint16 _blockSize; //текущий размер блока
    QString _username; //имя
    QString _hash;
    HashTable *_htable;
    bool _isAutched; //флаг авторизации

public:
    // константы
    static const qint8 comUserSigningError      = 10;
    static const qint8 comUserAutorizationError = 11;
    static const qint8 comGetMessageHistory     =  0;
    static const qint8 comUserAutorized         =  1;
    static const qint8 comUserSigned            =  2;
    static const qint8 comUsersOnline           =  3;
    static const qint8 comPublicServerMessage   =  4;
    static const qint8 comPublicMessage         =  5;
    static const qint8 comUserJoin              =  6;
    static const qint8 comUserLeft              =  7;

    explicit AAClient(QObject *parent = nullptr);
    explicit AAClient(QObject *parent, QTcpSocket *_sock, HashTable *htable);
    ~AAClient() {}

    QString getUsername() const {return _username;}
    bool getAutched() const {return _isAutched;}

signals:
    //сигналы для обновления гуи
    void addUserToGui(QString username);
    void removeUserFromGui(QString username);
    void messageToGui(QString message);
    //сигнал удаления пользователя из QList
    void removeUser(AAClient *client);
    
    void doSendListOfUsersOnline(QTcpSocket *_sock);
    void doSendMessageHistory(QTcpSocket *_sock);
    void doSendMessageToUsers(QString message, QString fromUsername);

private slots:
    void on_readyRead();
    void on_disconnected();
};

#endif // AACLIENT_H
