#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow) {
    ui->setupUi(this);

    ui->ErrMsg->hide();

    ui->groupBox_SignUp->hide();
    ui->pushButton_Login->setEnabled(false);

    QRegExp expForUsername("[0-9a-zA-Z]{3,20}");
    ui->lineEdit_Login_Username->setValidator(new QRegExpValidator(expForUsername, this));
    ui->lineEdit_SignUp_Username->setValidator(new QRegExpValidator(expForUsername, this));
    QRegExp expForPassword("[0-9a-zA-Z]{8,20}");
    ui->lineEdit_Login_Password->setValidator(new QRegExpValidator(expForPassword, this));
    ui->lineEdit_SignUpPassword->setValidator(new QRegExpValidator(expForPassword, this));
    ui->lineEdit_SignUpPassword_2->setValidator(new QRegExpValidator(expForPassword, this));

    ui->lineEdit_Login_Password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_SignUpPassword->setEchoMode(QLineEdit::Password);
    ui->lineEdit_SignUpPassword_2->setEchoMode(QLineEdit::Password);

    connect(ui->lineEdit_Login_Username, SIGNAL(textChanged(QString)), this, SLOT(AbleToLogin()));
    connect(ui->lineEdit_Login_Password, SIGNAL(textChanged(QString)), this, SLOT(AbleToLogin()));
}

LoginWindow::~LoginWindow() {
    delete ui;
}

void LoginWindow::ShowLoginForm() {
    ui->groupBox_SignUp->hide();
    ui->lineEdit_Login_Username->setText(ui->lineEdit_SignUp_Username->text());
    ui->lineEdit_Login_Password->setText(ui->lineEdit_SignUpPassword->text());
    ui->lineEdit_SignUp_Username->setText("");
    ui->lineEdit_SignUpPassword->setText("");
    ui->lineEdit_SignUpPassword_2->setText("");
    ui->groupBox->show();
    ui->question->show();
    ui->pushButton_SignUpShowForm->show();
}

void LoginWindow::ErrWnd(qint8 command) {

    switch (command) { // always plus 33
    case LoginWindow::comUserAutorizationError: {
        this->setMaximumHeight(273);
        this->setMinimumHeight(273);
        ui->ErrMsg->show();
        // default y is 65
        ui->ErrMsg->setGeometry(12,98,331,51);
        // default y is 99
        ui->groupBox->setGeometry(12,145,331,121);
        ui->ErrLog->setText("The username or password you entered was incorrect.");
    }
    break;

    case LoginWindow::comUserSigningError: {
        this->setMaximumHeight(273);
        this->setMinimumHeight(273);
        ui->ErrMsg->show();
        // y is 65 (default)
        ui->groupBox_SignUp->setGeometry(12,111,331,161);
        ui->ErrLog->setText("That name has already been claimed.");
    }
    break;

    }
}

void LoginWindow::AbleToLogin() {
    ui->pushButton_Login->setEnabled(ui->lineEdit_Login_Username->hasAcceptableInput() &&
                                     ui->lineEdit_Login_Password->hasAcceptableInput());
}

void LoginWindow::AbleToSignUp() {
    ui->pushButton_SignUp->setEnabled(ui->lineEdit_SignUp_Username->hasAcceptableInput() &&
                                      ui->lineEdit_SignUpPassword->hasAcceptableInput() &&
                                      ui->lineEdit_SignUpPassword_2->hasAcceptableInput() &&
                                      ui->lineEdit_SignUpPassword->text() == ui->lineEdit_SignUpPassword_2->text());
}

void LoginWindow::on_pushButton_SignUpShowForm_clicked() {
    this->setMaximumHeight(227);
    this->setMinimumHeight(227);
    this->setWindowTitle("Sign Up!");
    this->update();
    ui->ErrMsg->hide();
    ui->ErrMsg->setGeometry(12,65,331,51);
    ui->groupBox_SignUp->setGeometry(12,65,331,161);
    ui->groupBox->hide();
    ui->question->hide();
    ui->pushButton_SignUpShowForm->hide();
    ui->groupBox_SignUp->show();
    ui->pushButton_SignUp->setEnabled(false);

    connect(ui->lineEdit_SignUp_Username, SIGNAL(textChanged(QString)), this, SLOT(AbleToSignUp()));
    connect(ui->lineEdit_SignUpPassword, SIGNAL(textChanged(QString)), this, SLOT(AbleToSignUp()));
    connect(ui->lineEdit_SignUpPassword_2, SIGNAL(textChanged(QString)), this, SLOT(AbleToSignUp()));
}

void LoginWindow::on_pushButton_SignUp_clicked() {
    Hash hash(ui->lineEdit_SignUpPassword->text());

    this->setMaximumHeight(227);
    this->setMinimumHeight(227);
    this->setWindowTitle("Login...");
    this->update();
    ui->ErrMsg->hide();
    ui->groupBox->setGeometry(12,99,331,121);

    // Signing Up

    emit TryToSignUp(ui->lineEdit_SignUp_Username->text(), QString::number(hash.Get()));

    // ----------

    // in ShowLogin
}

void LoginWindow::on_pushButton_Close_2_clicked() {
    this->setMaximumHeight(227);
    this->setMinimumHeight(227);
    this->setWindowTitle("Login...");
    this->update();
    ui->ErrMsg->hide();
    ui->groupBox->setGeometry(12,99,331,121);

    ui->groupBox_SignUp->hide();
    ui->groupBox->show();
    ui->question->show();
    ui->pushButton_SignUpShowForm->show();
}

void LoginWindow::on_pushButton_Login_clicked() {
    Hash hash(ui->lineEdit_Login_Password->text());
    // If login

    emit TryToLogin(ui->lineEdit_Login_Username->text(), QString::number(hash.Get()));
}

void LoginWindow::on_pushButton_Close_1_clicked() {
    close();
}
