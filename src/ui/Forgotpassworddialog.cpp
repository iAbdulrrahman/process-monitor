#include "Forgotpassworddialog.h"
#include "ui_Forgotpassworddialog.h"
#include <QAbstractButton>
#include <QDialog>
#include <QMessageBox>
#include "mainwindow.h"

ForgotPasswordDialog::ForgotPasswordDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ForgotPasswordDialog)
{
    ui->setupUi(this);
}

ForgotPasswordDialog::~ForgotPasswordDialog()
{
    delete ui;
}

void ForgotPasswordDialog::on_buttonBox_2_clicked(QAbstractButton *button)
{
        // Identify which button was clicked (if using a Standard Button Box)
        // if (ui->buttonBox_2->standardButton(button) == QDialogButtonBox::Ok) {

        //     // Logic for your variable
        //     QString resetPassword = "Success"; // Or however you're storing it

        //     // Tell the app it's time to move to the main page
        //     emit passwordResetSuccessful();

        //     // 4. Close this dialog
        //     this->accept();


                // Assuming buttonBox_2 has an "Ok" or "Save" button
                if (ui->buttonBox_2->standardButton(button) == QDialogButtonBox::Ok) {

                    QString resetPassword = "successfully changed password";

                    // Show the message to the user
                    QMessageBox::information(this, "Success", resetPassword);

                    // Close this and open MainWindow
                    this->hide();
                    MainWindow *mainWin = new MainWindow();
                    mainWin->setAttribute(Qt::WA_DeleteOnClose);
                    mainWin->show();
                }
}

