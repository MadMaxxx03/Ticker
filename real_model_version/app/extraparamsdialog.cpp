#include "extraparamsdialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QGroupBox>
#include <QApplication>
#include <QSettings>
#include <QDoubleValidator>
#include <QDir>

ExtraParamsDialog::ExtraParamsDialog(QWidget *parent,
                                     QSerialPort *port,
                                     QTextEdit *logOutput,
                                     QCheckBox *swing,
                                     QCheckBox *stab,
                                     QCheckBox *filter)
    : QDialog(parent),
      serialPort(port),
      logsEdit(logOutput),
      checkBoxSwing(swing),
      checkBoxStab(stab),
      checkBoxFilter(filter)
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

        QByteArray  dataToSend = "#";
        dataToSend += "-";

        // 1. Формируем первый бинарный блок: 00111 + чекбоксы
        QString bin1 = "00111";
        bin1 += checkBoxSwing && checkBoxSwing->isChecked() ? "1" : "0";
        bin1 += checkBoxStab && checkBoxStab->isChecked() ? "1" : "0";
        bin1 += checkBoxFilter && checkBoxFilter->isChecked() ? "1" : "0";

        // Переводим в символ
        char char1 = static_cast<char>(bin1.toUInt(nullptr, 2));
        dataToSend += char1;

        // 2. Формируем второй бинарный блок: 0011000 + регулятор
        QString bin2 = "0011000";
        bool isPIDSelected = (controlComboBox->currentIndex() == 0);
        bin2 += isPIDSelected ? "1" : "0"; // переменная isPIDSelected должна быть определена где-то в диалоге
        char char2 = static_cast<char>(bin2.toUInt(nullptr, 2));
        dataToSend += char2;

        // 3. Добавляем разделитель
        dataToSend += "-";

        // 4. Добавляем 96 символов параметров
        QByteArray payload;
        QByteArray payloadBytes;

        if (isPIDSelected) {
            for (int i = 0; i < pidEdits.size(); ++i) {
                bool ok;
                float value = pidEdits[i]->text().toFloat(&ok);
                if (!ok) value = 0.0f;

                QByteArray floatBytes(reinterpret_cast<const char*>(&value), sizeof(float));
                payloadBytes.append(floatBytes);
            }
        } else {
            float fi = bangEditFi->text().toFloat();
            float pwm = bangEditPWM->text().toFloat();

            QByteArray fiBytes(reinterpret_cast<const char*>(&fi), sizeof(float));
            QByteArray pwmBytes(reinterpret_cast<const char*>(&pwm), sizeof(float));
            payloadBytes.append(fiBytes);
            payloadBytes.append(pwmBytes);

            // Дополняем до 96 байт (12 * 8 = 96 символов)
            while (payloadBytes.size() < 48) { // 48 байт = 96 hex символов
                payloadBytes.append(char(0));
            }
        }

        // Конвертируем в hex и переводим в upper-case
        payload = payloadBytes.toHex().toUpper();
        dataToSend += payload;

        // 5. Завершаем строку
        dataToSend += "\n\r";

        if (serialPort && serialPort->isOpen()) {
            logsEdit->append("Порт открыт. Отправка команды...");
            serialPort->write(dataToSend);
            serialPort->flush(); // Убедиться, что буфер очищен
            logsEdit->append("Команда отправлена " + dataToSend);

            QString statusText;
            if (checkBoxSwing)
                statusText += "Раскачка " + QString(checkBoxSwing->isChecked() ? "включена" : "отключена") + ", ";
            if (checkBoxStab)
                statusText += "Стабилизация " + QString(checkBoxStab->isChecked() ? "включена" : "отключена") + ", ";
            if (checkBoxFilter)
                statusText += "Фильтрация " + QString(checkBoxFilter->isChecked() ? "включена" : "отключена");

            // Удалим последнюю запятую и пробел если есть
            if (statusText.endsWith(", ")) statusText.chop(2);

            logsEdit->append(statusText);
        } else {
            logsEdit->append("Порт закрыт. Данные не отправлены.");
        }

        accept();
    }

}


