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

    timer3D = new QTimer(this);
    connect(timer3D, SIGNAL(timeout()), this, SLOT(timer3D_slot()));

    QFont labelFont("Times", 20);
    QFont axisFont("Arial", 25);
    penSize = 5;

    QMenuBar * menuBar = this->menuBar();
    QMenu * fileMenu = menuBar->addMenu("Режим");

    D2ModelingAction = new QAction("2D Моделирование", this);
    connect(D2ModelingAction, &QAction::triggered, this, &MainWindow::on2DSimulationClicked);
    fileMenu->addAction(D2ModelingAction);

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
    labelb2 = new QLabel("Коэф демпф подшипника");
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
    labelW = new QLabel("Угл скорость");

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
        label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        labelsLayout1->addWidget(label);
        labelsLayout1->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    }
    labelk2->setFont(labelFont);
    labelk2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelsLayout1->addWidget(labelk2);

    for (QLineEdit* lineEdit: {editm1, editm2, editm3, editl, editFf, editFr, editFc, editb2}){
        lineEdit->setFont(labelFont);
        lineEdit->setMaximumWidth(120);
        lineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        editsLayout1->addWidget(lineEdit);
        editsLayout1->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    }
    editk2->setFont(labelFont);
    editk2->setMaximumWidth(120);
    editk2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    editsLayout1->addWidget(editk2);

    for (QLabel* label: {labelX, labelV, labelFi}){
        label->setFont(labelFont);
        label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        labelsLayout2->addWidget(label);
        labelsLayout2->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    }
    labelW->setFont(labelFont);
    labelW->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelsLayout2->addWidget(labelW);

    for (QLineEdit* lineEdit: {editX, editV, editFi}){
        lineEdit->setFont(labelFont);
        lineEdit->setMaximumWidth(120);
        lineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        editsLayout2->addWidget(lineEdit);
        editsLayout2->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    }
    editW->setFont(labelFont);
    editW->setMaximumWidth(120);
    editW->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
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

    // Кнопка запуска
    start3DButton = new QPushButton("Запуск");
    start3DButton->setFont(labelFont);
    start3DButton->setFixedHeight(40);
    connect(start3DButton, &QPushButton::clicked, this, &MainWindow::on_start3DButton_clicked);

    QVBoxLayout *D3layout = new QVBoxLayout;

    // Создание окна для отображения 3D-графики
    view = new Qt3DExtras::Qt3DWindow();
    container = QWidget::createWindowContainer(view);
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
    container->setMinimumSize(QSize(500, 500));
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    D3layout->addWidget(container);
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(saveButton);
    buttonsLayout->addWidget(start3DButton);
    LELayout->addLayout(D3layout);
    initialValuesLayout->addLayout(LELayout);
    initialValuesLayout->addLayout(buttonsLayout);
    initialValuesWidget->setLayout(initialValuesLayout);

    // Корневая сущность
    rootEntity = new Qt3DCore::QEntity();

    // Создание камеры
    Qt3DRender::QCamera *camera = view->camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0, 0, 20.0f));
    camera->setViewCenter(QVector3D(0, 0, 0));

    // Добавление контроллера камеры
    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setCamera(camera);

    // Создание неподвижной основной конструкции
    Qt3DCore::QEntity *mainStructureEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DCore::QTransform *mainStructureTransform = new Qt3DCore::QTransform();
    mainStructureEntity->addComponent(mainStructureTransform);

    // Создание верхней планки
    Qt3DCore::QEntity *topBarEntity = new Qt3DCore::QEntity(mainStructureEntity);
    Qt3DExtras::QCuboidMesh *topBarMesh = new Qt3DExtras::QCuboidMesh();
    topBarMesh->setXExtent(10.0f);
    topBarMesh->setYExtent(0.5f);
    topBarMesh->setZExtent(0.5f);
    Qt3DExtras::QPhongMaterial *topBarMaterial = new Qt3DExtras::QPhongMaterial();
    topBarMaterial->setDiffuse(QColor(QRgb(0x404040))); // Серый цвет
    topBarEntity->addComponent(topBarMesh);
    topBarEntity->addComponent(topBarMaterial);

    Qt3DCore::QTransform *topBarTransform = new Qt3DCore::QTransform();
    topBarTransform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));
    topBarEntity->addComponent(topBarTransform);


    // Создание каретки
    Qt3DCore::QEntity *cartEntity = new Qt3DCore::QEntity(mainStructureEntity);
    Qt3DExtras::QCuboidMesh *cartMesh = new Qt3DExtras::QCuboidMesh();
    cartMesh->setXExtent(1.0f);
    cartMesh->setYExtent(1.0f);
    cartMesh->setZExtent(1.0f);
    cartEntity->addComponent(cartMesh);
    Qt3DExtras::QPhongMaterial *cartMaterial = new Qt3DExtras::QPhongMaterial();
    cartMaterial->setDiffuse(QColor(QRgb(0xffd700))); // Жёлтый цвет
    cartEntity->addComponent(cartMaterial);

    cartTransform = new Qt3DCore::QTransform();
    cartEntity->addComponent(cartTransform);

    // Создание боковых стоек
    Qt3DCore::QEntity *leftBarEntity = new Qt3DCore::QEntity(mainStructureEntity);
    Qt3DExtras::QCuboidMesh *sideBarMesh = new Qt3DExtras::QCuboidMesh();
    sideBarMesh->setXExtent(1.0f);
    sideBarMesh->setYExtent(10.0f);
    sideBarMesh->setZExtent(1.0f);
    leftBarEntity->addComponent(sideBarMesh);
    leftBarEntity->addComponent(cartMaterial);  // Используем тот же материал, что и для каретки

    Qt3DCore::QTransform *leftBarTransform = new Qt3DCore::QTransform();
    leftBarTransform->setTranslation(QVector3D(-5.5f, -5.0f, 0.0f)); // Позиционирование слева
    leftBarEntity->addComponent(leftBarTransform);

    Qt3DCore::QEntity *rightBarEntity = new Qt3DCore::QEntity(mainStructureEntity);
    rightBarEntity->addComponent(sideBarMesh);
    rightBarEntity->addComponent(cartMaterial);  // Используем тот же материал, что и для каретки

    Qt3DCore::QTransform *rightBarTransform = new Qt3DCore::QTransform();
    rightBarTransform->setTranslation(QVector3D(5.5f, -5.0f, 0.0f)); // Позиционирование справа
    rightBarEntity->addComponent(rightBarTransform);

    // Создание стержня маятника
    Qt3DCore::QEntity *rodEntity = new Qt3DCore::QEntity(cartEntity);
    Qt3DExtras::QCylinderMesh *rodMesh = new Qt3DExtras::QCylinderMesh();
    rodMesh->setRadius(0.1f);
    rodMesh->setLength(5.0f);
    Qt3DExtras::QPhongMaterial *rodMaterial = new Qt3DExtras::QPhongMaterial();
    rodMaterial->setDiffuse(QColor(QRgb(0x808080)));
    rodEntity->addComponent(rodMesh);
    rodEntity->addComponent(rodMaterial);

    rodTransform = new Qt3DCore::QTransform();
    rodTransform->setTranslation(QVector3D(0.0f, -2.5f, 0.0f));
    rodEntity->addComponent(rodTransform);

    // Создание груза маятника
    Qt3DCore::QEntity *weightEntity = new Qt3DCore::QEntity(rodEntity);
    Qt3DExtras::QSphereMesh *weightMesh = new Qt3DExtras::QSphereMesh();
    weightMesh->setRadius(0.5f);
    Qt3DExtras::QPhongMaterial *weightMaterial = new Qt3DExtras::QPhongMaterial();
    weightMaterial->setDiffuse(QColor(QRgb(0xff0000)));
    weightEntity->addComponent(weightMesh);
    weightEntity->addComponent(weightMaterial);

    Qt3DCore::QTransform *weightTransform = new Qt3DCore::QTransform();
    weightTransform->setTranslation(QVector3D(0.0f, -2.5f, 0.0f)); // Изменено: позиционирование относительно стержня
    weightEntity->addComponent(weightTransform);

    // Создание источника света
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QDirectionalLight *light = new Qt3DRender::QDirectionalLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1.0f);
    lightEntity->addComponent(light);

    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform();
    lightTransform->setTranslation(QVector3D(0.0f, 20.0f, 20.0f));
    lightTransform->setRotation(QQuaternion::fromEulerAngles(-45.0f, 0.0f, 0.0f));
    lightEntity->addComponent(lightTransform);

    // Установка корневой сущности
    view->setRootEntity(rootEntity);

    //cartAnimation = new QPropertyAnimation(cartTransform, "translation");
    cartTransform = new Qt3DCore::QTransform();
    cartTransform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));
    cartEntity->addComponent(cartTransform);
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

