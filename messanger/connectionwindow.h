#ifndef CONNECTIONWINDOW_H
#define CONNECTIONWINDOW_H

#include <QMainWindow>

namespace Ui {
class ConnectionWindow;
}

class ConnectionWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::ConnectionWindow *ui;

public:
    explicit ConnectionWindow(QWidget *parent = 0);
    ~ConnectionWindow();

private slots:
    void AbleToSend();
    void on_ConnectButton_clicked();
    void on_CloseButton_clicked();

signals:
    void ConnectToHost(QString& IP, QString& Port);

};

#endif // CONNECTIONWINDOW_H
