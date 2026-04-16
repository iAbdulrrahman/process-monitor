#include "LoginWindow.h"
#include "ui_LoginWindow.h"
#include "mainwindow.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    if(username == "admin" && password == "admin"){
        QMessageBox::information(this, "login", "login successfull for admin");
        hide();
        // This requires 'mainwindow' to be defined in LoginWindow.h
        mainwindow = new MainWindow(this);
        mainwindow->show();
    } else {
        QMessageBox::warning(this, "login", "Unsuccessful login");
    }
}
