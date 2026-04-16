#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QSettings>
#include <QVBoxLayout> // Add this becuase of the floating layout problem

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. Only use code layout IF you don't have one in the .ui designer.
    // If your screen is black, it's safer to use the UI Designer for layouts.
    // But if you want it in code, make sure the centralwidget is ready:
    if (!ui->centralwidget->layout()) {
        QVBoxLayout *layout = new QVBoxLayout(ui->centralwidget);
        layout->addWidget(ui->tableWidget);
        layout->addWidget(ui->darkModeButton);
        layout->addWidget(ui->pushButton);
    }

    this->setWindowTitle("Real time Process Monitor");

    // 2. Table Setup (Cleaned up the duplicates)
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setHorizontalHeaderLabels({"PID", "Name", "CPU%", "Memory", "I\\O Read", "I\\O Write", "Status"});

    // Header resizing - Use Interactive for columns, but let them stretch to fill
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);

    // 3. Data (Keep your row data here)
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("15%"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("2.4GB"));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem("Healthy"));
    // ... add your other rows ...

    // 4. Theme Loading
    QSettings settings("MyCompany", "ProcessMonitor");
    bool isDark = settings.value("darkMode", false).toBool();
    ui->darkModeButton->setChecked(isDark);
    applyTheme(isDark);
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::applyTheme(bool dark) {
    if (dark) {
        this->setStyleSheet(
            "QMainWindow { background-color: #1e1e1e; }"
            "QTableWidget { background-color: #252526; color: #d4d4d4; gridline-color: #333333; selection-background-color: #094771; selection-color: #ffffff; border: none; alternate-background-color: #2d2d2d; }"
            "QHeaderView::section { background-color: #333333; color: #cccccc; padding: 6px; border: 1px solid #1e1e1e; font-weight: bold; }"
            "QRadioButton, QLabel { color: #d4d4d4; font-size: 11pt; }"
            "QPushButton { background-color: #333333; color: white; border: 1px solid #454545; border-radius: 4px; padding: 8px 15px; }"
            "QPushButton:hover { background-color: #454545; }"
            "QPushButton:pressed { background-color: #094771; }"
            );
    } else {
        this->setStyleSheet(
            "QMainWindow { background-color: #f5f5f5; }"
            "QTableWidget { background-color: white; alternate-background-color: #f9f9f9; }"
            "QHeaderView::section { background-color: #e0e0e0; border: 1px solid #cccccc; }"
            "QPushButton { padding: 8px; } "
            );
    }
}

void MainWindow::on_darkModeButton_toggled(bool checked) {
    applyTheme(checked);
    QSettings settings("MyCompany", "ProcessMonitor");
    settings.setValue("darkMode", checked);
}

void MainWindow::on_pushButton_clicked()
{
    this->close();
}