void MainWindow::on2DSimulationClicked(){
    stackedWidget->setCurrentWidget(centralWidget);
    timer3D->stop();
}

void MainWindow::onSetValuesClicked(){
    stackedWidget->setCurrentWidget(initialValuesWidget);

    timer->stop();
    timer3D->stop();

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

void MainWindow::on_start3DButton_clicked(){
    if (isFirstReadFlag){
        modifiIni("C:/Users/baben_bakg1j1/Programming/C++/Ticker/app/values.ini", values);
        isFirstReadFlag = false;
    }
    values = MainWindow::readIni("C:/Users/baben_bakg1j1/Programming/C++/Ticker/app/values.ini", "Modified");
    constants = {values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7], values[8]};
    val = {values[9], values[10], values[11], values[12]};
    result = MainWindow::rungeKutta(0, 1, 100, val, constants);

    timer3D->start(100);
}

void MainWindow::timer3D_slot(){
    result = MainWindow::rungeKutta(0, 1, 100, result, constants);

    if (result[0] > l_max || result[0] < -l_max)
        result[1] = 0;

    float angle = MainWindow::to_degrees(result[2]) - 180;
    QMatrix4x4 matrix;
    matrix.rotate(angle, QVector3D(0.0f, 0.0f, 1.0f));
    matrix.translate(QVector3D(0.0f, -2.5f, 0.0f)); // Позиционирование стержня относительно каретки

    QMatrix4x4 matrix2;
    matrix2.rotate(0, QVector3D(0.0f, 0.0f, 1.0f));
    matrix2.translate(QVector3D(result[0] * 100 / 3, 0.0f, 0.0f)); // Позиционирование стержня относительно каретки

    cartTransform->setMatrix(matrix2);
    rodTransform->setMatrix(matrix);
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
