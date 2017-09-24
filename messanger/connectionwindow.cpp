#include "connectionwindow.h"
#include "ui_connectionwindow.h"

ConnectionWindow::ConnectionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConnectionWindow) {
    ui->setupUi(this);

    ui->ConnectButton->setEnabled(false);
    QRegExp expForIP("[0-9]{1,3}[.]{1}[0-9]{1,3}[.]{1}[0-9]{1,3}[.]{1}[0-9]{1,3}");
    QRegExp expForPort("[0-9]{5}");
    ui->lineEdit_IP->setValidator(new QRegExpValidator(expForIP, this));
    ui->lineEdit_Port->setValidator(new QRegExpValidator(expForPort, this));
    connect(ui->lineEdit_IP, SIGNAL(textChanged(QString)), this, SLOT(AbleToSend()));
    connect(ui->lineEdit_Port, SIGNAL(textChanged(QString)), this, SLOT(AbleToSend()));
}

ConnectionWindow::~ConnectionWindow() {
    delete ui;
}

// AbleToSend private slot enabled to push BUTTON "Send" while both forms are correctly filed
void ConnectionWindow::AbleToSend() {
    ui->ConnectButton->setEnabled(ui->lineEdit_IP->hasAcceptableInput() && ui->lineEdit_Port->hasAcceptableInput());
}

void ConnectionWindow::on_ConnectButton_clicked() {
    emit ConnectToHost(ui->lineEdit_IP->text(), ui->lineEdit_Port->text());
}

void ConnectionWindow::on_CloseButton_clicked() {
    close();
}
