#include "overview.h"
#include "ui_overview.h"
#include "main.h"

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
}

Overview::~Overview()
{
    delete ui;
}
