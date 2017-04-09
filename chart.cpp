#include "chart.h"
#include "capture.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QTime>
#include <QtCore/QDebug>

int last_val = 0;

Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    m_series(0),
    m_axis(new QValueAxis),
    m_step(0),
    m_x(5),
    m_y(1)
{
    qsrand((uint) QTime::currentTime().msec());

    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    m_timer.setInterval(1000);

    m_series = new QSplineSeries(this);
    QPen green(Qt::red);
    green.setWidth(3);
    m_series->setPen(green);
    m_series->append(m_x, m_y);

    addSeries(m_series);
    createDefaultAxes();
    setAxisX(m_axis, m_series);
    m_axis->setTickCount(5);
    axisX()->setRange(0, 10);
    axisY()->setRange(0, 10); // change this one

    m_timer.start();
}

Chart::~Chart()
{

}

void Chart::handleTimeout()
{
    int j = 0;
    QMap<QString, int> test = get_hostnames_list();
    for (int i : test.values()) {
        j += i;
    }

    qreal x = plotArea().width() / m_axis->tickCount();
    qreal y = (m_axis->max() - m_axis->min()) / m_axis->tickCount();
    m_x += y;
    m_y = j - last_val;
    m_series->append(m_x, m_y);
    scroll(x, 0);
    last_val = j;
//    if (m_x == 100)
//        m_timer.stop();
}
