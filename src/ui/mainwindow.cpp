#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>      // Header for buttons
#include <QTableWidget>     // Header for tables
#include <QTableWidgetItem> // Header for adding things into cells
#include <QHeaderView>
#include <QSettings>
#include <QSet>
#include "../core/Process.h"

namespace {
class NumericTableWidgetItem : public QTableWidgetItem {
public:
    using QTableWidgetItem::QTableWidgetItem;

    bool operator<(const QTableWidgetItem& other) const override {
        bool okLeft = false;
        bool okRight = false;
        const double left = data(Qt::UserRole).toDouble(&okLeft);
        const double right = other.data(Qt::UserRole).toDouble(&okRight);

        if (okLeft && okRight) {
            return left < right;
        }

        return QTableWidgetItem::operator<(other);
    }
};
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , refreshTimer(new QTimer(this))
{  // constructor of MainWindow
    ui->setupUi(this);
    // created table in ui mode
    //added gridLayout to format all the smaller widgets into

    // change window title name
    this->setWindowTitle("Real time Process Monitor "); //title name

    // Setup the table grid
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setRowCount(0);

    //Table column title
    ui->tableWidget->setHorizontalHeaderLabels({"PID", "Name", "CPU%", "Memory", "I\\O Read", "I\\O Write", "Status"});

    // Select the rows
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // select one row at a time
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // Make the table responsive
    // Makes columns fill the available horizontal space with the keyword Streach
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Enable click-to-sort on columns
    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->horizontalHeader()->setSortIndicatorShown(true);
    ui->tableWidget->horizontalHeader()->setSectionsClickable(true);

    // alternating row colors on
    ui->tableWidget->setAlternatingRowColors(true);

    // 1. Load the saved setting
    QSettings settings("MyCompany", "ProcessMonitor");
    bool isDark = settings.value("darkMode", false).toBool(); // default to false

    // 2. Set the button state (this will trigger the toggle function automatically)
    ui->darkModeButton->setChecked(isDark);

    // 3. Apply the theme immediately
    applyTheme(isDark);

    // Initial render and periodic updates every second (on UI thread)
    updateList(this->processMonitor.getProcesses());
    connect(refreshTimer, &QTimer::timeout, this, [this]() {
        this->processMonitor.refresh();
        this->updateList(this->processMonitor.getProcesses());
    });
    refreshTimer->start(1000);
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

int MainWindow::findRowByPid(const QString& pid) const {
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QTableWidgetItem* item = ui->tableWidget->item(row, 0);
        if (item && item->text() == pid) {
            return row;
        }
    }

    return -1;
}

void MainWindow::updateList(const std::vector<Process>& processes) {
    QTableWidget* table = ui->tableWidget;
    QHeaderView* header = table->horizontalHeader();

    const int sortColumn = header->sortIndicatorSection();
    const Qt::SortOrder sortOrder = header->sortIndicatorOrder();

    table->setSortingEnabled(false);

    QSet<QString> livePids;

    for (const Process& process : processes) {
        const QString pid = QString::fromStdString(process.getID()).trimmed();
        const QString name = QString::fromStdString(process.getName()).trimmed();
        const QString cpu = QString::fromStdString(process.resourceInfo.getCPURate());
        const QString memory = QString::fromStdString(process.resourceInfo.getMemSize());
        const QString readSpeed = QString::fromStdString(process.resourceInfo.getReadSpeed());
        const QString writeSpeed = QString::fromStdString(process.resourceInfo.getWriteSpeed());

        livePids.insert(pid);

        int row = findRowByPid(pid);
        if (row == -1) {
            row = table->rowCount();
            table->insertRow(row);
        }

        QTableWidgetItem* pidItem = table->item(row, 0);
        if (!pidItem) {
            pidItem = new QTableWidgetItem();
            table->setItem(row, 0, pidItem);
        }
        pidItem->setText(pid);

        QTableWidgetItem* nameItem = table->item(row, 1);
        if (!nameItem) {
            nameItem = new QTableWidgetItem();
            table->setItem(row, 1, nameItem);
        }
        nameItem->setText(name);

        QTableWidgetItem* cpuItem = table->item(row, 2);
        if (!cpuItem) {
            cpuItem = new NumericTableWidgetItem();
            table->setItem(row, 2, cpuItem);
        }
        cpuItem->setText(cpu);
        cpuItem->setData(Qt::UserRole, process.resourceInfo.getCPURateValue());

        QTableWidgetItem* memItem = table->item(row, 3);
        if (!memItem) {
            memItem = new NumericTableWidgetItem();
            table->setItem(row, 3, memItem);
        }
        memItem->setText(memory);
        memItem->setData(Qt::UserRole, process.resourceInfo.getMemBytes());

        QTableWidgetItem* readItem = table->item(row, 4);
        if (!readItem) {
            readItem = new QTableWidgetItem();
            table->setItem(row, 4, readItem);
        }
        readItem->setText(readSpeed);

        QTableWidgetItem* writeItem = table->item(row, 5);
        if (!writeItem) {
            writeItem = new QTableWidgetItem();
            table->setItem(row, 5, writeItem);
        }
        writeItem->setText(writeSpeed);

        QTableWidgetItem* statusItem = table->item(row, 6);
        if (!statusItem) {
            statusItem = new QTableWidgetItem();
            table->setItem(row, 6, statusItem);
        }
        statusItem->setText("active");
    }

    for (int row = table->rowCount() - 1; row >= 0; --row) {
        QTableWidgetItem* pidItem = table->item(row, 0);
        if (!pidItem || !livePids.contains(pidItem->text())) {
            table->removeRow(row);
        }
    }

    table->setSortingEnabled(true);
    if (sortColumn >= 0) {
        table->sortByColumn(sortColumn, sortOrder);
    }
}

//when button is clicked then close application
void MainWindow::on_pushButton_clicked()
{
    this->close();
}

