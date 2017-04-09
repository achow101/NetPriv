#include "overview.h"
#include "ui_overview.h"
#include "capture.h"
#include <QStandardItemModel>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include "chart.h"
#include <QtCharts/QChartView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

QT_CHARTS_USE_NAMESPACE

QMap<QString, int> hostname_data;
QMap<QString, int> prog_data;
int total_packets = 0;
int last_packets = 0;

Overview::Overview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Overview)
{
    //auto update
    QTimer *update_timer = new QTimer(this);
    update_timer->start(500);
    connect(update_timer, SIGNAL(timeout()), SLOT(auto_update_traffic()));

    // Setup ui
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
}

Overview::~Overview()
{
    stop_threads();
    delete ui;
}

void Overview::auto_update_traffic()
{
    int row_index = 0;
    int total_packets = 0;

    // Traffic table
    QStandardItemModel *traffic_model = new QStandardItemModel(1, 2, this);
    traffic_model->setHorizontalHeaderItem(0, new QStandardItem(QString("IP Address")));
    traffic_model->setHorizontalHeaderItem(1, new QStandardItem(QString("Number of Packets Sent")));
    hostname_data = get_hostnames_list();
    for (QString ip: hostname_data.keys()) {
            traffic_model->setVerticalHeaderItem(row_index, new QStandardItem(QString::number(row_index + 1)));
            traffic_model->setItem(row_index, 0, new QStandardItem(ip));
            QStandardItem *number_item = new QStandardItem();
            number_item->setData(QVariant(hostname_data.value(ip)), Qt::DisplayRole);
            traffic_model->setItem(row_index, 1, number_item);
            row_index++;
            total_packets += hostname_data.value(ip);
    }
    row_index = 0;
    traffic_model->sort(1, Qt::DescendingOrder);
    ui->traffic_tbl->setModel(traffic_model);
    ui->traffic_tbl->resizeColumnsToContents();

    // Overview traffic table
    QStandardItemModel *over_traffic_model = new QStandardItemModel(1, 2, this);
    over_traffic_model->setHorizontalHeaderItem(0, new QStandardItem(QString("IP Address")));
    over_traffic_model->setHorizontalHeaderItem(1, new QStandardItem(QString("Number of Packets Sent")));
    for (QString ip: hostname_data.keys()) {
            over_traffic_model->setVerticalHeaderItem(row_index, new QStandardItem(QString::number(row_index + 1)));
            over_traffic_model->setItem(row_index, 0, new QStandardItem(ip));
            QStandardItem *number_item = new QStandardItem();
            number_item->setData(QVariant(hostname_data.value(ip)), Qt::DisplayRole);
            over_traffic_model->setItem(row_index, 1, number_item);
            row_index++;
    }
    row_index = 0;
    over_traffic_model->sort(1, Qt::DescendingOrder);
    over_traffic_model->setRowCount(10);
    ui->over_traffic_tbl->setModel(over_traffic_model);
    ui->over_traffic_tbl->resizeColumnsToContents();

    // Programs table
    QStandardItemModel *prog_model = new QStandardItemModel(1, 2, this);
    prog_model->setHorizontalHeaderItem(0, new QStandardItem(QString("Program Name")));
    prog_model->setHorizontalHeaderItem(1, new QStandardItem(QString("Number of Packets Sent")));
    prog_data = get_programs_map();
    for (QString program_name: prog_data.keys()) {
            prog_model->setVerticalHeaderItem(row_index, new QStandardItem(QString::number(row_index + 1)));
            prog_model->setItem(row_index, 0, new QStandardItem(program_name));
            QStandardItem *number_item = new QStandardItem();
            number_item->setData(QVariant(prog_data.value(program_name)), Qt::DisplayRole);
            prog_model->setItem(row_index, 1, number_item);
            row_index++;
    }
    row_index = 0;
    prog_model->sort(1, Qt::DescendingOrder);
    ui->prog_tbl->setModel(prog_model);
    ui->prog_tbl->resizeColumnsToContents();

    // Overview programs table
    QStandardItemModel *over_prog_model = new QStandardItemModel(1, 2, this);
    over_prog_model->setHorizontalHeaderItem(0, new QStandardItem(QString("Program Name")));
    over_prog_model->setHorizontalHeaderItem(1, new QStandardItem(QString("Number of Packets Sent")));
    for (QString program_name: prog_data.keys()) {
            over_prog_model->setVerticalHeaderItem(row_index, new QStandardItem(QString::number(row_index + 1)));
            over_prog_model->setItem(row_index, 0, new QStandardItem(program_name));
            QStandardItem *number_item = new QStandardItem();
            number_item->setData(QVariant(prog_data.value(program_name)), Qt::DisplayRole);
            over_prog_model->setItem(row_index, 1, number_item);
            row_index++;
    }
    row_index = 0;
    over_prog_model->sort(1, Qt::DescendingOrder);
    over_prog_model->setRowCount(10);
    ui->over_prog_tbl->setModel(over_prog_model);
    ui->over_prog_tbl->resizeColumnsToContents();

    // Summary labels
    ui->tot_packets_disp->setText(QString::number(total_packets));
    ui->num_progs_disp->setText(QString::number(prog_data.keys().size()));
    ui->num_ips_disp->setText(QString::number(hostname_data.keys().size()));

    double rate = (total_packets - last_packets) / 0.5;
    last_packets = total_packets;
    ui->packets_second_disp->setText(QString::number(rate));
}

void Overview::on_actionQuit_triggered()
{
    QApplication::quit();
}
