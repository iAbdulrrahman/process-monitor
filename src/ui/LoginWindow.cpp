#include "LoginWindow.h"
#include "ui_LoginWindow.h"
#include "mainwindow.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    // Programiticly  HIDE password
    ui->lineEdit_password->setEchoMode(QLineEdit::Password); //*
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

// void LoginDialog::on_pushButton_login_clicked()
// {
//     QString username = ui->lineEdit_username->text();
//     QString password = ui->lineEdit_password->text();

//     if(username == "admin" && password == "admin"){
//         QMessageBox::information(this, "login", "login successfull for admin");
//         hide();
//         // This requires 'mainwindow' to be defined in LoginWindow.h
//         mainwindow = new MainWindow(this);
//         mainwindow->show();
//     } else {
//         QMessageBox::warning(this, "login", "Unsuccessful login");
//     }
// }

void LoginDialog::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    if(username == "admin" && password == "admin"){
        // 1. Hide the login window IMMEDIATELY
        this->hide();

        // 2. Now show the success message
        QMessageBox::information(this, "login", "Login successful for admin");

        // 3. Create and show MainWindow (with memory leak protection)
        mainwindow = new MainWindow(); // Removed 'this' to allow independent closing
        mainwindow->setAttribute(Qt::WA_DeleteOnClose);
        mainwindow->show();
    } else {
        QMessageBox::warning(this, "login", "Unsuccessful login");
    }
}


