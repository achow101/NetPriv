#include "overview.h"
#include "ui_overview.h"

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
