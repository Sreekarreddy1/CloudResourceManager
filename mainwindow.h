#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private:
    void updateMemoryUsage();
private:
    void updateStorageUsage();
private:
     void updateCpuUsage();

QTimer *cpuTimer;

FILETIME previousIdleTime{};
FILETIME previousKernelTime{};
FILETIME previousUserTime{};
bool firstCpuReading = true;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
