#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>      // Header for buttons
#include <QTableWidget>     // Header for tables
#include <QTableWidgetItem> // Header for adding things into cells
#include <QHeaderView>
#include <QSettings>
#include <QSet>
#include <QFont>
#include <QStringList>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <format>
#include "../core/Process.h"
#include "../core/SystemInfo.h"
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
    , metricsNetworkManager(new QNetworkAccessManager(this))
    , refreshTimer(new QTimer(this))
{  // constructor of MainWindow
    ui->setupUi(this);

    // 1. Only use code layout IF you don't have one in the .ui designer.
    // If your screen is black, it's safer to use the UI Designer for layouts.
    // But if you want it in code, make sure the centralwidget is ready:
    if (!ui->centralwidget->layout()) {
        QVBoxLayout *layout = new QVBoxLayout(ui->centralwidget);
        layout->addWidget(ui->tableWidget);
        layout->addWidget(ui->preferencesButton);
    }

    this->setWindowTitle("Real time Process Monitor");

    // 2. Table Setup (Cleaned up the duplicates)
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setRowCount(0);

    //Table column title
    ui->tableWidget->setHorizontalHeaderLabels({"PID", "Name", "CPU%", "Memory", "I\\O Read", "I\\O Write", "Owner"});

    // Header resizing - pre-interactive behavior.
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(20);
    ui->tableWidget->verticalHeader()->setMinimumSectionSize(18);
    ui->tableWidget->horizontalHeader()->setFixedHeight(52);
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    QFont tableFont = ui->tableWidget->font();
    tableFont.setPointSize(9);
    ui->tableWidget->setFont(tableFont);

    QFont headerFont = ui->tableWidget->horizontalHeader()->font();
    headerFont.setPointSize(9);
    ui->tableWidget->horizontalHeader()->setFont(headerFont);

    // Enable click-to-sort on columns
    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->horizontalHeader()->setSortIndicatorShown(true);
    ui->tableWidget->horizontalHeader()->setSectionsClickable(true);
    ui->tableWidget->horizontalHeader()->setSortIndicator(2, Qt::DescendingOrder);

    setupPreferencesMenu();

    // Initial render and periodic updates every second (on UI thread)
    updateList(this->processMonitor.getProcesses());
    connect(refreshTimer, &QTimer::timeout, this, [this]() {
        this->processMonitor.refresh();
        this->updateList(this->processMonitor.getProcesses());
    });

    QSettings settings("MyCompany", "ProcessMonitor");
    const int refreshRateMs = settings.value("refreshRateMs", 1000).toInt();
    refreshTimer->start(refreshRateMs > 0 ? refreshRateMs : 1000);
}

