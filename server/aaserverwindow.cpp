#include "aaserverwindow.h"
#include "ui_aaserverwindow.h"

AAServerWindow::AAServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AAServerWindow) {
    ui->setupUi(this);

    _htable = new HashTable();
    ui->Send->setEnabled(false);
    ui->Stop->setEnabled(false);
}

AAServerWindow::~AAServerWindow() {
    delete ui;
}

void AAServerWindow::on_actionHashTable_triggered() {
    HTWnd = new HTWindow(this, _htable);
    HTWnd->show();
}

void AAServerWindow::on_Start_clicked() {
    _server = new AAServer(this, _htable, ui->chatArea);
    if (_server->doStartServer(QHostAddress::Any, 12345)) {
        ui->Start->setEnabled(false);
        ui->Send->setEnabled(true);
        ui->Stop->setEnabled(true);
        ui->textinfo->append(QString::fromUtf8("Server successfully started!\n"));
        connect(_server, SIGNAL(addUserToGui(QString)), this, SLOT(on_addUserToGui(QString)));
        connect(_server, SIGNAL(removeUserFromGui(QStringList*)), this, SLOT(on_removeUserFromGui(QStringList*)));
        connect(_server, SIGNAL(messageToGui(QString)), this, SLOT(on_messageToGui(QString)));
    }
    else {
        ui->textinfo->append("Unable to start the server: " + _server->errorString());
    }
}

void AAServerWindow::on_addUserToGui(QString username) {
    ui->users->addItem(username);
}

void AAServerWindow::on_removeUserFromGui(QStringList *_users) {
    ui->users->clear();
    ui->users->addItems(*_users);
}

void AAServerWindow::on_messageToGui(QString message) {
    ui->textinfo->append(message);
}

void AAServerWindow::on_Stop_clicked() {
    _server->doServerInterrupt();
    _server->close();
    ui->textinfo->append(QString::fromUtf8("\nServer stoped!\n"));
    qDebug() << QString::fromUtf8("Server stoped!");
    ui->Start->setEnabled(true);
    ui->Send->setEnabled(false);
    ui->Stop->setEnabled(false);
}

void AAServerWindow::on_actionExit_triggered() {
    if (ui->Stop->isEnabled()) {
        _server->doServerInterrupt();
        _server->close();
    }
    close();
}

void AAServerWindow::on_Send_clicked(){
    _server->doSendServerMessageToUsers(ui->lineEdit->text());
    ui->chatArea->append("[PublicServerMessage]: " + ui->lineEdit->text());
    ui->lineEdit->setText("");
}
