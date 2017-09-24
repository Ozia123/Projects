#include "clientinterface.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.addLibraryPath((QCoreApplication::applicationDirPath() + "/platforms/"));

    ClientInterface CInterface;

    return a.exec();
}
