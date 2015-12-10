#include <QCoreApplication>
#include <assert.h>
#include "scada.hpp"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    try {
        scada::SCADA *scada = new scada::SCADA(argc, argv, a);
        assert(scada);
        scada->init();
    } catch (QString *message) {
        qDebug() << *message;
        exit(0);
    }

    return a.exec();
}
