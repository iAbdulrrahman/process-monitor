#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include "mainwindow.h"
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_pushButton_login_clicked();

    void on_pushButton_clicked();

private:
    Ui::LoginDialog *ui;// for the login window to run another window after button click
    //LoginDialog* loginDialog;
    MainWindow* mainwindow;
};

#endif // LOGINWINDOW_H
