#include "overview.h"
#include "ui_overview.h"
#include "capture.h"
#include <QStandardItemModel>
#include <QThread>
#include <QDebug>

QString temp = "hue", a = "";
QMap<QString, int> test;
int i, test2 = 100, test3 = 1000, row_index = 0;

Overview::Overview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Overview)
{
    test = get_hostnames_list();
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(released()), this, SLOT(refresh_pressed()));
    ui->label->setText(a);

    QStandardItemModel *model = new QStandardItemModel(1, 2, this);

    model->setHorizontalHeaderItem(0, new QStandardItem(QString("IP Address")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Number of Packets Sent")));
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
}

Overview::~Overview()
{
    stop_threads();
    delete ui;
}

void Overview::refresh_pressed()
{
    test = get_hostnames_list();
    for (QString h : test.keys()) {
        a += h + "\n";
    }
    ui->label->setText(a);
}

void Overview::on_pushButton_3_pressed()
{
    QStandardItemModel *model = new QStandardItemModel(2, 3, this);

    model->setHorizontalHeaderItem(0, new QStandardItem(QString("IP Adress")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Number of Packets Sent")));
    //model->setHorizontalHeaderItem(2, new QStandardItem(QString("Column3 Header")));
    test = get_hostnames_list();
    for (QString ip: test.keys()) {
            model->setVerticalHeaderItem(row_index, new QStandardItem(QString::number(row_index + 1)));
            model->setItem(row_index, 0, new QStandardItem(ip));
            model->setItem(row_index, 1, new QStandardItem(QString::number(test.value(ip))));
            row_index++;
    }
    row_index = 0;
    ui->tableView->setModel(model);
}
