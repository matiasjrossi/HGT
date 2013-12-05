#include <QtGui/QApplication>
#include "mainwindow.h"
#include "logger.h"

int main(int argc, char *argv[])
{
    Logger::init(QString("hgt.log"),Logger::FATAL);
    Logger::get()->output("Starting HGT is a Graphical Tool");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
