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

    QFont labelFont("Times", 15);
    QFont axisFont("Arial", 20);
    penSize = 5;

    localPath = "C:/Users/baben_bakg1j1/Programming/C++/Ticker/app";

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

    QHBoxLayout *controlНLayout = new QHBoxLayout;
    QVBoxLayout *controlLVLayout = new QVBoxLayout;
    QVBoxLayout *controlEVLayout = new QVBoxLayout;
    QHBoxLayout *controlMenuLayout = new QHBoxLayout;
    QVBoxLayout *controLayout = new QVBoxLayout;

    labelControlType = new QLabel("Вид");
    labelControlType->setFont(labelFont);
    labelControlParam1 = new QLabel("Параметр 1");
    labelControlParam1->setFont(labelFont);
    labelControlParam2 = new QLabel("Параметр 2");
    labelControlParam2->setFont(labelFont);

    controlMenuLayout->addWidget(labelControlType);

    controlLVLayout->addWidget(labelControlParam1);
    controlLVLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    controlLVLayout->addWidget(labelControlParam2);

    QComboBox *controlComboBox = new QComboBox();
    controlComboBox->setFont(labelFont);  // Устанавливаем шрифт
    controlComboBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    controlComboBox->setMaximumWidth(300); // Максимальная ширина в пикселях

    // Добавление элементов в QListWidget
    controlComboBox->addItem("Sliding-mode");
    controlComboBox->addItem("Back stepping");
    controlComboBox->addItem("Адаптивное управление");
    controlComboBox->addItem("Model Predictive Control");

    editControlParam1 = new QLineEdit;
    editControlParam1->setFont(labelFont);
    editControlParam1->setMaximumWidth(120);
    editControlParam1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    editControlParam2 = new QLineEdit;
    editControlParam2->setMaximumWidth(120);
    editControlParam2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    editControlParam2->setFont(labelFont);

    controlMenuLayout->addWidget(controlComboBox);
    controlMenuLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    controlEVLayout->addWidget(editControlParam1);
    controlEVLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    controlEVLayout->addWidget(editControlParam2);

    controlНLayout->addLayout(controlLVLayout);
    controlНLayout->addLayout(controlEVLayout);

    controLayout->addLayout(controlMenuLayout);
    controLayout->addLayout(controlНLayout);

    QHBoxLayout *observerНLayout = new QHBoxLayout;
    QVBoxLayout *observerLVLayout = new QVBoxLayout;
    QVBoxLayout *observerEVLayout = new QVBoxLayout;
    QHBoxLayout *observerMenuLayout = new QHBoxLayout;
    QVBoxLayout *observerLayout = new QVBoxLayout;

    labelObserverType = new QLabel("Вид");
    labelObserverType->setFont(labelFont);
    labelObserverParam1 = new QLabel("Параметр 1");
    labelObserverParam1->setFont(labelFont);
    labelObserverParam2 = new QLabel("Параметр 2");
    labelObserverParam2->setFont(labelFont);

    observerMenuLayout->addWidget(labelObserverType);

    observerLVLayout->addWidget(labelObserverParam1);
    observerLVLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    observerLVLayout->addWidget(labelObserverParam2);

    observerComboBox = new QComboBox();
    observerComboBox->setFont(labelFont);  // Устанавливаем шрифт
    observerComboBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    observerComboBox->setMaximumWidth(300); // Максимальная ширина в пикселях

    // Добавление элементов в QListWidget
    observerComboBox->addItem("Без наблюдателя");
    observerComboBox->addItem("Наблюдатель с большим коэффициентом усиления");
    observerComboBox->addItem("Наблюдатель Калмана");
    observerComboBox->addItem("Адаптивный наблюдатель");
    observerComboBox->addItem("Нелинейный наблюдатель");

    editObserverParam1 = new QLineEdit;
    editObserverParam1->setFont(labelFont);
    editObserverParam1->setMaximumWidth(120);
    editObserverParam1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    editObserverParam2 = new QLineEdit;
    editObserverParam2->setMaximumWidth(120);
    editObserverParam2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    editObserverParam2->setFont(labelFont);

    observerMenuLayout->addWidget(observerComboBox);
    observerMenuLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    observerEVLayout->addWidget(editObserverParam1);
    observerEVLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    observerEVLayout->addWidget(editObserverParam2);

    observerНLayout->addLayout(observerLVLayout);
    observerНLayout->addLayout(observerEVLayout);
    observerLayout->addLayout(observerMenuLayout);
    observerLayout->addLayout(observerНLayout);

    QGroupBox *modelParamsGroup = new QGroupBox("Параметры модели");
    modelParamsGroup->setLayout(LEVLayout1);
    QGroupBox *initValGroup = new QGroupBox("Начальные условия");
    initValGroup->setLayout(LEVLayout2);
    QGroupBox *controlGroup = new QGroupBox("Управление");
    controlGroup->setLayout(controLayout);
    QGroupBox *observerGroup = new QGroupBox("Наблюдатель");
    observerGroup->setLayout(observerLayout);

    menuLayout->addWidget(modelParamsGroup);
    menuLayout->addWidget(initValGroup);
    menuLayout->addWidget(controlGroup);
    menuLayout->addWidget(observerGroup);

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
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(saveButton);
    buttonsLayout->addWidget(startButton);
    buttonsLayout->addWidget(stopButton);
    buttonsLayout->addWidget(writeButton);
    mainLayout->addLayout(menuLayout);
    mainLayout->addLayout(D3layout);
    initialValuesLayout->addLayout(mainLayout);
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

    secondWindow->setWindowTitle("2D моделирование");
    secondWindow->resize(800, 600);
    secondWindow->show();

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

