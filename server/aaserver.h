#ifndef AASERVER_H
#define AASERVER_H

#include <QObject>
#include <QAbstractSocket>
#include <QTcpServer>
#include <QTextBrowser>
#include <QDebug>
#include "aaclient.h"
#include "hashtable.h"

class AAServer : public QTcpServer {
    Q_OBJECT

private:
    QList<AAClient *> _clients; //список пользователей
    QStringList *_users; // список имён авторизованых пользователей
    HashTable *_htable;
    QTextBrowser *_chatWidget;

public:
    explicit AAServer(QObject *parent, HashTable *htable, QTextBrowser *chatWidget);

    bool doStartServer(QHostAddress _IP, qint16 _Port);
    void doServerInterrupt();
    void doSendServerMessageToUsers(QString message); //приватное серверное сообщение
    bool isNameUsed(QString username) const; //проверить используется ли имя

private slots:
    void on_addUserToGui(QString username); // добавляем пользователя в гуи сервера, отправляем в гуи пользователей нового пользователя
    void on_removeUserFromGui(QString username);
    void on_messageToGui(QString message);

    void on_removeUser(AAClient *client);

    void newUser();

    void doSendListOfUsersOnline(QTcpSocket *_sock);
    void doSendMessageHistory(QTcpSocket *_sock);
    void doSendMessageToUsers(QString message, QString fromUsername);

signals:
    void addUserToGui(QString username);
    void removeUserFromGui(QStringList *users);
    void messageToGui(QString message);

};

#endif // AASERVER_H
