#ifndef OVERVIEW_H
#define OVERVIEW_H

#include <QMainWindow>

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
    void on_pushButton_3_pressed();
};

#endif // OVERVIEW_H
