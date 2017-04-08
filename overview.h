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
};

#endif // OVERVIEW_H
