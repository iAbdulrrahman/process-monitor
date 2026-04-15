#include <iostream>
#include "ui/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); //anything between these two lines is so run the application
    MainWindow w;
    w.show();
    return a.exec(); //anything between these two lines is so run the application
}

