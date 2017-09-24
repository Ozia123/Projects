#include "reconnectionwindow.h"
#include "ui_reconnectionwindow.h"

reconnectionwindow::reconnectionwindow(QWidget *parent) :
    ConnectionWindow(parent),
    ui(new Ui::reconnectionwindow) {
    ui->setupUi(this);
    this->setMaximumHeight(250);
    this->setMinimumHeight(250);
    this->setMaximumWidth(351);
    this->setMinimumWidth(351);
    QRegExp expForIP("[0-9]{1,3}[.]{1}[0-9]{1,3}[.]{1}[0-9]{1,3}[.]{1}[0-9]{1,3}");
    QRegExp expForPort("[0-9]{5}");
    ui->lineEdit_IP->setValidator(new QRegExpValidator(expForIP, this));
    ui->lineEdit_Port->setValidator(new QRegExpValidator(expForPort, this));
    connect(ui->lineEdit_IP, SIGNAL(textChanged(QString)), this, SLOT(AbleToSend()));
    connect(ui->lineEdit_Port, SIGNAL(textChanged(QString)), this, SLOT(AbleToSend()));
}

reconnectionwindow::~reconnectionwindow() {
    delete ui;
}

void reconnectionwindow::FillUpFields(QString &_IP, QString &_Port, QString &_Error) {
    ui->lineEdit_IP->setText(_IP);
    ui->lineEdit_Port->setText(_Port);
    ui->textBrowser->setText(_Error);
}

void reconnectionwindow::AbleToSend() {
    ui->ConnectButton_2->setEnabled(ui->lineEdit_IP->hasAcceptableInput() && ui->lineEdit_Port->hasAcceptableInput());
}

void reconnectionwindow::on_ConnectButton_2_clicked() {
    emit ConnectToHost(ui->lineEdit_IP->text(), ui->lineEdit_Port->text());
}

void reconnectionwindow::on_CloseButton_2_clicked() {
    close();
}
