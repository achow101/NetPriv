#include "overview.h"
#include "ui_overview.h"
#include "capture.h"
#include <QStandardItemModel>

QString temp = "hue", a = "";
QMap<QString, int> test = {{temp, 1}, {temp, 2}};
int i;

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

    ui->tableView->setModel(model);
}

Overview::~Overview()
{
    stop_threads();
    delete ui;
}
