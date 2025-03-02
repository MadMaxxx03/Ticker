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

    timerStend = new QTimer(this);
    connect(timerStend, SIGNAL(timeout()), this, SLOT(timerStend_slot()));

    //QFont labelFont("Times", 8);
    //QFont axisFont("Arial", 10);
    //penSize = 3;
    //localPath = "C:/Ticker/real_model_version/app";

    QFont labelFont("Times", 15);
    QFont axisFont("Arial", 20);
    penSize = 5;
    localPath = "C:/Users/baben_bakg1j1/Programming/C++/Ticker/real_model_version/app";
    localPath = "C:/Users/baben/programming/bmstu/Ticker/real_model_version/app";

    serial = new QSerialPort(this);

    QMenuBar *menuBar = this->menuBar();

    QMenu *fileMenu = menuBar->addMenu("Режим");

    modelingAction = new QAction("Моделирование", this);
    connect(modelingAction, &QAction::triggered, this, &MainWindow::onSimulationClicked);
    fileMenu->addAction(modelingAction);

    stendAction = new QAction("Сненд", this);
    connect(stendAction, &QAction::triggered, this, &MainWindow::onStendClicked);
    fileMenu->addAction(stendAction);

    menuBar->setFont(labelFont);
    fileMenu->setFont(labelFont);

    initialValuesWidget = new QWidget;
    QVBoxLayout *initialValuesLayout = new QVBoxLayout;
    QVBoxLayout *menuLayout = new QVBoxLayout;

    QHBoxLayout *LEHLayout1 = new QHBoxLayout;
    QVBoxLayout *LEVLayout1 = new QVBoxLayout;
    QVBoxLayout *labelsLayout1 = new QVBoxLayout;
    QVBoxLayout *editsLayout1 = new QVBoxLayout;

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

    labelX = new QLabel("Координата");
    labelV = new QLabel("Скорость");
    labelFi = new QLabel("Угол");
    labelW = new QLabel("Угловая скорость");

    editX = new QLineEdit;
    editV = new QLineEdit;
    editFi = new QLineEdit;
    editW = new QLineEdit;

    fields = {editm1, editm2, editm3, editl, editFf, editFr, editFc, editb2, editk2, editX, editV, editFi, editW};

    extraParamsStendButton = new QPushButton("Управление и наблюдатель");
    extraParamsStendButton->setFixedHeight(40);
    extraParamsStendButton->setFont(labelFont);
    connect(extraParamsStendButton, &QPushButton::clicked, this, &MainWindow::on_extraParamsButton_clicked);

    extraParamsButton = new QPushButton("Управление и наблюдатель");
    extraParamsButton->setFixedHeight(40);
    extraParamsButton->setFont(labelFont);
    connect(extraParamsButton, &QPushButton::clicked, this, &MainWindow::on_extraParamsButton_clicked);


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
    LEVLayout1->addSpacing(10);
    LEVLayout1->addLayout(LEHLayout1);

    LEHLayout2->addLayout(labelsLayout2);
    LEHLayout2->addLayout(editsLayout2);
    LEVLayout2->addSpacing(10);
    LEVLayout2->addLayout(LEHLayout2);

    QGroupBox *modelParamsGroup = new QGroupBox("Параметры модели");
    modelParamsGroup->setLayout(LEVLayout1);
    QGroupBox *initValGroup = new QGroupBox("Начальные условия");
    initValGroup->setLayout(LEVLayout2);

    menuLayout->addWidget(modelParamsGroup);
    menuLayout->addWidget(initValGroup);

    // Кнопка запуска
    startButton = new QPushButton("Запуск");
    startButton->setFont(labelFont);
    startButton->setFixedHeight(40);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::on_startButton_clicked);

    // Кнопка остановки моделирования
    stopButton = new QPushButton("Остановка моделирования");
    stopButton->setFont(labelFont);
    stopButton->setFixedHeight(40);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::on_stopButton_clicked);

    // Кнопка сохранения значений в файл
    writeButton = new QPushButton("Сохранение значений в файл");
    writeButton->setFont(labelFont);
    writeButton->setFixedHeight(40);
    connect(writeButton, &QPushButton::clicked, this, &MainWindow::on_writeButton_clicked);

    QVBoxLayout *D3layout = new QVBoxLayout;

    // Создание окна для отображения 3D-графики
    view = new Qt3DExtras::Qt3DWindow();
    container = QWidget::createWindowContainer(view);
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
    container->setMinimumSize(QSize(500, 500));
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout *mainLayout = new QHBoxLayout;

    D3layout->addWidget(container);
    QVBoxLayout *buttonsLayout = new QVBoxLayout;
    buttonsLayout->addWidget(extraParamsButton);
    buttonsLayout->addWidget(saveButton);
    buttonsLayout->addWidget(startButton);
    buttonsLayout->addWidget(stopButton);
    buttonsLayout->addWidget(writeButton);
    menuLayout->addLayout(buttonsLayout);

    QVBoxLayout *testLayout = new QVBoxLayout;
    QPushButton *testButton = new QPushButton("Test Button");
    testLayout->addWidget(testButton);

    QWidget *stendWidget = new QWidget;
    QVBoxLayout *stendLayout = new QVBoxLayout;
    QHBoxLayout *hStendLayout1 = new QHBoxLayout;
    QHBoxLayout *hStendLayout2 = new QHBoxLayout;
    QHBoxLayout *hStendLayoutText = new QHBoxLayout;
    QHBoxLayout *hStendLayout3 = new QHBoxLayout;
    QVBoxLayout *labelsStendLayout1 = new QVBoxLayout;
    QVBoxLayout *boxStendLayout1 = new QVBoxLayout;

    connectButton = new QPushButton("Подключиться");
    connectButton->setFixedHeight(40);
    connectButton->setFont(labelFont);
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::on_connectButton_clicked);

    indicator = new QRadioButton(this);
    indicator->setText(""); // Убираем текст
    indicator->setFixedSize(30, 30); // Устанавливаем размер
    indicator->setStyleSheet(
        "QRadioButton::indicator {"
        "   width: 30px;"
        "   height: 30px;"
        "   border-radius: 15px;" // Круглая форма
        "   background-color: red;"
        "}"
    );

    logsEdit = new QTextEdit();
    hStendLayoutText->addWidget(logsEdit);
    hStendLayout2->addWidget(extraParamsStendButton);
    hStendLayout3->addWidget(indicator);
    hStendLayout3->addSpacing(20);
    hStendLayout3->addWidget(connectButton);

    labePort = new QLabel("Port");
    labelBaudRate = new QLabel("Baud Rate");
    labelDataBits = new QLabel("Data Bits");
    labelParity = new QLabel("Parity");
    labelStopBits = new QLabel("Stop Bits");
    labelFlowControl = new QLabel("Flow Control");
    sendToStmButton = new QPushButton("Отправить на STM");

    for (QLabel* label: {labePort, labelBaudRate, labelDataBits,
         labelParity, labelStopBits, labelFlowControl}){
        label->setFont(labelFont);
        label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        labelsStendLayout1->addWidget(label);
        labelsStendLayout1->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    }
    labelsStendLayout1 -> addWidget(sendToStmButton);
    labelsStendLayout1->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    portComboBox = new QComboBox();

    // Добавление портов от COM1 до COM32
    for (int i = 1; i <= 32; ++i) {
        portComboBox->addItem("COM" + QString::number(i));
    }

    baudRateComboBox = new QComboBox();
    baudRateComboBox->addItem("9600");
    baudRateComboBox->addItem("1200");
    baudRateComboBox->addItem("2400");
    baudRateComboBox->addItem("4800");
    baudRateComboBox->addItem("115200");

    dataBitsComboBox = new QComboBox();
    dataBitsComboBox->addItem("8");
    dataBitsComboBox->addItem("2");
    dataBitsComboBox->addItem("4");
    dataBitsComboBox->addItem("5");
    dataBitsComboBox->addItem("16");
    dataBitsComboBox->addItem("None");

    parityComboBox = new QComboBox();
    parityComboBox->addItem("None");
    parityComboBox->addItem("");

    stopBitsComboBox = new QComboBox();
    stopBitsComboBox->addItem("1");
    stopBitsComboBox->addItem("None");

    flowControlComboBox = new QComboBox();
    flowControlComboBox->addItem("None");
    flowControlComboBox->addItem("");

    sendToStmButtonEdit = new QLineEdit();

    for (QComboBox* box: {portComboBox, baudRateComboBox, dataBitsComboBox,
             parityComboBox, stopBitsComboBox, flowControlComboBox}){
        box->setFont(labelFont);
        box->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        box->setMaximumWidth(300);
        boxStendLayout1->addWidget(box);
        boxStendLayout1->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    }

    boxStendLayout1->addWidget(sendToStmButtonEdit);
    boxStendLayout1->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    hStendLayout1->addLayout(labelsStendLayout1);
    hStendLayout1->addLayout(boxStendLayout1);
    stendLayout->addLayout(hStendLayout1);
    stendLayout->addSpacing(15);
    stendLayout->addLayout(hStendLayoutText);
    stendLayout->addSpacing(15);
    stendLayout->addLayout(hStendLayout2);
    stendLayout->addSpacing(10);
    stendLayout->addLayout(hStendLayout3);
    stendWidget->setLayout(stendLayout);

    QWidget *menuWidget = new QWidget;
    menuWidget->setLayout(menuLayout);

    menuStackedLayout = new QStackedLayout;
    menuStackedLayout->addWidget(menuWidget);
    menuStackedLayout->addWidget(stendWidget);
    menuStackedLayout->setSizeConstraint(QLayout::SetFixedSize);

    mainLayout->addLayout(menuStackedLayout);
    mainLayout->addLayout(D3layout);
    mainLayout->setStretch(0, 0); // Левый лэйаут не растягивается
    mainLayout->setStretch(1, 1); // Правый лэйаут растягивается
    initialValuesLayout->addLayout(mainLayout);
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
    leftBarTransform->setTranslation(QVector3D(-5.5f, -5.0f, 0.0f));
    leftBarEntity->addComponent(leftBarTransform);

    Qt3DCore::QEntity *rightBarEntity = new Qt3DCore::QEntity(mainStructureEntity);
    rightBarEntity->addComponent(sideBarMesh);
    rightBarEntity->addComponent(cartMaterial);  // Используем тот же материал, что и для каретки

    Qt3DCore::QTransform *rightBarTransform = new Qt3DCore::QTransform();
    rightBarTransform->setTranslation(QVector3D(5.5f, -5.0f, 0.0f));
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
    weightTransform->setTranslation(QVector3D(0.0f, -2.5f, 0.0f));
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

    cartTransform = new Qt3DCore::QTransform();
    cartTransform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));
    cartEntity->addComponent(cartTransform);

    QVector<double> values = MainWindow::readIni(localPath + "/values.ini", "Base");
    for (int i = 0; i < fields.size(); i++) {
            fields[i]->setText(QString::number(values[i]));
            fields[i]->setFont(labelFont);
    }

    setCentralWidget(initialValuesWidget);

    secondWindow = new SecondWindow();
    //Получение указателей на графики из SecondWindow
    plot1 = secondWindow->getPlot1();
    plot2 = secondWindow->getPlot2();
    plot3 = secondWindow->getPlot3();
    plot4 = secondWindow->getPlot4();

    plotTSize = 10;
    std::pair<int, int>* plotSizes[] = {&plotXSize, &plotVxSize, &plotFiSize, &plotOmegaFiSize};

    for (auto* plotSize : plotSizes) {
        plotSize->first = -1;
        plotSize->second = 1;
    }

    for (QCustomPlot* plot : {plot1, plot2, plot3, plot4}) {
        plot->xAxis->setRange(-1, plotTSize);
        plot->yAxis->setRange(-1,1);
    }

    secondWindow->setWindowTitle("2D моделирование");
    secondWindow->resize(800, 600);
    secondWindow->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

