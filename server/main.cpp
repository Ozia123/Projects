#include "aaserverwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    a.addLibraryPath(QCoreApplication::applicationDirPath() + "/plugins");

    AAServerWindow w;
    w.show();

    return a.exec();
}
