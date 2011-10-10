#include <QtGui/QApplication>
#include "qtKeyRec.h"

#ifdef QTOPIA
#include <QtopiaServiceRequest>
#include <QValueSpaceItem>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtKeyRec w;
    w.show();

    return a.exec();
}
