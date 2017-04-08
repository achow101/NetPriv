#include <QApplication>
#include <QtConcurrent/QtConcurrent>

#include "overview.h"
#include "main.h"
#include "capture.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Overview w;
    w.show();

    // Start pcap thread
    QtConcurrent::run(pcap_thread);

    return a.exec();
}