Eigen::MatrixXd matrixPower(const Eigen::MatrixXd& mat, int power) {
    if (power == 0) {
        return Eigen::MatrixXd::Identity(mat.rows(), mat.cols());
    } else if (power == 1) {
        return mat;
    } else {
        Eigen::MatrixXd temp = matrixPower(mat, power / 2);
        if (power % 2 == 0) {
            return temp * temp;
        } else {
            return temp * temp * mat;
        }
    }
}

Eigen::MatrixXd createColsMatrix(const Eigen::MatrixXd& A, const Eigen::Vector4d& C) {
    Eigen::MatrixXd cols(4, 4);
    cols.col(0) = C;
    cols.col(1) = A.transpose() * C;
    cols.col(2) = matrixPower(A.transpose(), 2) * C;
    cols.col(3) = matrixPower(A.transpose(), 3) * C;
    return cols;
}

const double l_max = 0.3;
bool isFirstReadFlag = true;
bool isConnected = false;

QVector<double> values = MainWindow::readIni("C:/Users/baben/programming/bmstu/Ticker/real_model_version/app", "Base");
QVector<double> constants = {values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7], values[8]};
QVector<double> val = {values[9], values[10], values[11], values[12]};

const double m1 = constants[0];
const double m2 = constants[1];
const double m3 = constants[2];
const double l = constants[3];
const double Ff = constants[4];
const double Fr = constants[5];
const double Fc = constants[6];
const double b2 = constants[7];
const double k2 = constants[8];

