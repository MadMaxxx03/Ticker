#include "secondwindow.h"

SecondWindow::SecondWindow(QWidget *parent)
    : QMainWindow(parent) {

    QWidget *centralWidget = new QWidget(); // Создаем основной виджет
    setCentralWidget(centralWidget); // Устанавливаем его как центральный виджет

    QFont labelFont("Times", 14);
    QFont axisFont("Arial", 17);
    int penSize = 5;

    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);

    QHBoxLayout *plotLayout1 = new QHBoxLayout;
    QHBoxLayout *plotLayout2 = new QHBoxLayout;

    plot1 = new QCustomPlot();
    plot2 = new QCustomPlot();
    plot3 = new QCustomPlot();
    plot4 = new QCustomPlot();

    for (QCustomPlot* plot : {plot1, plot2, plot3, plot4}) {
        plot->xAxis->setRange(-1,25);
        plot->xAxis->setTickLabelFont(axisFont);
        plot->yAxis->setTickLabelFont(axisFont);
        plot->xAxis->setLabelFont(labelFont);
        plot->yAxis->setLabelFont(labelFont);
        plot->xAxis->setLabel("Время, с");
        plot->xAxis->setBasePen(QPen(QColor(0, 0, 0), penSize));
        plot->yAxis->setBasePen(QPen(QColor(0, 0, 0), penSize));
        plot->xAxis->setTickPen(QPen(QColor(0, 0, 0), penSize));
        plot->yAxis->setTickPen(QPen(QColor(0, 0, 0), penSize));
        plot->xAxis->grid()->setPen(QPen(QColor(0, 0, 0, 80), 1));
        plot->yAxis->grid()->setPen(QPen(QColor(0, 0, 0, 80), 1));
    }
    plot1->yAxis->setRange(-0.5, 0.5);
    plot2->yAxis->setRange(-1, 1);
    plot3->yAxis->setRange(0, 250);
    plot4->yAxis->setRange(-400, 800);

    plot1->yAxis->setLabel("Координата, м");
    plot2->yAxis->setLabel("Скорость, м/c");
    plot3->yAxis->setLabel("Угол, град");
    plot4->yAxis->setLabel("Угловая скорость, м/с^2");

    plotLayout1->addWidget(plot1);
    plotLayout1->addWidget(plot2);
    plotLayout2->addWidget(plot3);
    plotLayout2->addWidget(plot4);
    centralLayout->addLayout(plotLayout1);
    centralLayout->addLayout(plotLayout2);
    centralWidget->setLayout(centralLayout);
}