QVector<double> y_obs = {0.0, 0.0, M_PI / 6, 0.0};

const double m1 = constants[0];
const double m2 = constants[1];
const double m3 = constants[2];
const double l = constants[3];
const double Ff = constants[4];
const double Fr = constants[5];
const double Fc = constants[6];
const double b2 = constants[7];
const double k2 = constants[8];

/*
Eigen::Vector4d y_0(0.0, 0.0, M_PI / 6, 0.0);

Eigen::MatrixXd A({
    {0, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, ((values[1]+(values[2])/2)*9.81*cos(y_0[2])-values[7])/((values[1]+(values[2])/4)*(values[3])), -values[8]/((values[1]+(values[2])/4)*values[3]*values[3])}
});

Eigen::Vector4d C(1, 1, 1, 1);

Eigen::MatrixXd invA = (C * A.transpose() * C.transpose()).inverse();
Eigen::MatrixXd invC = C.inverse();
Eigen::MatrixXd H = (A.transpose().pow(4) + 2.6*8*(A.transpose().pow(3)) + 3.4*64*(A.transpose().pow(2)) + 2.6*512*A.transpose() + 4096*Eigen::MatrixXd::Identity(4,4)) * invA * invC;

Eigen::Vector4d y_0(0.0, 0.0, M_PI / 6, 0.0);

Eigen::Matrix4d obs({
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, M_PI/6, 0}
});
*/

QVector<double> result = MainWindow::rungeKutta(0, 1, 100, val, constants);
QVector<double> resultObs = MainWindow::rungeKuttaObserver(0, 1, 100, y_obs, constants);

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

    QString selectedItem = observerComboBox->currentText();
    if (selectedItem == "Без наблюдателя") {
        observerFlag = false;
    } else if (selectedItem == "Наблюдатель с большим коэффициентом усиления") {
        observerFlag = true;
    }
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
    if (!observerFlag){
        y_obs = {0.0, 0.0, M_PI / 6, 0.0};
        resultObs = MainWindow::rungeKuttaObserver(0, 1, 100, y_obs, constants);
    }

    T = beginT;
    plotTime.clear();
    plotXY.clear();
    plotVxY.clear();
    plotFiY.clear();
    plotOmegaFiY.clear();
    plotObsXY.clear();
    plotObsVxY.clear();
    plotObsFiY.clear();
    plotObsOmegaFiY.clear();

    for (QCustomPlot* plot : {plot1, plot2, plot3, plot4}) {
        plot->clearGraphs();
    }

    timer->start(100);
}

void MainWindow::on_stopButton_clicked(){
    timer->stop();
}

void MainWindow::on_writeButton_clicked(){
    writeToOutput(localPath + "/output.txt",
                  plotTime, plotXY, plotVxY, plotFiY, plotOmegaFiY);
}

void MainWindow::timer_slot(){
    result = MainWindow::rungeKutta(0, 1, 100, result, constants);
    if (observerFlag){
        resultObs = MainWindow::rungeKuttaObserver(0, 1, 100, y_obs, constants);
        plotObsXY.push_back(resultObs[0]);
        plotObsVxY.push_back(resultObs[1]);
        plotObsFiY.push_back(resultObs[2]);
        plotObsOmegaFiY.push_back(resultObs[3]);
        y_obs = resultObs;
    }

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

    plotTime.push_back(T);
    plotXY.push_back(result[0]);
    plotVxY.push_back(result[1]);
    plotFiY.push_back(MainWindow::to_degrees(result[2]));
    plotOmegaFiY.push_back(MainWindow::to_degrees(result[3]));

    T += stepT;

    plot1->addGraph();
    plot1->addGraph();
    plot1->graph(0)->setPen(QPen(QColor(0, 0, 255), penSize));
    plot1->graph(0)->addData(plotTime, plotXY);
    plot1->graph(1)->setPen(QPen(QColor(255, 165, 0), penSize));
    plot1->graph(1)->addData(plotTime, plotObsXY);
    plot1->replot();

    plot2->addGraph();
    plot2->addGraph();
    plot2->graph(0)->setPen(QPen(QColor(0, 0, 255), penSize));
    plot2->graph(0)->addData(plotTime, plotVxY);
    plot2->graph(1)->setPen(QPen(QColor(255, 165, 0), penSize));
    plot2->graph(1)->addData(plotTime, plotObsVxY);
    plot2->replot();

    plot3->addGraph();
    plot3->addGraph();
    plot3->graph(0)->setPen(QPen(QColor(0, 0, 255), penSize));
    plot3->graph(0)->addData(plotTime, plotFiY);
    plot3->graph(1)->setPen(QPen(QColor(255, 165, 0), penSize));
    plot3->graph(1)->addData(plotTime, plotObsFiY);
    plot3->replot();

    plot4->addGraph();
    plot4->addGraph();
    plot4->graph(0)->setPen(QPen(QColor(0, 0, 255), penSize));
    plot4->graph(0)->addData(plotTime, plotOmegaFiY);
    plot4->graph(1)->setPen(QPen(QColor(255, 165, 0), penSize));
    plot4->graph(1)->addData(plotTime, plotObsOmegaFiY);
    plot4->replot();
}