QVector<double> result = MainWindow::rungeKutta(0, 1, 100, val, constants);

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
        modifiIni(localPath + "/values.ini", newValues);
}

void MainWindow::on_startButton_clicked(){
    if (isFirstReadFlag){
        modifiIni(localPath + "/values.ini", values);
        isFirstReadFlag = false;
    }
    values = MainWindow::readIni(localPath + "/values.ini", "Modified");
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

    std::pair<int, int>* plotSizes[] = {&plotXSize, &plotVxSize, &plotFiSize, &plotOmegaFiSize};

    for (auto* plotSize : plotSizes) {
        plotSize->first = -1;
        plotSize->second = 1;
    }
    plotTSize = 10;

    timer->start(100);
}

void MainWindow::onSimulationClicked(){
    menuStackedLayout->setCurrentIndex(0);
}

void MainWindow::onStendClicked(){
    menuStackedLayout->setCurrentIndex(1);
    isConnected = false;
    timer->stop();
}

void MainWindow::on_stopButton_clicked(){
    timer->stop();
}

void MainWindow::on_extraParamsButton_clicked(){
    ExtraParamsDialog dialog(this);
    dialog.exec(); // Показывает диалоговое окно
}

void MainWindow::on_connectButton_clicked(){
    // Считывание выбранного порта
    QString selectedPort = portComboBox->currentText();
    serial->setPortName(selectedPort);

    // Считывание выбранной скорости передачи данных
    int selectedBaudRate = baudRateComboBox->currentText().toInt();
    serial->setBaudRate(static_cast<QSerialPort::BaudRate>(selectedBaudRate));

    // Считывание количества бит данных
    int selectedDataBits = dataBitsComboBox->currentText().toInt();
    serial->setDataBits(static_cast<QSerialPort::DataBits>(selectedDataBits));

    // Считывание четности
    QString selectedParity = parityComboBox->currentText();
    if (selectedParity == "None") {
        serial->setParity(QSerialPort::NoParity);
    } else {}

    // Считывание стоп-битов
    QString selectedStopBits = stopBitsComboBox->currentText();
    if (selectedStopBits == "1") {
        serial->setStopBits(QSerialPort::OneStop);
    } else {}

    // Считывание управления потоком
    QString selectedFlowControl = flowControlComboBox->currentText();
    if (selectedFlowControl == "None") {
        serial->setFlowControl(QSerialPort::NoFlowControl);
    } else {}

    if (isConnected){
        timerStend->stop();
        isConnected = false;
        connectButton->setText("Подключиться");
        indicator->setStyleSheet(
            "QRadioButton::indicator {"
            "   width: 30px;"
            "   height: 30px;"
            "   border-radius: 15px;" // Круглая форма
            "   background-color: red;"
            "}"
        );
    }
    else{
        if (serial->open(QIODevice::ReadWrite)) {
            logsEdit->append("Serial port opened successfully!");
            if (serial->waitForReadyRead(3000)) {  // Ожидание данных 3 секунды
                QByteArray data = serial->readAll();
            }

            serial->close();

            indicator->setStyleSheet(
                "QRadioButton::indicator {"
                "   width: 30px;"
                "   height: 30px;"
                "   border-radius: 15px;" // Круглая форма
                "   background-color: green;"
                "}"
            );

            T = beginT;
            plotTime.clear();
            plotXY.clear();
            plotVxY.clear();
            plotFiY.clear();
            plotOmegaFiY.clear();

            for (QCustomPlot* plot : {plot1, plot2, plot3, plot4}) {
                plot->clearGraphs();
            }

            std::pair<int, int>* plotSizes[] = {&plotXSize, &plotVxSize, &plotFiSize, &plotOmegaFiSize};

            for (auto* plotSize : plotSizes) {
                plotSize->first = -1;
                plotSize->second = 1;
            }
            plotTSize = 10;
            isConnected = true;
            connectButton->setText("Отключиться");

            if (isFirstReadFlag){
                modifiIni(localPath + "/values.ini", values);
                isFirstReadFlag = false;
            }
            values = MainWindow::readIni(localPath + "/values.ini", "Modified");
            constants = {values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7], values[8]};
            val = {values[9], values[10], values[11], values[12]};
            result = MainWindow::rungeKutta(0, 1, 100, val, constants);

            plotXYModel.clear();
            plotVxYModel.clear();
            plotFiYModel.clear();
            plotOmegaFiYModel.clear();

            timerStend->start(100);

        } else {
            logsEdit->append("Failed to open serial port: " + serial->errorString());
        }

    }
}

