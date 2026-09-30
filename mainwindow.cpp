#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>

#ifdef Q_OS_WIN
#include <windows.h>
#endif





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QLineSeries *cpuSeries = new QLineSeries();
    QLineSeries();

    QLineSeries *memorySeries = new QLineSeries();
    QLineSeries();

    QLineSeries *storageSeries = new QLineSeries();
    QLineSeries();

    // Sample data
    cpuSeries->append(0, 35);
    cpuSeries->append(1, 42);
    cpuSeries->append(2, 48);
    cpuSeries->append(3, 40);
    cpuSeries->append(4, 45);

    memorySeries->append(0, 50);
    memorySeries->append(1, 61);
    memorySeries->append(2, 64);
    memorySeries->append(3, 58);
    memorySeries->append(4, 63);

    storageSeries->append(0, 68);
    storageSeries->append(1, 72);
    storageSeries->append(2, 73);
    storageSeries->append(3, 72);
    storageSeries->append(4, 74);

    QChart *chart = new QChart();
    chart->setBackgroundBrush(QBrush(QColor("#2b2b2b")));
    chart->setTitleBrush(QBrush(QBrush(Qt::white)));


    chart->addSeries(cpuSeries);
    chart->addSeries(memorySeries);
    chart->addSeries(storageSeries);

    chart->setTitle("Resource Usage History");

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, 4);
    axisX->setTitleText("Time");
    axisX->setLabelsColor(Qt::white);
    axisX->setTitleBrush(QBrush(QBrush(Qt::white)));

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setTitleText("Usage (%)");
    axisY->setLabelsColor(Qt::white);
    axisY->setTitleBrush(QBrush(QBrush(Qt::white)));

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    cpuSeries->attachAxis(axisX);
    cpuSeries->attachAxis(axisY);

    memorySeries->attachAxis(axisX);
    memorySeries->attachAxis(axisY);

    storageSeries->attachAxis(axisX);
    storageSeries->attachAxis(axisY);

    //ui->resourceChartView->setChart(chart);
    //ui->resourceChartView->setRenderHint(QPainter::Antialiasing);
    QChartView *chartView = new QChartView(chart);

    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout(ui->resourceChartView);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
    //chartView->setMinimumSize(700,250);



    updateMemoryUsage();
    updateStorageUsage();


    cpuTimer = new QTimer(this);

    connect(cpuTimer, &QTimer::timeout,
            this, &MainWindow::updateCpuUsage);

    cpuTimer->start(1000);


}

MainWindow::~MainWindow()
{
    delete ui;
}






void MainWindow::updateMemoryUsage()
{
//ifdef Q_OS_WIN


    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memoryStatus)){
        double totalGB =static_cast<double>(memoryStatus.ullTotalPhys) /(1024.0 * 1024.0 * 1024.0);
    double freeGB =static_cast<double>(memoryStatus.ullAvailPhys) /(1024.0 * 1024.0 * 1024.0);
    double usedGB = totalGB-freeGB;

    int usagePercent = static_cast<int>(memoryStatus.dwMemoryLoad);

    ui->memoryUsageLabel->setText(QString("%1%").arg(usagePercent));

    ui->memoryTotalLabel->setText(QString("Total: %1 GB").arg(totalGB, 0, 'f', 1));

    ui->memoryUsedLabel->setText(QString("Used: %1 GB").arg(usedGB, 0, 'f', 1));

    ui->memoryFreeLabel->setText(QString("Free: %1 GB").arg(freeGB, 0, 'f', 1));

    ui->memoryProgressBar->setValue(usagePercent);
    }

}


void MainWindow::updateStorageUsage()
{
    ULARGE_INTEGER freeBytes;
    ULARGE_INTEGER totalBytes;
    ULARGE_INTEGER availableBytes;

    if (GetDiskFreeSpaceExW(L"C:\\",
                            &availableBytes,
                            &totalBytes,
                            &freeBytes))
    {
        double totalGB =
            static_cast<double>(totalBytes.QuadPart) /
            (1024.0 * 1024.0 * 1024.0);

        double freeGB =
            static_cast<double>(freeBytes.QuadPart) /
            (1024.0 * 1024.0 * 1024.0);

        double usedGB = totalGB - freeGB;

        int usagePercent =
            static_cast<int>((usedGB / totalGB) * 100);

        ui->storageUsagelabel->setText(QString("%1%").arg(usagePercent));
        ui->storageTotalLabel->setText(QString("Total: %1 GB").arg(totalGB, 0, 'f', 1));

        ui->storageUsedLabel->setText(QString("Used: %1 GB").arg(usedGB, 0, 'f', 1));

        ui->storageFreeLabel->setText(QString("Free: %1 GB").arg(freeGB, 0, 'f', 1));

        ui->storageProgressBar->setValue(usagePercent);
    }
}

void MainWindow::updateCpuUsage(){
    FILETIME idleTime;
    FILETIME kernelTime;
    FILETIME userTime;
    ULARGE_INTEGER idle;
    ULARGE_INTEGER kernel;
    ULARGE_INTEGER user;

    ULARGE_INTEGER previousIdle;
    ULARGE_INTEGER previousKernel;
    ULARGE_INTEGER previousUser;


    if (GetSystemTimes(&idleTime, &kernelTime, &userTime))
    {
        idle.LowPart = idleTime.dwLowDateTime;
        idle.HighPart = idleTime.dwHighDateTime;

        kernel.LowPart = kernelTime.dwLowDateTime;
        kernel.HighPart = kernelTime.dwHighDateTime;

        user.LowPart = userTime.dwLowDateTime;
        user.HighPart = userTime.dwHighDateTime;

        previousIdle.LowPart = previousIdleTime.dwLowDateTime;
        previousIdle.HighPart = previousIdleTime.dwHighDateTime;

        previousKernel.LowPart = previousKernelTime.dwLowDateTime;
        previousKernel.HighPart = previousKernelTime.dwHighDateTime;

        previousUser.LowPart = previousUserTime.dwLowDateTime;
        previousUser.HighPart = previousUserTime.dwHighDateTime;


        // Calculate differences

        ULARGE_INTEGER idleDiff;
        ULARGE_INTEGER kernelDiff;
        ULARGE_INTEGER userDiff;

        idleDiff.QuadPart = idle.QuadPart - previousIdle.QuadPart;
        kernelDiff.QuadPart = kernel.QuadPart - previousKernel.QuadPart;
        userDiff.QuadPart = user.QuadPart - previousUser.QuadPart;
        ULARGE_INTEGER totalDiff;
        ULARGE_INTEGER busyDiff;

        totalDiff.QuadPart = kernelDiff.QuadPart + userDiff.QuadPart;
        busyDiff.QuadPart = totalDiff.QuadPart - idleDiff.QuadPart;
        double cpuUsage = 0.0;

        if (totalDiff.QuadPart > 0)
        {
            cpuUsage =
                (static_cast<double>(busyDiff.QuadPart) /
                 static_cast<double>(totalDiff.QuadPart)) * 100.0;
        }
        previousIdleTime = idleTime;
        previousKernelTime = kernelTime;
        previousUserTime = userTime;

        int cpuPercent = static_cast<int>(cpuUsage);

        ui->cpuUsageLabel->setText(
            QString("%1%").arg(cpuPercent)
            );

        ui->cpuProgressBar->setValue(cpuPercent);
        if (firstCpuReading)
        {
            previousIdleTime = idleTime;
            previousKernelTime = kernelTime;
            previousUserTime = userTime;

            firstCpuReading = false;
            return;
        }
    }

}



