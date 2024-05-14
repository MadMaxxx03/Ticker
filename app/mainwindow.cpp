#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Маятник");

    stepT = 0.1;
    beginT = 0;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_slot()));

    QFont labelFont("Times", 20);
    QFont axisFont("Arial", 25);
    penSize = 5;

    QMenuBar * menuBar = this->menuBar();
    QMenu * fileMenu = menuBar->addMenu("Режим");

    modelingAction = new QAction("Моделирование", this);
    connect(modelingAction, &QAction::triggered, this, &MainWindow::onSimulationClicked);
    fileMenu->addAction(modelingAction);

    initValAction = new QAction("Задать начальные значения", this);
    connect(initValAction, &QAction::triggered, this, &MainWindow::onSetValuesClicked);
    fileMenu->addAction(initValAction);

    initialValuesWidget = new QWidget;
    QVBoxLayout *initialValuesLayout = new QVBoxLayout;
    QHBoxLayout *LELayout = new QHBoxLayout;

    QHBoxLayout *LEHLayout1 = new QHBoxLayout;
    QVBoxLayout *LEVLayout1 = new QVBoxLayout;
    QVBoxLayout *labelsLayout1 = new QVBoxLayout;
    QVBoxLayout *editsLayout1 = new QVBoxLayout;

    labeModelParams = new QLabel("Параметры модели");
    labeModelParams->setFont(axisFont);

    labelm1 = new QLabel("Масса тележки");
    labelm2 = new QLabel("Масса на конце стержня");
    labelm3 = new QLabel("Масса стержня");
    labell = new QLabel("Длина стержня");
    labelm2 = new QLabel("Масса на конце стержня");
    labelFf = new QLabel("Сила трения");
    labelFr = new QLabel("Сила сопротивления");
    labelFc = new QLabel("Управление");
    labelb2 = new QLabel("Коэф демпфирования подшипника");
    labelk2 = new QLabel("Коэф жескости пружины");

    editm1 = new QLineEdit;
    editm2 = new QLineEdit;
    editm3 = new QLineEdit;
    editl = new QLineEdit;
    editFf = new QLineEdit;
    editFr = new QLineEdit;
    editFc = new QLineEdit;
    editb2 = new QLineEdit;
    editk2 = new QLineEdit;

    QHBoxLayout *LEHLayout2 = new QHBoxLayout;
    QVBoxLayout *LEVLayout2 = new QVBoxLayout;
    QVBoxLayout *labelsLayout2 = new QVBoxLayout;
    QVBoxLayout *editsLayout2 = new QVBoxLayout;

    labeInitVal = new QLabel("Начальные условия");
    labeInitVal->setFont(axisFont);

    labelX = new QLabel("Координата");
    labelV = new QLabel("Скорость");
    labelFi = new QLabel("Угол");
    labelW = new QLabel("Угловая скорость");

    editX = new QLineEdit;
    editV = new QLineEdit;
    editFi = new QLineEdit;
    editW = new QLineEdit;

    fields = {editm1, editm2, editm3, editl, editFf, editFr, editFc, editb2, editk2, editX, editV, editFi, editW};

    saveButton = new QPushButton("Сохранить значения");
    saveButton->setFixedHeight(40);
    saveButton->setFont(labelFont);

    connect(saveButton, &QPushButton::clicked, this, &MainWindow::on_saveButton_clicked);

    for (QLabel* label: {labelm1, labelm2, labelm3, labell, labelFf, labelFr, labelFc, labelb2}){
        label->setFont(labelFont);
        labelsLayout1->addWidget(label);
        labelsLayout1->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    }
    labelk2->setFont(labelFont);
    labelsLayout1->addWidget(labelk2);

    for (QLineEdit* lineEdit: {editm1, editm2, editm3, editl, editFf, editFr, editFc, editb2}){
        lineEdit->setFont(labelFont);
        editsLayout1->addWidget(lineEdit);
        editsLayout1->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    }
    editk2->setFont(labelFont);
    editsLayout1->addWidget(editk2);

    for (QLabel* label: {labelX, labelV, labelFi}){
        label->setFont(labelFont);
        labelsLayout2->addWidget(label);
        labelsLayout2->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    }
    labelW->setFont(labelFont);
    labelsLayout2->addWidget(labelW);

    for (QLineEdit* lineEdit: {editX, editV, editFi}){
        lineEdit->setFont(labelFont);
        editsLayout2->addWidget(lineEdit);
        editsLayout2->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    }
    editW->setFont(labelFont);
    editsLayout2->addWidget(editW);

    LEHLayout1->addLayout(labelsLayout1);
    LEHLayout1->addLayout(editsLayout1);
    LEVLayout1->addWidget(labeModelParams);
    LEVLayout1->addSpacing(10);
    LEVLayout1->addLayout(LEHLayout1);

    LEHLayout2->addLayout(labelsLayout2);
    LEHLayout2->addLayout(editsLayout2);
    LEVLayout2->addWidget(labeInitVal);
    LEVLayout2->addSpacing(10);
    LEVLayout2->addLayout(LEHLayout2);

    LELayout->addLayout(LEVLayout1);
    LELayout->addLayout(LEVLayout2);

    initialValuesLayout->addLayout(LELayout);
    initialValuesLayout->addWidget(saveButton);
    initialValuesWidget->setLayout(initialValuesLayout);

    centralWidget = new QWidget;
    QVBoxLayout *centralayout = new QVBoxLayout;
    QHBoxLayout *plotLayout1 = new QHBoxLayout;
    QHBoxLayout *plotLayout2 = new QHBoxLayout;
    QHBoxLayout *buttonCentralLayout = new QHBoxLayout;

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

    startButton = new QPushButton("Запуск");
    startButton->setFont(labelFont);
    startButton->setFixedWidth(200);
    startButton->setFixedHeight(40);
    buttonCentralLayout->addWidget(startButton);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::on_startButton_clicked);

    plotLayout1->addWidget(plot1);
    plotLayout1->addWidget(plot2);
    plotLayout2->addWidget(plot3);
    plotLayout2->addWidget(plot4);
    centralayout->addLayout(plotLayout1);
    centralayout->addLayout(plotLayout2);
    centralayout->addLayout(buttonCentralLayout);
    centralWidget->setLayout(centralayout);

    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(centralWidget);
    stackedWidget->addWidget(initialValuesWidget);
    setCentralWidget(stackedWidget);
    stackedWidget->setCurrentWidget(centralWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

const double l_max = 0.3;
bool isFirstReadFlag = true;

QVector<double> values = MainWindow::readIni("C:/Users/baben_bakg1j1/Programming/C++/Ticker/app/values.ini", "Base");
QVector<double> constants = {values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7], values[8]};
QVector<double> val = {values[9], values[10], values[11], values[12]};

