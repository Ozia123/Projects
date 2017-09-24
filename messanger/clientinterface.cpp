#include "clientinterface.h"
#include "ui_clientinterface.h"

ClientInterface::ClientInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientInterface) {
    ui->setupUi(this);
    _socket = new QTcpSocket(this);
    //connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(on_displayError(QAbstractSocket::SocketError)));

    CWnd = new ConnectionWindow(this);
    CErrWnd = nullptr;
    LWnd = nullptr;
    _blockSize = 0;
    connect(CWnd, SIGNAL(ConnectToHost(QString&,QString&)), this, SLOT(ConnectToHost(QString&,QString&)));
    CWnd->show();
}

ClientInterface::~ClientInterface() {
    delete ui;
}

// private slots:

void ClientInterface::ConnectToHost(QString &_IP, QString &_Port) {
    _socket->connectToHost(_IP, _Port.toInt());

    if (CWnd) {
        CWnd->close();
        CWnd = nullptr;
    }
    if (CErrWnd) {
        CErrWnd->close();
        CErrWnd = nullptr;
    }

    if (!_socket->waitForConnected(3000)) {
        CErrWnd = new reconnectionwindow(this);
        CErrWnd->FillUpFields(_IP, _Port, _socket->errorString());
        connect(CErrWnd, SIGNAL(ConnectToHost(QString&,QString&)), this, SLOT(ConnectToHost(QString&,QString&)));
        CErrWnd->show();
        return;
    }

    IP = _IP;
    Port = _Port;
    LWnd = new LoginWindow(this);
    connect(LWnd, SIGNAL(TryToLogin(QString&,QString&)), this, SLOT(TryToLogin(QString&,QString&)));
    connect(LWnd, SIGNAL(TryToSignUp(QString&,QString&)), this, SLOT(TryToSignUp(QString&,QString&)));
    connect(_socket, SIGNAL(connected()), this, SLOT(on_connected()));
    connect(_socket, SIGNAL(disconnected()), this, SLOT(on_disconnected()));
    connect(_socket,SIGNAL(readyRead()),this,SLOT(on_readyRead()));
    LWnd->show();
}

void ClientInterface::TryToLogin(QString &_Username, QString &_Hash) {
    //после подключения следует отправить запрос на авторизацию
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    //резервируем 2 байта для размера блока. Класс AAClient используется в реализации сервера, но тут используем статические члены этого класса - константы команд
    //третий байт - команда
    out << (quint16)0 << (quint8)ClientInterface::comUserAutorized << _Username << _Hash;
    //возваращаемся в начало
    out.device()->seek(0);
    //вписываем размер блока на зарезервированное место
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);

    Username = _Username;
    Hash = _Hash;
}

void ClientInterface::TryToSignUp(QString &_Username, QString &_Hash) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << (quint8)ClientInterface::comUserSigned << _Username << _Hash;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);
}

void ClientInterface::AbleToSend() {
    if (ui->textEdit->toPlainText().isEmpty()) {
        ui->send->setEnabled(false);
        ui->label_WriteAMessage->show();
        return;
    }
    ui->send->setEnabled(true);
    ui->label_WriteAMessage->hide();
}

void ClientInterface::on_send_clicked() {
    QString  message = ui->textEdit->toPlainText();
    while (message.at(message.length() - 1) == (QChar)'\n') {
        message.remove(message.length() - 1, 1);
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << (quint8)ClientInterface::comPublicMessage << message;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _socket->write(block);

    ui->textEdit->setText("");
}

void ClientInterface::on_settings_clicked() {
    SWnd = new SettingsWindow(this);
    SWnd->show();
}

// public slots:

void ClientInterface::on_connected() {
    qDebug() << "connected!";
}

void ClientInterface::on_disconnected() {
    qDebug() << "disconnected!";
}

void ClientInterface::on_readyRead() {
    //тут обрабатываются данные от сервера
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

    switch (command) {
    // Errors:
    case ClientInterface::comUserAutorizationError: {
        qDebug() << "comUserAutorizationError";
        LWnd->ErrWnd(ClientInterface::comUserAutorizationError);
    }
    break;

    case ClientInterface::comUserSigningError: {
        qDebug() << "comUserSigningError";
        LWnd->ErrWnd(ClientInterface::comUserSigningError);
    }
    break;


    // Normal log
    case ClientInterface::comUserAutorized: {
        // блок с авторизацией
        ui->Username->setText(Username);
        ui->IP->setText(IP);
        ui->Port->setText(Port);
        ui->send->setEnabled(false);
        connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(AbleToSend()));
        LWnd->close();
        this->show();

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0 << (quint8)ClientInterface::comUsersOnline;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        _socket->write(block);
    }
    break;

    case ClientInterface::comUserSigned: {
        // блок с регистрацией
        LWnd->ShowLoginForm();
    }
    break;

    //сервер отправит список пользователей, если авторизация пройдена, в таком случае третий байт равен константе MyClient::comUsersOnline
    case ClientInterface::comUsersOnline: {
        qDebug() << "sended users list";
        QStringList users;
        in >> users;

        //обновляем гуи
        ui->listWidget->addItems(users);

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0 << (quint8)ClientInterface::comGetMessageHistory;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        _socket->write(block);
    }
    break;

    case ClientInterface::comUserJoin: {
        qDebug() << "sended user";
        QString user;
        in >> user;

        //обновляем гуи
        ui->listWidget->addItem(user);
    }
    break;

    case ClientInterface::comUserLeft: {
        qDebug() << "Remove user";
        QStringList users;
        in >> users;
        ui->listWidget->clear();
        ui->listWidget->addItems(users);
    }
    break;

    //общее сообщение от сервера
    case ClientInterface::comPublicServerMessage: {
        //считываем и добавляем в лог
        QString message;
        in >> message;
        ui->ChatArea->append("[PublicServerMessage]: " + message);
        //AddToLog("[PublicServerMessage]: "+message, Qt::red);
    }
    break;

    case ClientInterface::comPublicMessage: {
        QString message;
        QString fromUsername;
        in >> message >> fromUsername;
        ui->ChatArea->append(fromUsername + ": " + message);
    }
    break;

    case ClientInterface::comGetMessageHistory: {
        QString buffer;
        in >> buffer;
        ui->ChatArea->append(buffer);
    }
    break;


    }
}
