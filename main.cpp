#include <QApplication>
#include <QtConcurrent/QtConcurrent>

#include "overview.h"
#include "main.h"
#include "capture.h"

#include "chart.h"
#include <QtCharts/QChartView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Overview w;

    QMainWindow window;
    Chart *chart = new Chart;
    chart->setTitle("Dynamic spline chart");
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::AllAnimations);
    QChartView chartView(chart);
    chartView.setRenderHint(QPainter::Antialiasing);
    window.setCentralWidget(&chartView);
    window.resize(400, 300);
    window.show();

    w.show();

    // Start pcap thread
    QtConcurrent::run(pcap_thread);

    return a.exec();
}
