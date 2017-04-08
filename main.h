#ifndef MAIN_H
#define MAIN_H
#include <QApplication>
#include <QMap>
#include <QMutex>

QMutex lock;
QMap<QString, int> hostnames_map;

#endif // MAIN_H
