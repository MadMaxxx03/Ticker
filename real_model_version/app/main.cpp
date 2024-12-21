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

void MainWindow::writeToOutput(QString path, const QVector<double>& v1, const QVector<double>& v2, const QVector<double>& v3, const QVector<double>& v4, const QVector<double>& v5){
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for writing: " << file.errorString();
        return;
    }

    QTextStream out(&file);
    int length = v1.size();

    out << "Time   X   Vx   Fi   OmegaFiY\n";

    for (int i = 0; i < length; ++i) {
        out << v1[i] << "  "
            << v2[i] << "  "
            << v3[i] << "  "
            << v4[i] << "  "
            << v5[i] << "\n";
    }

    file.close();
    qDebug() << "Data written to file successfully.";
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

//Мат модель
QVector<double> MainWindow::mathModel(double t, const QVector<double>& model, const QVector<double>& constants){

    const double m1 = constants[0];
    const double m2 = constants[1];
    const double m3 = constants[2];
    const double l = constants[3];
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

// Функция расчета масштаба графиков
pair<int, int> MainWindow::calculatePlotScale(const pair<int, int> scale, double value){
    pair<int, int> newScale = scale;
    if (value > scale.second * 0.9){
        newScale.second *= 1.5;
        if (value > newScale.second){
            newScale.second = (value + 5);
        }
    }
    if (value < scale.first * 0.9){
        newScale.first *= 1.5;
        if (value < newScale.first){
            newScale.first = (value - 5);
        }
    }
    return newScale;
}

QVector<double> MainWindow::parsePacket(const QByteArray &packet) {
    QVector<double> values(4, 0.0); // Массив для хранения результатов

    // Найти строку между символами $ и /
    int start = packet.indexOf('$');
    int end = packet.indexOf('/', start);
    if (start == -1 || end == -1 || start >= end) {
        qDebug() << "Invalid packet format!";
        return values; // Вернуть пустые значения, если формат неверен
    }

    QByteArray rawData = packet.mid(start + 1, end - start - 1);
    //qDebug() << "Extracted data:" << rawData; // Выводим извлеченную строку

    // Убедиться, что длина данных кратна 8 символам (32-битное число в hex)
    if (rawData.size() % 8 != 0 || rawData.size() / 8 != 4) {
        qDebug() << "Data length is invalid!";
        return values;
    }

    // Конвертировать каждую группу из 8 символов в float
    for (int i = 0; i < 4; ++i) {
        QByteArray hexValue = rawData.mid(i * 8, 8); // Берем 8 символов
        bool ok;
        uint32_t intValue = hexValue.toUInt(&ok, 16); // Преобразуем в uint32_t
        if (ok) {
            float floatValue;
            memcpy(&floatValue, &intValue, sizeof(floatValue)); // Преобразуем в float
            values[i] = static_cast<double>(floatValue); // Сохраняем результат
        } else {
            qDebug() << "Conversion error for:" << hexValue;
        }
    }

    return values;
}

void MainWindow::displayInformation(QTextEdit *logsEdit, QSerialPort* serial){
    if (serial->open(QIODevice::ReadWrite)) {
        if (serial->waitForReadyRead(1000)) {  // Ожидание данных 1 секунды
            QByteArray data = serial->readAll();
            logsEdit->append("Packet");
            QVector<double> values = parsePacket(data);
            // Вывод значений в logsEdit
            for (int i = 0; i < values.size(); ++i) {
                logsEdit->append(QString("Value %1: %2").arg(i + 1).arg(values[i], 0, 'f', 6));
            }
        } else {
            logsEdit->append("No data received within the timeout.");
        }
        serial->close();
        //logsEdit->append("Serial port closed.");
    } else {
        logsEdit->append("Failed to open serial port: " + serial->errorString());
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
