#ifndef WELCOMEPAGEDIALOG_H
#define WELCOMEPAGEDIALOG_H

#include <QDialog>

namespace Ui {
class WelcomePageDialog;
}

class WelcomePageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomePageDialog(QWidget *parent = nullptr);
    ~WelcomePageDialog();

private:
    Ui::WelcomePageDialog *ui;
};

#endif // WELCOMEPAGEDIALOG_H
