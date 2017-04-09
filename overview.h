#ifndef OVERVIEW_H
#define OVERVIEW_H

#include <QMainWindow>
#include <QStandardItemModel>

namespace Ui {
class Overview;
}

class Overview : public QMainWindow
{
    Q_OBJECT

public:
    explicit Overview(QWidget *parent = 0);
    ~Overview();

private:
    Ui::Overview *ui;

private slots:
    void auto_update_traffic();
};

void copyTop10(QStandardItemModel* from, QStandardItemModel* to);

#endif // OVERVIEW_H
