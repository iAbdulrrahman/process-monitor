#include "LoginWindow.h"
#include "ui_LoginWindow.h"
#include "mainwindow.h"
#include <QMessageBox>
#include "Forgotpassworddialog.h"
#include <QPixmap>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    static bool resourceInitialized = []() {
        Q_INIT_RESOURCE(resources);
        return true;
    }();
    Q_UNUSED(resourceInitialized);

    // Programiticly  HIDE password
    ui->lineEdit_password->setEchoMode(QLineEdit::Password); //*

    ui->label_logo->setMinimumSize(220, 220);
    ui->label_logo->setMaximumSize(260, 260);
    ui->label_logo->setAlignment(Qt::AlignCenter);
    ui->verticalLayout_branding->setAlignment(ui->label_logo, Qt::AlignHCenter);

    // Use the app logo from resources
    const QPixmap logo(":/ui/heartbeat.png");
    if (!logo.isNull()) {
        ui->label_logo->setPixmap(logo.scaled(230, 230, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        ui->label_logo->setText("Logo");
    }
    ui->label_logo->setScaledContents(false);
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



void LoginDialog::on_pushButton_clicked()
{
    //click close current dialog
    //goto ForgotPasswordDialog
    // 1. Hide/Close the login window
    this->hide();

    // 2. Create the Forgot Password Dialog
    // Use 'nullptr' or no parent so it doesn't get hidden if 'this' is destroyed
    ForgotPasswordDialog *forgotDlg = new ForgotPasswordDialog();

    // 3. Ensure it cleans up memory when closed
    forgotDlg->setAttribute(Qt::WA_DeleteOnClose);

    // 4. Show the new window
    forgotDlg->show();


}

