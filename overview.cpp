#include "overview.h"
#include "ui_overview.h"
#include "capture.h"
#include <QStandardItemModel>
#include <QThread>
#include <QDebug>
#include <QTimer>

QString temp = "hue", a = "";
QMap<QString, int> hostname_data;
QMap<QString, int> prog_data;
int i, test2 = 100, test3 = 1000, row_index = 0;

Overview::Overview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Overview)
{
    //auto update
    QTimer *update_timer = new QTimer(this);
    update_timer->start(500);
    connect(update_timer, SIGNAL(timeout()), SLOT(auto_update_traffic()));


    //Abel's mess below
    ui->setupUi(this);

    QStandardItemModel *model = new QStandardItemModel(1, 2, this);

    model->setHorizontalHeaderItem(0, new QStandardItem(QString("IP Address")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Number of Packets Sent")));

    QStandardItemModel *model2 = new QStandardItemModel(1, 2, this);

    model2->setHorizontalHeaderItem(0, new QStandardItem(QString("Program Name")));
    model2->setHorizontalHeaderItem(1, new QStandardItem(QString("Number of Packets Sent")));
    //model->setHorizontalHeaderItem(2, new QStandardItem(QString("Column3 Header")));

    /*
    QStandardItem *firstRow = new QStandardItem(QString("String Value"));
    model->setItem(0, 0, firstRow);
    QStandardItem *secondRow = new QStandardItem(QString::number(test2));
    //QString printable = QString::fromLatin1("My magic number is %1. That's all!").arg(test2);
    //QStandardItem *secondRow = new QStandardItem(printable);
    QStandardItem *thirdRow = new QStandardItem(QString::fromLatin1("Number is %1").arg(test3));
    model->setItem(1, 0, secondRow);
    model->setItem(2, 0, thirdRow);
    */
    ui->tableView->setModel(model);
    ui->tableView_2->setModel(model2);
}

Overview::~Overview()
{
    stop_threads();
    delete ui;
}

void Overview::auto_update_traffic()
{
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
    }
    row_index = 0;
    traffic_model->sort(1, Qt::DescendingOrder);
    ui->tableView->setModel(traffic_model);

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
    ui->tableView_2->setModel(prog_model);
}
