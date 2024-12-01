#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "secondwindow.h"
#include <QLabel>
#include <QDebug>
#include <QLineEdit>
#include <QStackedWidget>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVector>
#include <QTimer>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <QSettings>
#include <QApplication>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DRender/QCamera>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QOrbitCameraController>
#include <QPropertyAnimation>
#include <QMatrix4x4>
#include <Qt3DRender/QDirectionalLight>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <Eigen/Dense>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_saveButton_clicked();
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_writeButton_clicked();
    void timer_slot();

public:
    static QVector<double> readIni(QString path, QString category);
    static void modifiIni(QString path, QVector<double>);
    static QVector<double> sum_vector(const QVector<double>& vec1, const QVector<double>& vec2);
    static QVector<double> mult_vector(const QVector<double>& vec1, double x);
    static QVector<double> rungeKutta(double t0, double tf, int steps, const QVector<double>& y0, const QVector<double>& constants);
    static QVector<double> rungeKuttaObserver(double t0, double tf, int steps, QVector<double> y_obs, const QVector<double>& constants, Eigen::MatrixXd H);
    static QVector<double> mathModel(double t, const QVector<double>& model, const QVector<double>& constants);
    static QVector<double> mathModelObserver(double t, QVector<double> y_obs, const QVector<double>& constants, Eigen::MatrixXd H);
    static QVector<double> control(double dt, const QVector<double>& model, const QVector<double>& u);
    static void writeToOutput(QString path, const QVector<double>& v1, const QVector<double>& v2, const QVector<double>& v3, const QVector<double>& v4, const QVector<double>& v5);
    static double to_degrees(double radians);

private:
    Ui::MainWindow *ui;
    SecondWindow *secondWindow;

    QTimer *timer;
    double beginT, stepT, T;
    int penSize;

    bool isFirstReadFlag, observerFlag;

    Qt3DCore::QEntity *rootEntity;
    Qt3DExtras::Qt3DWindow *view;
    QWidget *container;
    Qt3DCore::QTransform *cartTransform;
    Qt3DCore::QTransform *rodTransform;
    QPropertyAnimation *cartAnimation;

    QAction *D2ModelingAction;
    QAction *D3ModelingAction;
    QAction *initValAction;

    QComboBox *observerComboBox;

    QStackedWidget *stackedWidget;
    QWidget *D3Widget;
    QWidget *centralWidget;
    QWidget *initialValuesWidget;

    QLabel * labeModelParams;
    QLabel * labelm1;
    QLabel * labelm2;
    QLabel * labelm3;
    QLabel * labell;
    QLabel * labelFf;
    QLabel * labelFr;
    QLabel * labelFc;
    QLabel * labelb2;
    QLabel * labelk2;

    QLabel * labeInitVal;
    QLabel * labelX;
    QLabel * labelV;
    QLabel * labelFi;
    QLabel * labelW;

    QLineEdit * editm1;
    QLineEdit * editm2;
    QLineEdit * editm3;
    QLineEdit * editl;
    QLineEdit * editFf;
    QLineEdit * editFr;
    QLineEdit * editFc;
    QLineEdit * editb2;
    QLineEdit * editk2;

    QLineEdit * editX;
    QLineEdit * editV;
    QLineEdit * editFi;
    QLineEdit * editW;

    QList<QLineEdit*> fields;

    QLabel * labelControl;
    QLabel * labelControlType;
    QLabel * labelControlParam1;
    QLabel * labelControlParam2;

    QLineEdit * editControlParam1;
    QLineEdit * editControlParam2;

    QLabel * labelObserver;
    QLabel * labelObserverType;
    QLabel * labelObserverParam1;
    QLabel * labelObserverParam2;

    QLineEdit * editObserverParam1;
    QLineEdit * editObserverParam2;

    QCustomPlot *plot1;
    QCustomPlot *plot2;
    QCustomPlot *plot3;
    QCustomPlot *plot4;

    QVector<double> plotTime, plotXY, plotVxY, plotFiY, plotOmegaFiY;
    QVector<double> plotObsXY, plotObsVxY, plotObsFiY, plotObsOmegaFiY;

    QPushButton *saveButton;
    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *writeButton;

    QString localPath;

};
#endif // MAINWINDOW_H