QVector<double> result = MainWindow::rungeKutta(0, 1, 100, val, constants);

void MainWindow::onSimulationClicked(){
    stackedWidget->setCurrentWidget(centralWidget);
}

void MainWindow::onSetValuesClicked(){
    stackedWidget->setCurrentWidget(initialValuesWidget);

    timer->stop();
    for (QCustomPlot* plot : {plot1, plot2, plot3, plot4}) {
        plot->clearGraphs();
        plot->replot();
    }

    for (int i = 0; i < fields.size(); i++) {
        fields[i]->setText(QString::number(values[i]));
        fields[i]->setFont(QFont ("Times", 20));
    }
}

void MainWindow::on_saveButton_clicked(){
    QString inputText;
    bool conversionOK;
    double inputValue;
    bool isAllOk = true;
    QVector<double> newValues;

    for (QLineEdit* field : fields) {
        field->setStyleSheet("background-color: white;");
        inputText = field->text();
        inputValue = inputText.toDouble(&conversionOK);

        if(!conversionOK){
            field->setStyleSheet("background-color: red;");
            isAllOk = false;
        }

        newValues.push_back(inputValue);
    }

    if (isAllOk)
        modifiIni("C:/Users/baben_bakg1j1/Programming/C++/Ticker/app/values.ini", newValues);
}

void MainWindow::on_startButton_clicked(){
    if (isFirstReadFlag){
        modifiIni("C:/Users/baben_bakg1j1/Programming/C++/Ticker/app/values.ini", values);
        isFirstReadFlag = false;
    }
    values = MainWindow::readIni("C:/Users/baben_bakg1j1/Programming/C++/Ticker/app/values.ini", "Modified");
    constants = {values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7], values[8]};
    val = {values[9], values[10], values[11], values[12]};
    result = MainWindow::rungeKutta(0, 1, 100, val, constants);

    T = beginT;
    plotTime.clear();
    plotXY.clear();
    plotVxY.clear();
    plotFiY.clear();
    plotOmegaFiY.clear();

    for (QCustomPlot* plot : {plot1, plot2, plot3, plot4}) {
        plot->clearGraphs();
    }

    timer->start(100);
}

void MainWindow::timer_slot(){
    result = MainWindow::rungeKutta(0, 1, 100, result, constants);

    if (result[0] > l_max || result[0] < -l_max)
        result[1] = 0;

    plotTime.push_back(T);
    plotXY.push_back(result[0]);
    plotVxY.push_back(result[1]);
    plotFiY.push_back(MainWindow::to_degrees(result[2]));
    plotOmegaFiY.push_back(MainWindow::to_degrees(result[3]));

    T += stepT;

    plot1->addGraph();
    plot1->graph()->setPen(QPen(QColor(0, 0, 255), penSize));
    plot1->graph(0)->addData(plotTime, plotXY);
    plot1->replot();

    plot2->addGraph();
    plot2->graph()->setPen(QPen(QColor(0, 0, 255), penSize));
    plot2->graph(0)->addData(plotTime, plotVxY);
    plot2->replot();

    plot3->addGraph();
    plot3->graph()->setPen(QPen(QColor(0, 0, 255), penSize));
    plot3->graph(0)->addData(plotTime, plotFiY);
    plot3->replot();

    plot4->addGraph();
    plot4->graph()->setPen(QPen(QColor(0, 0, 255), penSize));
    plot4->graph(0)->addData(plotTime, plotOmegaFiY);
    plot4->replot();
}
