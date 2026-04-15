#include <iostream>
#include "core/ProcessMonitor.h"
#include "ui/MainWindow.h"

#include "LoginWindow.h"  //for login window   first

#include <QApplication>
#include <QTableWidget>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv); //anything between these two lines is so run the application
    //MainWindow w; // this will open summaryWindow first
    //w.show();
    LoginDialog loginWindow;
    loginWindow.show();
    return a.exec(); //anything between these two lines is so run the application
}

