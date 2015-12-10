#include <QCoreApplication>
#include "scada.hpp"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    try {
        scada::SCADA *scada = new scada::SCADA(argc, argv, a);
        scada->init();
    } catch (QString *message) {
        qDebug() << *message;
        exit(0);
    }
    /*
    QXlsx::Document doc(QDir(QDir::currentPath()).filePath("test.xlsx"));

    doc.addSheet("Каталог событий");

    qDebug() << doc.read(1, 1).toString();
    qDebug() << doc.read(2, 1).toString();
    qDebug() << doc.read(3, 1).toString();

    doc.save();

    exit(0);
    */
    return a.exec();
}
