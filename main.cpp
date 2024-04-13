#include "form.h"

#include <QApplication>
#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Form w;
    w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}
