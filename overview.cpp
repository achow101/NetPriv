#include "overview.h"
#include "ui_overview.h"
#include "capture.h"
#include <QStandardItemModel>

QString temp = "hue", a = "";
QMap<QString, int> test = {{temp, 1}, {temp, 2}};
int i, test2 = 100, test3 = 1000;

Overview::Overview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Overview)
{
    ui->setupUi(this);
    for (i = 0; i < test.size(); i++) {
        a += test.firstKey();
        test.remove(test.firstKey());
    }
    ui->label->setText(a);

    QStandardItemModel *model = new QStandardItemModel(2, 3, this);

    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Column1 Header")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Column2 Header")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Column3 Header")));

    QStandardItem *firstRow = new QStandardItem(QString("String Value"));
    model->setItem(0, 0, firstRow);
    QStandardItem *secondRow = new QStandardItem(QString::number(test2));
    //QString printable = QString::fromLatin1("My magic number is %1. That's all!").arg(test2);
    //QStandardItem *secondRow = new QStandardItem(printable);
    QStandardItem *thirdRow = new QStandardItem(QString::fromLatin1("Number is %1").arg(test3));
    model->setItem(1, 0, secondRow);
    model->setItem(2, 0, thirdRow);

    ui->tableView->setModel(model);
}

Overview::~Overview()
{
    stop_threads();
    delete ui;
}
