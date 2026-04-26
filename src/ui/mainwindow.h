#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QSettings>
#include <QTimer>
#include "../core/Process.h"
#include "../core/ProcessMonitor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void updateList(const std::vector<Process>& processes);

private:
    void applyTheme(bool dark);
    void setupPreferencesMenu();
    void publishSystemMetrics(double totalCpuPercent, double totalMemBytes, double totalReadBytesPerSec, double totalWriteBytesPerSec);

    Ui::MainWindow* ui;
    ProcessMonitor processMonitor;
    QNetworkAccessManager* metricsNetworkManager;
    QTimer* refreshTimer;
    int findRowByPid(const QString& pid) const;
};
#endif // MAINWINDOW_H
