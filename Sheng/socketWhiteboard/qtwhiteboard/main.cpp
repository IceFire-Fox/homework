#include "drawwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    drawWindow w;
    w.show();
    //w.space->image();
    return a.exec();
}
