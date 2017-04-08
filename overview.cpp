#include "overview.h"
#include "ui_overview.h"
#include "capture.h"
#include <QStandardItemModel>
#include <QThread>
#include <QDebug>
#include <QTimer>

QString temp = "hue", a = "";
QMap<QString, int> test;
QMap<QString, int> programs;
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
    QStandardItemModel *model = new QStandardItemModel(1, 2, this);

    model->setHorizontalHeaderItem(0, new QStandardItem(QString("IP Address")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Number of Packets Sent")));
    test = get_hostnames_list();
    for (QString ip: test.keys()) {
            model->setVerticalHeaderItem(row_index, new QStandardItem(QString::number(row_index + 1)));
            model->setItem(row_index, 0, new QStandardItem(ip));
            model->setItem(row_index, 1, new QStandardItem(QString::number(test.value(ip))));
            row_index++;
    }
    row_index = 0;
    ui->tableView->setModel(model);

    //second one
    QStandardItemModel *model2 = new QStandardItemModel(1, 2, this);

    model2->setHorizontalHeaderItem(0, new QStandardItem(QString("Program Name")));
    model2->setHorizontalHeaderItem(1, new QStandardItem(QString("Number of Packets Sent")));
    programs = get_programs_map();
    for (QString program_name: programs.keys()) {
            model2->setVerticalHeaderItem(row_index, new QStandardItem(QString::number(row_index + 1)));
            model2->setItem(row_index, 0, new QStandardItem(program_name));
            model2->setItem(row_index, 1, new QStandardItem(QString::number(programs.value(program_name))));
            row_index++;
    }
    row_index = 0;
    ui->tableView_2->setModel(model2);
}
