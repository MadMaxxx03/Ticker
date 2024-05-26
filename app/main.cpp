#include "mainwindow.h"
using namespace std;

//Считывание начальных значений
QVector<double> MainWindow::readIni(QString path, QString category){
    QSettings sett(path, QSettings::IniFormat);
    QVector<double> values;
    values << sett.value(category + "/m1", 0.0).toDouble()
           << sett.value(category + "/m2", 0.0).toDouble()
           << sett.value(category + "/m3", 0.0).toDouble()
           << sett.value(category + "/l", 0.0).toDouble()
           << sett.value(category + "/Ff", 0.0).toDouble()
           << sett.value(category + "/Fr", 0.0).toDouble()
           << sett.value(category + "/Fc", 0.0).toDouble()
           << sett.value(category + "/b2", 0.0).toDouble()
           << sett.value(category + "/k2", 0.0).toDouble()
           << sett.value(category + "/x", 0.0).toDouble()
           << sett.value(category + "/V", 0.0).toDouble()
           << sett.value(category + "/fi", 0.0).toDouble()
           << sett.value(category + "/W", 0.0).toDouble();
    return values;
}

//Изменение начальных значений
void MainWindow::modifiIni(QString path, QVector<double> values) {
    QSettings sett(path, QSettings::IniFormat);
    sett.setValue("Modified/m1", values[0]);
    sett.setValue("Modified/m2", values[1]);
    sett.setValue("Modified/m3", values[2]);
    sett.setValue("Modified/l", values[3]);
    sett.setValue("Modified/Ff", values[4]);
    sett.setValue("Modified/Fr", values[5]);
    sett.setValue("Modified/Fc", values[6]);
    sett.setValue("Modified/b2", values[7]);
    sett.setValue("Modified/k2", values[8]);
    sett.setValue("Modified/x", values[9]);
    sett.setValue("Modified/V", values[10]);
    sett.setValue("Modified/fi", values[11]);
    sett.setValue("Modified/W", values[12]);
}

//Функция суммирования векторов
QVector<double> MainWindow::sum_vector(const QVector<double>& vec1, const QVector<double>& vec2) {
    QVector<double> result(vec1.size());

    transform(vec1.begin(), vec1.end(), vec2.begin(), result.begin(), [](double num1, double num2) {
        return num1 + num2;
        });

    return result;
}

//Функция перемножения векторов
QVector<double> MainWindow::mult_vector(const QVector<double>& vec1, double x) {
    QVector<double> result(vec1.size());

    transform(vec1.begin(), vec1.end(), result.begin(), [x](double num) {
        return num * x;
        });

    return result;
}

//Функция перевода радиан в градусы
double MainWindow::to_degrees(double radians) {
    return radians / (M_PI / 180.0);
}

//Мат модель с наблюдателем
QVector<double> MainWindow::mathModelObserver(double t, const QVector<double>& model, const QVector<double>& constants){

    const double m1 = constants[0];
    const double m2 = constants[1];
    const double m3 = constants[2];
    const double l = constants[3];
    const double Ff = constants[4];
    const double Fr = constants[5];
    const double Fc = constants[6];
    const double b2 = constants[7];
    const double k2 = constants[8];

    const double x = model[0];
    const double V = model[1];
    const double fi = model[2];
    const double W = model[3];

    double M1 = (m1 + m2 + m3);
    double M2 = (m2 + m3 / 2);
    double M3 = (m2 + m3 / 3);

    double x_dt = V;
    double V_dt = (Fc*M3*l*l-M2*M3*l*l*l*W*W*sin(fi)+M2*M2*l*l*9.81*cos(fi)*sin(fi)-M2*l*cos(fi)*(b2*W+k2*fi) )/(M1*M3*l*l-M2*M2*l*l*cos(fi)*cos(fi));
    double fi_dt = W;
    double W_dt = (M1*M2*9.81*l*sin(fi)-M1*(b2*W+k2*fi)+Fc*M2*l*cos(fi)-M2*M2*l*l*W*W*sin(fi)*cos(fi))/(M1*M3*l*l-M2*M2*l*l*cos(fi)*cos(fi));

    return {x_dt, V_dt, fi_dt, W_dt};
}

//Мат модель
QVector<double> MainWindow::mathModel(double t, const QVector<double>& model, const QVector<double>& constants){

    const double m1 = constants[0];
    const double m2 = constants[1];
    const double m3 = constants[2];
    const double l = constants[3];
    const double Ff = constants[4];
    const double Fr = constants[5];
    const double Fc = constants[6];
    const double b2 = constants[7];
    const double k2 = constants[8];

    const double x = model[0];
    const double V = model[1];
    const double fi = model[2];
    const double W = model[3];

    double M1 = (m1 + m2 + m3);
    double M2 = (m2 + m3 / 2);
    double M3 = (m2 + m3 / 3);

    double x_dt = V;
    double V_dt = (Fc*M3*l*l-M2*M3*l*l*l*W*W*sin(fi)+M2*M2*l*l*9.81*cos(fi)*sin(fi)-M2*l*cos(fi)*(b2*W+k2*fi) )/(M1*M3*l*l-M2*M2*l*l*cos(fi)*cos(fi));
    double fi_dt = W;
    double W_dt = (M1*M2*9.81*l*sin(fi)-M1*(b2*W+k2*fi)+Fc*M2*l*cos(fi)-M2*M2*l*l*W*W*sin(fi)*cos(fi))/(M1*M3*l*l-M2*M2*l*l*cos(fi)*cos(fi));

    return {x_dt, V_dt, fi_dt, W_dt};
}

//Рассчет управления
QVector<double> MainWindow::control(double dt, const QVector<double>& model, const QVector<double>& u){

    return {};
}

//РК4
QVector<double> MainWindow::rungeKutta(double t0, double tf, int steps, const QVector<double>& y0, const QVector<double>& constants) {
    double dt = (tf - t0) / steps / 10;
    double t = t0;
    QVector<double> y = y0;
    QVector<double> k1;
    QVector<double> k2;
    QVector<double> k3;
    QVector<double> k4;

    for (int i = 0; i < steps; ++i) {
        t += dt;
        k1 = MainWindow::mathModel(t, y, constants);
        k2 = MainWindow::mathModel(t + dt / 2, MainWindow::sum_vector(y, MainWindow::mult_vector(k1, dt / 2)), constants);
        k3 = MainWindow::mathModel(t + dt / 2, MainWindow::sum_vector(y, MainWindow::mult_vector(k2, dt / 2)), constants);
        k4 = MainWindow::mathModel(t + dt, MainWindow::sum_vector(y, MainWindow::mult_vector(k3, dt)), constants);
        y = MainWindow::sum_vector(MainWindow::mult_vector(MainWindow::sum_vector(MainWindow::sum_vector(MainWindow::sum_vector(MainWindow::mult_vector(k2, 2), MainWindow::mult_vector(k3, 2)), k1), k4), (dt / 6)), y);
    }

    return y;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
