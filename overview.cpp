#include "overview.h"
#include "ui_overview.h"
#include "capture.h"
#include <QThread>

QString temp = "hue", a = "first";
QMap<QString, int> test = {{temp, 1}, {temp, 2}};
int i;

Overview::Overview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Overview)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(released()), this, SLOT(refresh_pressed()));
//    while (test.size() > 0) {
//        a += test.firstKey();

//        test.remove(test.firstKey());
//    }
    ui->label->setText(a);
    a = "second";
}

Overview::~Overview()
{
    stop_threads();
    delete ui;
}

void Overview::refresh_pressed()
{
    ui->label->setText(a);
}
