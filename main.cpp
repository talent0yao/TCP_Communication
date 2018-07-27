#include "tcpwidget.h"
#include "clientwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TCPWidget w;
    w.show();

    ClientWidget c;
    c.show();

    return a.exec();
}
