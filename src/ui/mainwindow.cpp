#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>      // Header for buttons
#include <QTableWidget>     // Header for tables
#include <QTableWidgetItem> // Header for adding things into cells
#include <QHeaderView>
#include <QSettings>
#include <QSet>
#include <QFont>
#include <format>
#include "../core/Process.h"
#include "../core/utils.h"

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

    // 1. Only use code layout IF you don't have one in the .ui designer.
    // If your screen is black, it's safer to use the UI Designer for layouts.
    // But if you want it in code, make sure the centralwidget is ready:
    if (!ui->centralwidget->layout()) {
        QVBoxLayout *layout = new QVBoxLayout(ui->centralwidget);
        layout->addWidget(ui->tableWidget);
        layout->addWidget(ui->darkModeButton);
        //layout->addWidget(ui->pushButton);
    }

    this->setWindowTitle("Real time Process Monitor");

    // 2. Table Setup (Cleaned up the duplicates)
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setRowCount(0);

    //Table column title
    ui->tableWidget->setHorizontalHeaderLabels({"PID", "Name", "CPU%", "Memory", "I\\O Read", "I\\O Write", "Owner"});

    // Header resizing - Use Interactive for columns, but let them stretch to fill
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(20);
    ui->tableWidget->verticalHeader()->setMinimumSectionSize(18);
    ui->tableWidget->horizontalHeader()->setFixedHeight(52);

    QFont tableFont = ui->tableWidget->font();
    tableFont.setPointSize(9);
    ui->tableWidget->setFont(tableFont);

    // Enable click-to-sort on columns
    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->horizontalHeader()->setSortIndicatorShown(true);
    ui->tableWidget->horizontalHeader()->setSectionsClickable(true);

    // 4. Theme Loading
    QSettings settings("MyCompany", "ProcessMonitor");
    bool isDark = settings.value("darkMode", false).toBool();
    ui->darkModeButton->setChecked(isDark);
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
        this->setStyleSheet(
            "QMainWindow { background-color: #1e1e1e; }"
            "QTableWidget { background-color: #252526; color: #d4d4d4; gridline-color: #333333; selection-background-color: #094771; selection-color: #ffffff; border: none; alternate-background-color: #2d2d2d; font-size: 9pt; }"
            "QHeaderView::section { background-color: #333333; color: #cccccc; padding: 4px; border: 1px solid #1e1e1e; font-weight: 600; font-size: 9pt; }"
            "QRadioButton, QLabel { color: #d4d4d4; font-size: 11pt; }"
            "QPushButton { background-color: #333333; color: white; border: 1px solid #454545; border-radius: 4px; padding: 8px 15px; }"
            "QPushButton:hover { background-color: #454545; }"
            "QPushButton:pressed { background-color: #094771; }"
            );
    } else {
        this->setStyleSheet(
            "QMainWindow { background-color: #f5f5f5; }"
            "QTableWidget { background-color: white; alternate-background-color: #f9f9f9; font-size: 9pt; }"
            "QHeaderView::section { background-color: #e0e0e0; border: 1px solid #cccccc; padding: 4px; font-size: 9pt; }"
            "QPushButton { padding: 8px; } "
            );
    }
}

void MainWindow::on_darkModeButton_toggled(bool checked) {
    applyTheme(checked);
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
    double totalCpuPercent = 0.0;
    double totalMemBytes = 0.0;
    double totalReadBytesPerSec = 0.0;
    double totalWriteBytesPerSec = 0.0;

    for (const Process& process : processes) {
        const QString pid = QString::fromStdString(process.getID()).trimmed();
        const QString name = QString::fromStdString(process.getName()).trimmed();
        const QString cpu = QString::fromStdString(process.resourceInfo.getCPURate());
        const QString memory = QString::fromStdString(process.resourceInfo.getMemSize());
        const QString readSpeed = QString::fromStdString(process.resourceInfo.getReadSpeed());
        const QString writeSpeed = QString::fromStdString(process.resourceInfo.getWriteSpeed());
        const QString owner = QString::fromStdString(process.getOwner());

        totalCpuPercent += process.resourceInfo.getCPURateValue();
        totalMemBytes += process.resourceInfo.getMemBytes();
        totalReadBytesPerSec += process.resourceInfo.getReadSpeedBytesPerSec();
        totalWriteBytesPerSec += process.resourceInfo.getWriteSpeedBytesPerSec();

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

        QTableWidgetItem* ownerItem = table->item(row, 6);
        if (!ownerItem) {
            ownerItem = new QTableWidgetItem();
            table->setItem(row, 6, ownerItem);
        }
        ownerItem->setText(owner);
    }

    for (int row = table->rowCount() - 1; row >= 0; --row) {
        QTableWidgetItem* pidItem = table->item(row, 0);
        if (!pidItem || !livePids.contains(pidItem->text())) {
            table->removeRow(row);
        }
    }

    const QString processCountText = QString::number(static_cast<qulonglong>(processes.size()));
    const QString totalCpuText = QString::fromStdString(std::format("{:.2f}%", totalCpuPercent));
    const QString totalMemText = QString::fromStdString(format_size_from_bytes(totalMemBytes));
    const QString totalReadText = QString::fromStdString(format_size_from_bytes(totalReadBytesPerSec));
    const QString totalWriteText = QString::fromStdString(format_size_from_bytes(totalWriteBytesPerSec));

    table->setHorizontalHeaderLabels({
        "PID",
        QString("Name\n%1 processes").arg(processCountText),
        QString("CPU%\nTotal %1").arg(totalCpuText),
        QString("Memory\nTotal %1").arg(totalMemText),
        QString("I/O Read\nTotal %1/s").arg(totalReadText),
        QString("I/O Write\nTotal %1/s").arg(totalWriteText),
        "Owner"
    });

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
