#ifndef RECONNECTIONWINDOW_H
#define RECONNECTIONWINDOW_H

#include <QMainWindow>
#include "connectionwindow.h"

namespace Ui {
class reconnectionwindow;
}

class reconnectionwindow : public ConnectionWindow {
    Q_OBJECT

private:
    Ui::reconnectionwindow *ui;

public:
    explicit reconnectionwindow(QWidget *parent = 0);
    void FillUpFields(QString& _IP, QString& _Port, QString& _Error);
    ~reconnectionwindow();

private slots:
    void AbleToSend();
    void on_ConnectButton_2_clicked();
    void on_CloseButton_2_clicked();
};

#endif // RECONNECTIONWINDOW_H
