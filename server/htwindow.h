#ifndef HTWINDOW_H
#define HTWINDOW_H

#include <QMainWindow>
#include "hashtable.h"

namespace Ui {
class HTWindow;
}

class HTWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::HTWindow *ui;
    QFile *mFile;
    HashTable *_htable;

public:
    explicit HTWindow(QWidget *parent, HashTable *htable);
    void UpdateHTable();
    ~HTWindow();

private slots:
    void on_Close_clicked();
    void on_Save_clicked();
    void on_Load_clicked();
};

#endif // HTWINDOW_H
