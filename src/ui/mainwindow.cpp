

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>      // Header for buttons
#include <QTableWidget>     // Header for tables
#include <QTableWidgetItem> // Header for adding things into cells
#include <QSettings>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{  // constructor of MainWindow
    ui->setupUi(this);
    // created table in ui mode
    //added gridLayout to format all the smaller widgets into

    // change window title name
    this->setWindowTitle("Real time Process Monitor "); //title name

    // Setup the table grid
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setRowCount(5);

    //Table column title
    ui->tableWidget->setHorizontalHeaderLabels({"PID", "Name", "CPU%", "Memory", "I\\O Read", "I\\O Write", "Status"});

    // Select the rows
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // select one row at a time
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // Make the table responsive
    // Makes columns fill the available horizontal space with the keyword Streach
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Add row title into rows test in
    // Row 0
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("15%"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("2.4GB"));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem("Healthy"));
    ui->tableWidget->setItem(0, 3, new QTableWidgetItem("51%"));
    ui->tableWidget->setItem(0, 4, new QTableWidgetItem("READ"));
    ui->tableWidget->setItem(0, 5, new QTableWidgetItem("WRITE"));
    ui->tableWidget->setItem(0, 6, new QTableWidgetItem("Active"));

    // alternating row colors on
    ui->tableWidget->setAlternatingRowColors(true);

    // 1. Load the saved setting
    QSettings settings("MyCompany", "ProcessMonitor");
    bool isDark = settings.value("darkMode", false).toBool(); // default to false

    // 2. Set the button state (this will trigger the toggle function automatically)
    ui->darkModeButton->setChecked(isDark);

    // 3. Apply the theme immediately
    applyTheme(isDark);
}



void MainWindow::applyTheme(bool dark) {
    if (dark) {
        // Modern Charcoal & Slate Theme
        this->setStyleSheet(
            // Main Window background
            "QMainWindow { background-color: #1e1e1e; }"

            // Table styling
            "QTableWidget { "
            "  background-color: #252526; "
            "  color: #d4d4d4; "
            "  gridline-color: #333333; "
            "  selection-background-color: #094771; " // Nice blue highlight
            "  selection-color: #ffffff; "
            "  border: none; "
            "  alternate-background-color: #2d2d2d; " // Subtle row difference
            "}"

            // Column Headers
            "QHeaderView::section { "
            "  background-color: #333333; "
            "  color: #cccccc; "
            "  padding: 6px; "
            "  border: 1px solid #1e1e1e; "
            "  font-weight: bold; "
            "}"

            // Radio Buttons and Labels
            "QRadioButton, QLabel { "
            "  color: #d4d4d4; "
            "  font-size: 11pt; "
            "}"

            // The "Close" button
            "QPushButton { "
            "  background-color: #333333; "
            "  color: white; "
            "  border: 1px solid #454545; "
            "  border-radius: 4px; "
            "  padding: 8px 15px; "
            "}"
            "QPushButton:hover { background-color: #454545; }"
            "QPushButton:pressed { background-color: #094771; }"
            );
    } else {
        // Professional Clean Light Theme - It remembers preference when turning quitting app
        this->setStyleSheet(
            "QMainWindow { background-color: #f5f5f5; }"
            "QTableWidget { background-color: white; alternate-background-color: #f9f9f9; }"
            "QHeaderView::section { background-color: #e0e0e0; border: 1px solid #cccccc; }"
            "QPushButton { padding: 8px; } "
            );
    }
}


#include <QSettings>

    void MainWindow::on_darkModeButton_toggled(bool checked) {
    applyTheme(checked);

    // Save the setting
    QSettings settings("MyCompany", "ProcessMonitor");
    settings.setValue("darkMode", checked);
}


MainWindow::~MainWindow()
{
    delete ui;
}


//when button is clicked then close application
void MainWindow::on_pushButton_clicked()
{
    this->close();
}

