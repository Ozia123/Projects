#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "Hash.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::LoginWindow *ui;

public:
    static const qint8 comUserSigningError      = 10;
    static const qint8 comUserAutorizationError = 11;

    explicit LoginWindow(QWidget *parent = 0);
    void ErrWnd(qint8 command);
    void ShowLoginForm();
    ~LoginWindow();

private slots:
    void AbleToLogin();
    void AbleToSignUp();

    void on_pushButton_SignUpShowForm_clicked();
    void on_pushButton_SignUp_clicked();
    void on_pushButton_Close_2_clicked();
    void on_pushButton_Login_clicked();
    void on_pushButton_Close_1_clicked();

signals:
    void TryToLogin(QString& Username, QString& Hash);
    void TryToSignUp(QString& Username, QString& Hash);

};

#endif // LOGINWINDOW_H
