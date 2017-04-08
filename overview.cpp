#include "overview.h"
#include "ui_overview.h"
#include "main.h"

QMap<QString, int> test;

Overview::Overview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Overview)
{
    ui->setupUi(this);
}

Overview::~Overview()
{
    delete ui;
}
