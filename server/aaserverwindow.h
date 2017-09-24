#ifndef AASERVERWINDOW_H
#define AASERVERWINDOW_H

#include <QMainWindow>
#include "htwindow.h"
#include "aaserver.h"
#include "hashtable.h"

namespace Ui {
class AAServerWindow;
}

class AAServerWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::AAServerWindow *ui;
    HTWindow *HTWnd;
    AAServer *_server;
    HashTable *_htable;

public:
    explicit AAServerWindow(QWidget *parent = 0);

    void setText(QString str);

    ~AAServerWindow();

private slots:
    void on_actionHashTable_triggered();

    void on_Start_clicked();
    void on_Stop_clicked();

    void on_addUserToGui(QString username);
    void on_removeUserFromGui(QStringList *_users);
    void on_messageToGui(QString message);
    void on_actionExit_triggered();
    void on_Send_clicked();
};

#endif // AASERVERWINDOW_H