void MainWindow::publishSystemMetrics(double totalCpuPercent, double totalMemBytes, double totalReadBytesPerSec, double totalWriteBytesPerSec) {
    /*
    const QString memText = QString::fromStdString(format_size_from_bytes(totalMemBytes));
    const QString readText = QString::fromStdString(format_size_from_bytes(totalReadBytesPerSec));
    const QString writeText = QString::fromStdString(format_size_from_bytes(totalWriteBytesPerSec));
    */
    QJsonObject payload;
    payload["cpu"] = totalCpuPercent;
    payload["memory"] = totalMemBytes;
    payload["ioread"] = totalReadBytesPerSec;
    payload["iowrite"] = totalWriteBytesPerSec;

    QNetworkRequest request(QUrl("http://localhost:1880/system-metrics"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = metricsNetworkManager->post(request, QJsonDocument(payload).toJson(QJsonDocument::Compact));
    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
}


void MainWindow::setupPreferencesMenu() {
    QSettings settings("MyCompany", "ProcessMonitor");

    QMenu* preferencesMenu = new QMenu(this);

    QAction* darkModeAction = preferencesMenu->addAction("Night Mode");
    darkModeAction->setCheckable(true);

    const bool isDark = settings.value("darkMode", false).toBool();
    darkModeAction->setChecked(isDark);
    applyTheme(isDark);

    connect(darkModeAction, &QAction::toggled, this, [this](bool checked) {
        applyTheme(checked);
        QSettings prefs("MyCompany", "ProcessMonitor");
        prefs.setValue("darkMode", checked);
    });

    QMenu* refreshMenu = preferencesMenu->addMenu("Refresh Rate");
    QActionGroup* refreshGroup = new QActionGroup(this);
    refreshGroup->setExclusive(true);

    const int currentRate = settings.value("refreshRateMs", 1000).toInt();

    auto addRefreshAction = [&](const QString& text, int intervalMs) {
        QAction* action = refreshMenu->addAction(text);
        action->setCheckable(true);
        action->setData(intervalMs);
        refreshGroup->addAction(action);

        if (currentRate == intervalMs) {
            action->setChecked(true);
        }
    };

    addRefreshAction("0.5s", 500);
    addRefreshAction("1s", 1000);
    addRefreshAction("2s", 2000);
    addRefreshAction("5s", 5000);

    if (!refreshGroup->checkedAction()) {
        const QList<QAction*> refreshActions = refreshGroup->actions();
        if (!refreshActions.isEmpty()) {
            refreshActions.at(1)->setChecked(true);
        }
    }

    connect(refreshGroup, &QActionGroup::triggered, this, [this](QAction* action) {
        const int intervalMs = action->data().toInt();
        if (intervalMs > 0) {
            refreshTimer->setInterval(intervalMs);
            if (!refreshTimer->isActive()) {
                refreshTimer->start();
            }
            QSettings prefs("MyCompany", "ProcessMonitor");
            prefs.setValue("refreshRateMs", intervalMs);
        }
    });

    if (ui->preferencesButton) {
        ui->preferencesButton->setMenu(preferencesMenu);
    }
}



void MainWindow::applyTheme(bool dark) {
    if (dark) {
        this->setStyleSheet(
            "QMainWindow { background-color: #1e1e1e; }"
            "QTableWidget { background-color: #252526; color: #d4d4d4; gridline-color: #333333; selection-background-color: #094771; selection-color: #ffffff; border: none; alternate-background-color: #2d2d2d; font-size: 9pt; }"
            "QHeaderView::section { background-color: #333333; color: #cccccc; padding: 4px; border: 1px solid #1e1e1e; font-weight: 600; }"
            "QLabel { color: #d4d4d4; font-size: 11pt; }"
            "QToolButton#preferencesButton { background-color: #333333; color: white; border: 1px solid #454545; border-radius: 4px; padding: 6px 10px; }"
            "QToolButton#preferencesButton:hover { background-color: #454545; }"
            "QToolButton#preferencesButton::menu-indicator { image: none; width: 0; }"
            "QPushButton { background-color: #333333; color: white; border: 1px solid #454545; border-radius: 4px; padding: 8px 15px; }"
            "QPushButton:hover { background-color: #454545; }"
            "QPushButton:pressed { background-color: #094771; }"
            );
    } else {
        this->setStyleSheet(
            "QMainWindow { background-color: #f5f5f5; }"
            "QTableWidget { background-color: white; alternate-background-color: #f9f9f9; font-size: 9pt; }"
            "QHeaderView::section { background-color: #e0e0e0; border: 1px solid #cccccc; padding: 4px; }"
            "QToolButton#preferencesButton { padding: 6px 10px; }"
            "QToolButton#preferencesButton::menu-indicator { image: none; width: 0; }"
            "QPushButton { padding: 8px; } "
            );
    }
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
    const QString totalSystemMemText = QString::fromStdString(format_size_from_bytes(SystemInfo::totalMemoryBytes()));
    const QString totalReadText = QString::fromStdString(format_size_from_bytes(totalReadBytesPerSec));
    const QString totalWriteText = QString::fromStdString(format_size_from_bytes(totalWriteBytesPerSec));

    publishSystemMetrics(totalCpuPercent, totalMemBytes, totalReadBytesPerSec, totalWriteBytesPerSec);

    const QStringList headerLabels = {
        "PID",
        QString("Name\n%1 proc").arg(processCountText),
        QString("CPU%\n%1").arg(totalCpuText),
        QString("Memory\n%1 / %2").arg(totalMemText).arg(totalSystemMemText),
        QString("Read\n%1/s").arg(totalReadText),
        QString("Write\n%1/s").arg(totalWriteText),
        "Owner"
    };

    table->setHorizontalHeaderLabels(headerLabels);

    table->setSortingEnabled(true);
    if (sortColumn >= 0) {
        table->sortByColumn(sortColumn, sortOrder);
    }
}