void MainWindow::on_writeButton_clicked(){
    writeToOutput(localPath + "/output.txt",
                  plotTime, plotXY, plotVxY, plotFiY, plotOmegaFiY);
}

void MainWindow::timerStend_slot(){

    result = MainWindow::rungeKutta(0, 1, 100, result, constants);

    if (result[0] > l_max || result[0] < -l_max)
        result[1] = 0;

    float angle = -MainWindow::to_degrees(result[2]);
    QMatrix4x4 matrix;
    matrix.rotate(angle, QVector3D(0.0f, 0.0f, 1.0f));
    matrix.translate(QVector3D(0.0f, -2.5f, 0.0f)); // Позиционирование стержня относительно каретки

    QMatrix4x4 matrix2;
    matrix2.rotate(0, QVector3D(0.0f, 0.0f, 1.0f));
    matrix2.translate(QVector3D(result[0] * 100 / 3, 0.0f, 0.0f)); // Позиционирование стержня относительно каретки

    cartTransform->setMatrix(matrix2);
    rodTransform->setMatrix(matrix);

    plotXYModel.push_back(result[0]);
    plotVxYModel.push_back(result[1]);
    plotFiYModel.push_back(MainWindow::to_degrees(result[2]));
    plotOmegaFiYModel.push_back(MainWindow::to_degrees(result[3]));

    if (serial->open(QIODevice::ReadWrite)) {
        if (serial->waitForReadyRead(50)) {  // Ожидание данных 1 секунды
            QByteArray data = serial->readAll();
            QVector<double> valuesSerial = parsePacket(data);

            plotTime.push_back(T);
            plotXY.push_back(valuesSerial[0]);
            plotVxY.push_back(valuesSerial[1]);
            plotFiY.push_back(MainWindow::to_degrees(valuesSerial[2]));
            plotOmegaFiY.push_back(MainWindow::to_degrees(valuesSerial[3]));

            //Убирает задержку в работе
            for (QCustomPlot* plot : {plot1, plot2, plot3, plot4}) {
                plot->clearGraphs();
            }

            if (T > plotTSize * 0.9){
                plotTSize = plotTSize * 1.5;
                for (QCustomPlot* plot : {plot1, plot2, plot3, plot4}) {
                    plot->xAxis->setRange(-1, plotTSize);
                }
            }

            plotXSize = MainWindow::calculatePlotScale(plotXSize, plotXY.last());
            plotVxSize = MainWindow::calculatePlotScale(plotVxSize, plotVxY.last());
            plotFiSize = MainWindow::calculatePlotScale(plotFiSize, plotFiY.last());
            plotOmegaFiSize = MainWindow::calculatePlotScale(plotOmegaFiSize, plotOmegaFiY.last());

            T += 0.1;

            plot1->addGraph();
            plot1->addGraph();
            plot1->graph(0)->setPen(QPen(QColor(0, 0, 255), penSize));
            plot1->graph(0)->addData(plotTime, plotXY);
            plot1->graph(1)->setPen(QPen(QColor(255, 165, 0), penSize));
            plot1->graph(1)->addData(plotTime, plotXYModel);
            plot1->yAxis->setRange(plotXSize.first, plotXSize.second);
            plot1->replot();

            plot2->addGraph();
            plot2->addGraph();
            plot2->graph(0)->setPen(QPen(QColor(0, 0, 255), penSize));
            plot2->graph(0)->addData(plotTime, plotVxY);
            plot2->graph(1)->setPen(QPen(QColor(255, 165, 0), penSize));
            plot2->graph(1)->addData(plotTime, plotVxYModel);
            plot2->yAxis->setRange(plotVxSize.first, plotVxSize.second);
            plot2->replot();

            plot3->addGraph();
            plot3->addGraph();
            plot3->graph(0)->setPen(QPen(QColor(0, 0, 255), penSize));
            plot3->graph(0)->addData(plotTime, plotFiY);
            plot3->graph(1)->setPen(QPen(QColor(255, 165, 0), penSize));
            plot3->graph(1)->addData(plotTime, plotFiYModel);
            plot3->yAxis->setRange(plotFiSize.first, plotFiSize.second);
            plot3->replot();

            plot4->addGraph();
            plot4->addGraph();
            plot4->graph(0)->setPen(QPen(QColor(0, 0, 255), penSize));
            plot4->graph(0)->addData(plotTime, plotOmegaFiY);
            plot4->graph(1)->setPen(QPen(QColor(255, 165, 0), penSize));
            plot4->graph(1)->addData(plotTime, plotOmegaFiYModel);
            plot4->yAxis->setRange(plotOmegaFiSize.first, plotOmegaFiSize.second);
            plot4->replot();
        } else {
            logsEdit->append("No data received within the timeout.");
        }
        serial->close();
    } else {
        logsEdit->append("Failed to open serial port: " + serial->errorString());

        indicator->setStyleSheet(
            "QRadioButton::indicator {"
            "   width: 30px;"
            "   height: 30px;"
            "   border-radius: 15px;" // Круглая форма
            "   background-color: red;"
            "}"
        );
        timerStend->stop();
    }

}

