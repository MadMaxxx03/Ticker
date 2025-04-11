#include "extraparamsdialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QGroupBox>
#include <QApplication>
#include <QSettings>
#include <QDoubleValidator>
#include <QDir>

ExtraParamsDialog::ExtraParamsDialog(QWidget *parent, QSerialPort *port, QTextEdit *logOutput)
    : QDialog(parent), serialPort(port), logsEdit(logOutput)
{
    QFont labelFont = QApplication::font();
    labelFont.setPointSize(10);

    QVBoxLayout *controlLayout = new QVBoxLayout;

    labelControlType = new QLabel("Вид");
    labelControlType->setFont(labelFont);
    controlComboBox = new QComboBox();
    controlComboBox->setFont(labelFont);
    controlComboBox->addItem("PID регулятор");
    controlComboBox->addItem("Bang регулятор");
    connect(controlComboBox, &QComboBox::currentIndexChanged, this, &ExtraParamsDialog::updateFields);

    paramsLayout = new QGridLayout;

    QStringList pidParams = {"Fi_P", "Fi_I", "Fi_D", "W_P", "W_I", "W_D",
                             "X_P", "X_I", "X_D", "V_P", "V_I", "V_D"};

    QDoubleValidator *doubleValidator = new QDoubleValidator(this);

    for (const QString &param : pidParams) {
        QLabel *label = new QLabel(param);
        QLineEdit *edit = new QLineEdit;
        edit->setValidator(doubleValidator);
        label->setFont(labelFont);
        edit->setFont(labelFont);
        pidLabels.append(label);
        pidEdits.append(edit);
        paramsLayout->addWidget(label, paramsLayout->rowCount(), 0);
        paramsLayout->addWidget(edit, paramsLayout->rowCount() - 1, 1);
    }

    bangLabelFi = new QLabel("Fi");
    bangEditFi = new QLineEdit;
    bangLabelPWM = new QLabel("PWM");
    bangEditPWM = new QLineEdit;

    bangLabelFi->setFont(labelFont);
    bangEditFi->setFont(labelFont);
    bangLabelPWM->setFont(labelFont);
    bangEditPWM->setFont(labelFont);

    bangEditFi->setValidator(doubleValidator);
    bangEditPWM->setValidator(doubleValidator);

    paramsLayout->addWidget(bangLabelFi, 0, 0);
    paramsLayout->addWidget(bangEditFi, 0, 1);
    paramsLayout->addWidget(bangLabelPWM, 1, 0);
    paramsLayout->addWidget(bangEditPWM, 1, 1);

    saveButton = new QPushButton("Сохранить");
    connect(saveButton, &QPushButton::clicked, this, &ExtraParamsDialog::saveSettings);

    controlLayout->addWidget(labelControlType);
    controlLayout->addWidget(controlComboBox);
    controlLayout->addLayout(paramsLayout);
    controlLayout->addWidget(saveButton);

    setLayout(controlLayout);
    setWindowTitle("Параметры");

    loadSettings();
    updateFields(0);
}

void ExtraParamsDialog::updateFields(int index)
{
    bool isPID = (index == 0);

    for (int i = 0; i < pidLabels.size(); ++i) {
        pidLabels[i]->setVisible(isPID);
        pidEdits[i]->setVisible(isPID);
    }

    bangLabelFi->setVisible(!isPID);
    bangEditFi->setVisible(!isPID);
    bangLabelPWM->setVisible(!isPID);
    bangEditPWM->setVisible(!isPID);
}

void ExtraParamsDialog::loadSettings()
{
    QString appDir = QCoreApplication::applicationDirPath();
    QDir dir(appDir);

    if (dir.dirName() == "debug" || dir.dirName() == "release") {
        dir.cdUp();
    }
    dir.cdUp();

    QString configPath = dir.absolutePath() + "/app/config.ini";

    QSettings settings(configPath, QSettings::IniFormat);

    // Чтение данных из [Modified] или [Base] при отсутствии значений
    settings.beginGroup("Modified");
    for (int i = 0; i < pidEdits.size(); ++i) {
        QString paramName = pidLabels[i]->text();
        QString value = settings.value(paramName, "").toString();
        if (value.isEmpty()) {
            settings.endGroup();
            settings.beginGroup("Base");
            value = settings.value(paramName, "1.0").toString();
            settings.endGroup();
            settings.beginGroup("Modified"); // Вернемся в группу [Modified]
        }
        pidEdits[i]->setText(value);
    }

    bangEditFi->setText(settings.value("Fi", settings.value("Base/Fi", "1.0").toString()).toString());
    bangEditPWM->setText(settings.value("PWM", settings.value("Base/PWM", "1.0").toString()).toString());

    settings.endGroup();
}

void ExtraParamsDialog::setSerialPort(QSerialPort* port)
{
    serialPort = port;
}

void ExtraParamsDialog::saveSettings()
{
    QString appDir = QCoreApplication::applicationDirPath();
    QDir dir(appDir);

    if (dir.dirName() == "debug" || dir.dirName() == "release") {
        dir.cdUp();
    }
    dir.cdUp();

    QString configPath = dir.absolutePath() + "/app/config.ini";
    QSettings settings(configPath, QSettings::IniFormat);

    bool hasError = false;

    settings.beginGroup("Modified");

    for (int i = 0; i < pidEdits.size(); ++i) {
        if (pidEdits[i]->text().isEmpty()) {
            pidEdits[i]->setStyleSheet("border: 2px solid red");
            hasError = true;
        } else {
            settings.setValue(pidLabels[i]->text(), pidEdits[i]->text());
            pidEdits[i]->setStyleSheet("");
        }
    }

    if (bangEditFi->text().isEmpty()) {
        bangEditFi->setStyleSheet("border: 2px solid red");
        hasError = true;
    } else {
        settings.setValue("Fi", bangEditFi->text());
        bangEditFi->setStyleSheet("");
    }

    if (bangEditPWM->text().isEmpty()) {
        bangEditPWM->setStyleSheet("border: 2px solid red");
        hasError = true;
    } else {
        settings.setValue("PWM", bangEditPWM->text());
        bangEditPWM->setStyleSheet("");
    }

    settings.endGroup();

    if (!hasError) {
        settings.sync();

        QByteArray dataToSend = "#->2-412000003f19999a000000003f8000000000000000000000c2700000bdcccccd00000000000000000000000000000000\n\r";

        if (serialPort && serialPort->isOpen()) {
            logsEdit->append("Порт открыт. Отправка команды...");
            serialPort->write(dataToSend);
            serialPort->flush(); // Убедиться, что буфер очищен
            logsEdit->append("Команда отправлена (" + QString::number(dataToSend.size()) + " байт)");
        } else {
            logsEdit->append("Порт закрыт. Данные не отправлены.");
        }

        accept();
    }

}


