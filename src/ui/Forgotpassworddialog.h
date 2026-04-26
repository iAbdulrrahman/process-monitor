#ifndef FORGOTPASSWORDDIALOG_H
#define FORGOTPASSWORDDIALOG_H
#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class ForgotPasswordDialog;
}

class ForgotPasswordDialog : public QDialog{
    Q_OBJECT

public:
    explicit ForgotPasswordDialog(QWidget *parent = nullptr);
    ~ForgotPasswordDialog();

private slots:
    void on_buttonBox_2_clicked(QAbstractButton *button);
signals:
    void passwordResetSuccessful();

private:
    Ui::ForgotPasswordDialog *ui;
};

#endif // FORGOTPASSWORDDIALOG_H
