#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFont>
#include "qcustomplot.h"

// SecondWindow.h
class SecondWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit SecondWindow(QWidget *parent = nullptr);
    QCustomPlot* getPlot1() const { return plot1; }
    QCustomPlot* getPlot2() const { return plot2; }
    QCustomPlot* getPlot3() const { return plot3; }
    QCustomPlot* getPlot4() const { return plot4; }

private:
    QCustomPlot* plot1;
    QCustomPlot* plot2;
    QCustomPlot* plot3;
    QCustomPlot* plot4;
};
#endif // SECONDWINDOW_H
