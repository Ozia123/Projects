#ifndef CLIENTINTERFACE_H
#define CLIENTINTERFACE_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDataStream>
#include <QDebug>

#include "connectionwindow.h"
#include "reconnectionwindow.h"
#include "loginwindow.h"
#include "settingswindow.h"

namespace Ui {
class ClientInterface;
}

class ClientInterface : public QMainWindow {
    Q_OBJECT

private:
    Ui::ClientInterface *ui;

    // network
    QTcpSocket        *_socket;
    quint16            _blockSize;
    QString             buffer;

    // windows
    ConnectionWindow   *CWnd;
    reconnectionwindow *CErrWnd;
    LoginWindow        *LWnd;
    SettingsWindow     *SWnd;

    // information
    QString             IP;
    QString             Port;
    QString             Username;
    QString             Hash;

public:
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

    explicit ClientInterface(QWidget *parent = 0);
    ~ClientInterface();

private slots:
    void ConnectToHost(QString& _IP,QString& _Port);
    void TryToLogin(QString &_Username, QString &_Hash);
    void TryToSignUp(QString &_Username, QString &_Hash);
    void AbleToSend();
    void on_send_clicked();
    void on_settings_clicked();

public slots:
    void on_connected();
    void on_disconnected();
    void on_readyRead();

};

#endif // CLIENTINTERFACE_H
