#include "welcomepagedialog.h"
#include "ui_welcomepagedialog.h"

WelcomePageDialog::WelcomePageDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WelcomePageDialog)
{
    ui->setupUi(this);
    //ui->label->setPixmap(QPixmap(":/logo.png"));
}

WelcomePageDialog::~WelcomePageDialog()
{
    delete ui;
}
