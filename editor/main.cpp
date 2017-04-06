#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    QTranslator qtTranslator;
    qDebug() << qtTranslator.load("qtbase_ru", ":/translations");
    a.installTranslator(&qtTranslator);

    return a.exec();
}
