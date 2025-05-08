#include "BwhDesktop.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BwhDesktop w;
    //w.loadData(0);
    w.show();
    return a.exec();
}