void MainWindow::timer_slot(){
    result = MainWindow::rungeKutta(0, 1, 100, result, constants);

    if (result[0] > l_max || result[0] < -l_max)
        result[1] = 0;

    float angle = -MainWindow::to_degrees(result[2]);
    QMatrix4x4 matrix;
    matrix.rotate(angle, QVector3D(0.0f, 0.0f, 1.0f));
    matrix.translate(QVector3D(0.0f, -2.5f, 0.0f)); // Позиционирование стержня относительно каретки

    QMatrix4x4 matrix2;
    matrix2.rotate(0, QVector3D(0.0f, 0.0f, 1.0f));
    matrix2.translate(QVector3D(result[0] * 100 / 3, 0.0f, 0.0f)); // Позиционирование стержня относительно каретки

    cartTransform->setMatrix(matrix2);
    rodTransform->setMatrix(matrix);

    plotTime.push_back(T);
    plotXY.push_back(result[0]);
    plotVxY.push_back(result[1]);
    plotFiY.push_back(MainWindow::to_degrees(result[2]));
    plotOmegaFiY.push_back(MainWindow::to_degrees(result[3]));

    //Убирает задержку в работе
    for (QCustomPlot* plot : {plot1, plot2, plot3, plot4}) {
        plot->clearGraphs();
    }

    if (T > plotTSize * 0.9){
        plotTSize = plotTSize * 1.5;
        for (QCustomPlot* plot : {plot1, plot2, plot3, plot4}) {
            plot->xAxis->setRange(-1, plotTSize);
        }
    }

    plotXSize = MainWindow::calculatePlotScale(plotXSize, plotXY.last());
    plotVxSize = MainWindow::calculatePlotScale(plotVxSize, plotVxY.last());
    plotFiSize = MainWindow::calculatePlotScale(plotFiSize, plotFiY.last());
    plotOmegaFiSize = MainWindow::calculatePlotScale(plotOmegaFiSize, plotOmegaFiY.last());

    T += stepT;

    plot1->addGraph();
    plot1->graph(0)->setPen(QPen(QColor(0, 0, 255), penSize));
    plot1->graph(0)->addData(plotTime, plotXY);
    plot1->yAxis->setRange(plotXSize.first, plotXSize.second);
    plot1->replot();

    plot2->addGraph();
    plot2->graph(0)->setPen(QPen(QColor(0, 0, 255), penSize));
    plot2->graph(0)->addData(plotTime, plotVxY);
    plot2->yAxis->setRange(plotVxSize.first, plotVxSize.second);
    plot2->replot();

    plot3->addGraph();
    plot3->graph(0)->setPen(QPen(QColor(0, 0, 255), penSize));
    plot3->graph(0)->addData(plotTime, plotFiY);
    plot3->yAxis->setRange(plotFiSize.first, plotFiSize.second);
    plot3->replot();

    plot4->addGraph();
    plot4->graph(0)->setPen(QPen(QColor(0, 0, 255), penSize));
    plot4->graph(0)->addData(plotTime, plotOmegaFiY);
    plot4->yAxis->setRange(plotOmegaFiSize.first, plotOmegaFiSize.second);
    plot4->replot();
}

