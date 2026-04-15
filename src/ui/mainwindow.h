#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void updateList(const std::vector<Process>& processes);

private slots:
    void on_pushButton_clicked();
    void on_darkModeButton_toggled(bool checked); // Handles the click
    void applyTheme(bool dark);                  // Helper to change colors
private:
    Ui::MainWindow* ui;
    ProcessMonitor processMonitor;
    QTimer* refreshTimer;
    int findRowByPid(const QString& pid) const;
};
#endif // MAINWINDOW_H
