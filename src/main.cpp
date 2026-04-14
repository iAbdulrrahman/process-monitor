#include <iostream>
#include "core/ProcessMonitor.h"
#include "mainwindow.h"

#include <QApplication>
#include <QTableWidget>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv); //anything between these two lines is so run the application
    MainWindow w;
    w.show();
    return a.exec(); //anything between these two lines is so run the application
}

