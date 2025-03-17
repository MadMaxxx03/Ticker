#include "extraparamsdialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QGroupBox>
#include <QApplication>
#include <QSettings>
#include <QDoubleValidator>

ExtraParamsDialog::ExtraParamsDialog(QWidget *parent) : QDialog(parent)
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
    QSettings settings("config.ini", QSettings::IniFormat);

    for (int i = 0; i < pidEdits.size(); ++i) {
        pidEdits[i]->setText(settings.value(pidLabels[i]->text(), "1.0").toString());
    }

    bangEditFi->setText(settings.value("Fi", "1.0").toString());
    bangEditPWM->setText(settings.value("PWM", "1.0").toString());
}

void ExtraParamsDialog::saveSettings()
{
    QSettings settings("config.ini", QSettings::IniFormat);

    bool hasError = false;

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

    if (!hasError) {
        accept();
    }
}
