#include "CityMap.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CityMap w;
    w.show();
    return a.exec();
}
