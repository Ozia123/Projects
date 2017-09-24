#include "htwindow.h"
#include "ui_htwindow.h"

HTWindow::HTWindow(QWidget *parent, HashTable *htable) :
    QMainWindow(parent),
    ui(new Ui::HTWindow) {
    ui->setupUi(this);
    _htable = htable;

    UpdateHTable();

    mFile = new QFile(QString::QString("htable.rc"));

    if (!mFile->open(QFile::ReadOnly | QFile::Text)) {
        return;
    }

    QTextStream stream(mFile);
    QString buffer = stream.readAll();
    ui->textBrowser->setText(buffer);
}

void HTWindow::UpdateHTable() {
    ui->tableWidget->setRowCount(_htable->GetNumberOfUsers());
    ui->tableWidget->setColumnCount(2);

    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Hash" << "Username");

    for (int i(0); i < ui->tableWidget->rowCount(); i++) {
        QTableWidgetItem *hash = new QTableWidgetItem(tr("%1").arg(_htable->GetHashByID(i)));
        QTableWidgetItem *username = new QTableWidgetItem(tr("%1").arg(_htable->GetUserNameByID(i)));
        ui->tableWidget->setItem(i, 0, hash);
        ui->tableWidget->setItem(i, 1, username);
    }
}

HTWindow::~HTWindow() {
    delete ui;
}

void HTWindow::on_Close_clicked() {
    mFile->flush();
    mFile->close();
    close();
}

void HTWindow::on_Save_clicked() {
    _htable->Save(QString::QString("htable.rc"));
    QTextStream stream(mFile);
    QString buffer = stream.readAll();
    ui->textBrowser->setText(buffer);
}

void HTWindow::on_Load_clicked() {
    _htable->Load(QString::QString("htable.rc"));
    UpdateHTable();
}
