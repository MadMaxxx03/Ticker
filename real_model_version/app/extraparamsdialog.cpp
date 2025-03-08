#include "extraparamsdialog.h"
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QGroupBox>
#include <QApplication>

ExtraParamsDialog::ExtraParamsDialog(QWidget *parent) : QDialog(parent)
{
    QFont labelFont = QApplication::font();
    labelFont.setPointSize(10);

    // Управление
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

    controlComboBox = new QComboBox();
    controlComboBox->setFont(labelFont);
    controlComboBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    controlComboBox->setMaximumWidth(300);
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

    QGroupBox *controlGroup = new QGroupBox("Управление");
    controlGroup->setLayout(controLayout);

    // Кнопка "Сохранить"
    saveButton = new QPushButton("Сохранить");
    connect(saveButton, &QPushButton::clicked, this, &QDialog::accept);

    // Основной макет
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(controlGroup);
    mainLayout->addWidget(saveButton);

    setLayout(mainLayout);
    setWindowTitle("Параметры");
}
